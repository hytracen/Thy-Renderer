//
// Created by Yoyotao on 2022/7/24.
//

#include "scene.h"

Scene::Scene(int width, int height) : width_(width), height_(height) {
    frame_buffer_ = std::vector<Vector3f>(width_ * height_);
    z_buffer_ = std::vector<float>(width_ * height_, std::numeric_limits<float>::lowest());
}

int Scene::GetIndex(int x, int y) {
    return (width_ - 1 - y) * width_ + x;
}

void Scene::AddTri(Triangle *triangle) {
    triangles_.push_back(triangle);
}

void Scene::AddTriMesh(TriMesh *tri_mesh) {
    for (auto tri: tri_mesh->GetTriList()) {
        triangles_.push_back(tri);
    }
    tri_meshes_.push_back(tri_mesh);
}

void Scene::Clear() {
    for (int i = 0; i < frame_buffer_.size(); ++i) {
        frame_buffer_.at(i) = Vector3f{};
        z_buffer_.at(i) = std::numeric_limits<float>::lowest();
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
        t->RotateBy({0.f, 1.f, 0.f}, 10.f); // Model Transform

        auto vertexes = t->GetVertexes();
        auto t_view = *t;
        auto t_viewport = *t;

        for (int i: {0, 1, 2}) {
            // todo: normal
            vertexes.at(i) = camera_->GetCameraMat() * vertexes.at(i); // View Transform
            t_view.SetVertex(i, vertexes.at(i)); // View Space
            vertexes.at(i) = camera_->GetProjMat() * vertexes.at(i); // Proj Transform
        }

        // back face culling
        auto vec02 = vertexes.at(2).ToVec3() - vertexes.at(0).ToVec3();
        auto vec01 = vertexes.at(1).ToVec3() - vertexes.at(0).ToVec3();
        if (Cross(vec01, vec02).GetZ() < 0)
            continue;

        for (int i: {0, 1, 2}) {
            vertexes.at(i) = vertexes.at(i).GetProjDiv(); // Projection Division
//            vertexes.at(i) = GetViewportMat() * vertexes.at(i);
            // Viewport Transform
            vertexes.at(i).SetX((vertexes.at(i).GetX() + 1.f) / 2.f * (float)width_);
            vertexes.at(i).SetY((vertexes.at(i).GetY() + 1.f) / 2.f * (float)height_);
            t_viewport.SetVertex(i, vertexes.at(i));
        }

        Rasterize(t_viewport, t_view);

        ++tri_count;
    }
    std::cout << "triangles count:" << tri_count;
}

void Scene::SetCamera(Camera *camera) {
    camera_ = camera;
}

const std::vector<Vector3f> &Scene::GetFrameBuffer() const {
    return frame_buffer_;
}

Matrix4f Scene::GetViewportMat() const {
    return {(float) width_ / 2, 0.f, 0.f, (float) width_ / 2,
            0.f, (float) height_ / 2, 0.f, (float) height_ / 2,
            0.f, 0.f, 1.f, 0.f,
            0.f, 0.f, 0.f, 1.f};
}

void Scene::Rasterize(Triangle &t_viewport, Triangle &t_view) {
    auto [x_min, x_max, y_min, y_max] = t_viewport.Get2DBoundingBox();
    auto vertexes = t_viewport.GetVertexes();
    for (int x = (int) x_min; x <= (int) x_max && x < width_; ++x) {
        for (int y = (int) y_min; y <= (int) y_max && y < height_; ++y) {
            if (IsInTriangle((float) x + 0.5f, (float) y + 0.5f, vertexes)) {
                auto [alpha, beta, gamma] = ComputeBarycentric2D((float) x + 0.5f, (float) y + 0.5f,
                                                                 vertexes);
                // todo: z值的处理 https://github.com/ssloy/tinyrenderer/wiki/Technical-difficulties:-linear-interpolation-with-perspective-deformations
//                float z_interp = 1.0f / (alpha / vertexes.at(0).GetW() + beta / vertexes.at(1).GetW() +
//                                         gamma / vertexes.at(2).GetW());
//
//                z_interp *= (alpha * vertexes.at(0).GetZ() / vertexes.at(0).GetW()
//                             + beta * vertexes.at(1).GetZ() / vertexes.at(1).GetW()
//                             + gamma * vertexes.at(2).GetZ() / vertexes.at(2).GetW());
                float z_interp = 1.f / vertexes.at(0).GetW() * alpha
                               + 1.f / vertexes.at(1).GetW() * beta
                               + 1.f / vertexes.at(2).GetW() * gamma;
                z_interp = 1.f / z_interp;

                // 三维空间中的重心坐标
                float alpha3 = z_interp / vertexes.at(0).GetW() * alpha;
                float beta3 = z_interp / vertexes.at(1).GetW() * beta;
                float gamma3 = z_interp / vertexes.at(2).GetW() * gamma;

                auto tex_coord = t_viewport.GetTexCoord(0) * alpha3
                                               + t_viewport.GetTexCoord(1) * beta3
                                               + t_viewport.GetTexCoord(2) * gamma3;
                auto normal = t_viewport.GetNormal(0) * alpha3
                                            + t_viewport.GetNormal(1) * beta3
                                            + t_viewport.GetNormal(2) * gamma3;
                auto view_pos = t_view.GetVertex(0) * alpha3
                                              + t_view.GetVertex(1) * beta3
                                              + t_view.GetVertex(2) * gamma3;

                // todo: z值的正负
                int pixel_index = GetIndex(x, y);
                if (z_interp > z_buffer_.at(pixel_index)) {
                    z_buffer_.at(pixel_index) = z_interp;
                    frame_buffer_.at(pixel_index) = frag_shader_(FragShaderIn(view_pos.ToVec3(), tex_coord, normal.GetNorm(),
                                                                              tri_meshes_.at(0)->GetTexture()));
                }
            }
        }
    }
}

void Scene::SetFragShader(const std::function<Vector3f(const FragShaderIn &)> &frag_shader) {
    frag_shader_ = frag_shader;
}

void Scene::SetVertShader(const std::function<VertShaderOut(const VertShaderIn &)> &vertShader) {
    vert_shader_ = vertShader;
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
