//
// Created by Yoyotao on 2022/7/24.
//

#include "scene.h"

Scene::Scene(int width, int height) : width_(width), height_(height) {
    frame_buffer_ = std::vector(height_, std::vector<Vector3f>(width_, Vector3f()));
    z_buffer = std::vector<float>(width_ * height_, std::numeric_limits<float>::lowest());
}

void Scene::AddTri(Triangle *triangle) {
    triangles_.push_back(triangle);
}

void Scene::AddTriMesh(TriMesh *tri_mesh) {
    for (auto tri: tri_mesh->GetTriList()) {
        triangles_.push_back(tri);
    }
}

void Scene::Clear() {
    for (int i = 0; i < height_; ++i) {
        for (int j = 0; j < width_; ++j) {
            frame_buffer_.at(i).at(j) = Vector3f{};
            z_buffer.at(i * width_ + j) = std::numeric_limits<float>::lowest();
        }
    }
}

void Scene::ProcessInput(const int &pressed_key) {
    float radian = 10.f / 180.f * (float) M_PI;
    switch (pressed_key) {
        case 'd': {

        }
            break;
        case 'a': {

        }
            break;
        case 27: {
            exit(-1);
        }
            break;
        default:
            break;
    }
}

void Scene::Render() {
    int tri_count = 0;
    for (auto &t: triangles_) {
        ++tri_count;
        t->RotateBy({0.f, 1.f, 0.f}, 10.f);
        Triangle triangle_viewport{};
        for (int i = 0; i < 3; ++i) {
            auto vertex = camera_->VertexShader(t->GetVertexes().at(i)); // vertex processing
            vertex = vertex.GetNormHomo(); // 齐次坐标化为1
            vertex = GetViewportMat() * vertex;
            triangle_viewport.SetVertex(i, vertex);
        }

        auto [x_min, x_max, y_min, y_max] = triangle_viewport.Get2DBoundingBox();
        auto vertexes = triangle_viewport.GetVertexes();
        for (int x = (int) x_min; x <= (int) x_max && x < width_; ++x) {
            for (int y = (int) y_min; y <= (int) y_max && y < height_; ++y) {
                if (IsInTriangle((float) x + 0.5f, (float) y + 0.5f, vertexes)) {
                    auto [alpha, beta, gamma] = ComputeBarycentric2D((float) x + 0.5f, (float) y + 0.5f,
                                                                     vertexes);
                    // todo: z值的处理 https://github.com/ssloy/tinyrenderer/wiki/Technical-difficulties:-linear-interpolation-with-perspective-deformations
                    float z_interp = 1.0f / (alpha / vertexes.at(0).GetW() + beta / vertexes.at(1).GetW() +
                                             gamma / vertexes.at(2).GetW());

                    z_interp *= (alpha * vertexes.at(0).GetZ() / vertexes.at(0).GetW()
                                 + beta * vertexes.at(1).GetZ() / vertexes.at(1).GetW()
                                 + gamma * vertexes.at(2).GetZ() / vertexes.at(2).GetW());

                    // todo: z值的正负
                    int pixel_index = y * width_ + x;
                    if (z_interp > z_buffer.at(pixel_index)) {
                        z_buffer.at(pixel_index) = z_interp;
                        frame_buffer_.at(y).at(x) = camera_->FragShader(t->GetVertexes());
                    }
                }
            }
        }
    }
    std::cout << "triangles count:" << tri_count;
}

void Scene::SetCamera(Camera *camera) {
    camera_ = camera;
}

const std::vector<std::vector<Vector3f>> &Scene::GetFrameBuffer() const {
    return frame_buffer_;
}

Matrix4f Scene::GetViewportMat() const {
    return {(float) width_ / 2, 0.f, 0.f, (float) width_ / 2,
            0.f, (float) height_ / 2, 0.f, (float) height_ / 2,
            0.f, 0.f, 1.f, 0.f,
            0.f, 0.f, 0.f, 1.f};
}

bool IsInTriangle(float x, float y, const std::array<Vector4f, 3> &vertexes) {
    Vector3f p{x, y, 0.f};
    std::array<Vector3f, 3> vertexes_3{};
    std::transform(vertexes.begin(), vertexes.end(), vertexes_3.begin(), [](const Vector4f &v) {
        return Vector3f{v.GetX(), v.GetY(), 0};
    });

    Vector3f ab = vertexes_3.at(1) - vertexes_3.at(0);
    Vector3f bc = vertexes_3.at(2) - vertexes_3.at(1);
    Vector3f ca = vertexes_3.at(0) - vertexes_3.at(2);

    if (Cross(ab, p - vertexes_3.at(0)).GetZ() > 0.f
        && Cross(bc, p - vertexes_3.at(1)).GetZ() > 0.f
        && Cross(ca, p - vertexes_3.at(2)).GetZ() > 0.f)
        return true;
    return false;
}

std::array<float, 3> ComputeBarycentric2D(float x, float y, const std::array<Vector4f, 3> &vertexes) {
    auto [a, b, c] = vertexes;
    float alpha = -(x - b.GetX()) * (c.GetY() - b.GetY()) + (y - b.GetY()) * (c.GetX() - b.GetX());
    alpha /= -(a.GetX() - b.GetX()) * (c.GetY() - b.GetY()) + (a.GetY() - b.GetY()) * (c.GetX() - b.GetX());
    float beta = -(x - c.GetX()) * (a.GetY() - c.GetY()) + (y - c.GetY()) * (a.GetX() - c.GetX());
    beta /= -(b.GetX() - c.GetX()) * (a.GetY() - c.GetY()) + (b.GetY() - c.GetY()) * (a.GetX() - c.GetX());
    float gamma = 1 - alpha - beta;
    return {alpha, beta, gamma};
}
