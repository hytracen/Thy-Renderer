//
// Created by Yoyotao on 2022/7/26.
//

#ifndef RENDERER_CAMERA_H
#define RENDERER_CAMERA_H

#include "Utils/matrix.h"

class Camera {
public:
    Camera(Vector3f look_from, Vector3f look_at, Vector3f up);

//    void SetViewport(int width, int height);

    void SetFrustum(float aspect_ratio, float fov, float z_f, float z_n);

    void SetFrustum(float x_l, float x_r, float y_t, float y_b, float z_n, float z_f);

    Matrix4f GetModelMat();

    void SetModelMat(Matrix4f mat);

    Matrix4f GetCameraMat();

    [[nodiscard]] Matrix4f GetProjMat() const;

private:
//    int width_, height_;
    float fov_{}, aspect_ratio_{};
    float z_n_{}, z_f_{}, y_t_{}, y_b_{}, x_l_{}, x_r_{}; // 截锥
    Vector3f look_from_, look_at_, up_;
    Matrix4f m_model_{}, m_camera_{}, m_proj_{}, m_viewport_{};
};


#endif //RENDERER_CAMERA_H
