#include "camera.h"

Camera::Camera(Vector3f look_from, Vector3f look_at, Vector3f up) : look_from_(look_from), look_at_(look_at), up_(up) {}

//void Camera::SetViewport(int width_, int height_) {
//    width_ = width_;
//    height_ = height_;
//    aspect_ratio_ = (float)width_ / (float)height_;
//}

void Camera::SetFrustum(float aspect_ratio, float fov, float z_f, float z_n) {
    z_n_ = z_n;
    z_f_ = z_f;
    fov_ = fov;
    aspect_ratio_ = aspect_ratio;
    y_t_ = std::fabs(z_n_) * tanf((fov_/2.f) / 180.f * (float) M_PI);
    x_r_ = y_t_ * aspect_ratio_;
    x_l_ = -x_r_;
    y_b_ = -y_t_;
}

void Camera::SetFrustum(float x_l, float x_r, float y_t, float y_b, float z_n, float z_f) {
    x_l_ = x_l;
    x_r_ = x_r;
    y_t_ = y_t;
    y_b_ = y_b;
    z_n_ = z_n;
    z_f_ = z_f;
}

Matrix4f Camera::GetCameraMat() {
    Matrix4f m_translate = {1.f, 0.f, 0.f, -look_from_.GetX(),
                            0.f, 1.f, 0.f, -look_from_.GetY(),
                            0.f, 0.f, 1.f, -look_from_.GetZ(),
                            0.f, 0.f, 0.f, 1.f};
    Vector3f right = Cross(up_, -look_at_);
    Matrix4f m_rotate = {right.GetX(), right.GetY(), right.GetZ(), 0.f,
                         up_.GetX(), up_.GetY(), up_.GetZ(), 0.f,
                         -look_at_.GetX(), -look_at_.GetY(), -look_at_.GetZ(), 0.f,
                         0.f, 0.f, 0.f, 1.f};
    return m_rotate * m_translate;
}

Matrix4f Camera::GetProjMat() const {
    Matrix4f m = {2.f * z_n_ / (x_r_ - x_l_), 0.f, (x_l_ + x_r_) / (x_l_ - x_r_), 0.f,
            0.f, 2 * z_n_ / (y_t_ - y_b_), (y_b_ + y_t_) / (y_b_ - y_t_), 0.f,
            0.f, 0.f, (z_f_ + z_n_) / (z_n_ - z_f_), 2 * z_f_ * z_n_ / (z_f_ - z_n_),
            0.f, 0.f, 1.f, 0.f};
    return m;
}

Vector4f Camera::VertexShader(const Vector4f &v) {
    return GetProjMat() * GetCameraMat() * v;
}

Vector3f Camera::FragShader(const std::array<Vector4f, 3> &vertexes) {
    Vector3f normal = Cross((vertexes.at(1) - vertexes.at(0)).ToVec3(), (vertexes.at(2) - vertexes.at(0)).ToVec3());
    normal = normal.GetNorm();
    return Vector3f{255.f,255.f,255.f} * std::fabs(Dot(normal, Vector3f{0.f,0.f,-1.f}));
}

const Vector3f &Camera::GetLookFrom() const {
    return look_from_;
}


