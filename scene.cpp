//
// Created by Yoyotao on 2022/7/24.
//

#include "scene.h"

Scene::Scene() {}

void Scene::AddTri(Triangle *triangle) {
    triangles_.push_back(triangle);
    z_buffer.fill(std::numeric_limits<float>::lowest());
}

void Scene::Render(std::vector<std::vector<Vector3f>> &frame_buffer) {
    for (auto t: triangles_) {
        Vector3f look_from{0.f, 0.f, 5.f};
        float fov = 45.f;
        float aspect_ratio = 1.f;
        float near = -0.1f;
        float far = -50.f;
        float top = std::fabs(near) * tanf(fov / 180.f * (float) M_PI);
        float right = top * aspect_ratio;
        float left = -right;
        float bottom = -top;

        Matrix4f m_camera = {1.f, 0.f, 0.f, -look_from.GetX(),
                             0.f, 1.f, 0.f, -look_from.GetY(),
                             0.f, 0.f, 1.f, -look_from.GetZ(),
                             0.f, 0.f, 0.f, 1.f};
        Matrix4f m_proj = {2.f * near / (right - left), 0.f, (left + right) / (left - right), 0.f,
                           0.f, 2 * near / (top - bottom), (bottom + top) / (bottom - top), 0.f,
                           0.f, 0.f, (far + near) / (near - far), 2 * far * near / (far - near),
                           0.f, 0.f, 1.f, 0.f};

        Matrix4f m_viewport = {(float) kWidth / 2, 0.f, 0.f, (float) kWidth / 2,
                               0.f, (float) kHeight / 2, 0.f, (float) kHeight / 2,
                               0.f, 0.f, 1.f, 0.f,
                               0.f, 0.f, 0.f, 1.f};
//        auto test1 = m_camera * t->GetVertexes().at(0);
//        auto test2 = m_proj * test1;
//        auto test3 = m_viewport * test2;


        Triangle triangle_2d{};
        for (int i = 0; i < 3; ++i) {
            auto vertex = m_viewport * m_proj * m_camera * t->GetVertexes().at(i);
            for (int j = 0; j < 4; ++j) { // 将齐次坐标的w值化为1
                vertex.SetData(j, 0, vertex.GetData().at(j).at(0) / vertex.GetW());
            }
            triangle_2d.SetVertexes(i, vertex);
        }

        auto [x_min, x_max, y_min, y_max] = triangle_2d.Get2DBoundingBox();
        for (int x = (int) x_min; x <= (int) x_max; ++x) {
            for (int y = (int) y_min; y <= (int) y_max; ++y) {
                auto vertexes = triangle_2d.GetVertexes();
                if (IsInTriangle((float) x + 0.5f, (float) y + 0.5f, vertexes)) {
                    auto [alpha, beta, gamma] = ComputeBarycentric2D((float) x + 0.5f, (float) y + 0.5f,
                                                                     vertexes);
                    float z_interp = alpha * vertexes.at(0).GetZ() + beta * vertexes.at(1).GetZ() +
                                     gamma * vertexes.at(2).GetZ();
                    int pixel_index = x * kWidth + y;
                    if (z_interp > z_buffer.at(pixel_index)) {
                        z_buffer.at(pixel_index) = z_interp;
                        frame_buffer.at(y).at(x) = {0.5f, 0.2f, 0.2f};
                    }
                }
            }
        }
    }
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
