//
// Created by Yoyotao on 2022/7/24.
//

#ifndef RENDERER_SCENE_H
#define RENDERER_SCENE_H

#include <vector>
#include <numeric>

#include "opencv2/opencv.hpp"

#include "triangle.h"
#include "camera.h"

class Scene {
public:
    Scene(int width, int height);

    void AddTri(Triangle* triangle);

    void Clear();

    void ProcessInput(const int &pressed_key);

    void Render();

    void SetCamera(Camera* camera);

    [[nodiscard]] Matrix4f GetViewportMat() const;

    [[nodiscard]] const std::vector<std::vector<Vector3f>> &GetFrameBuffer() const;

private:
    int width_, height_;
    std::vector<Triangle*> triangles_;
    std::vector<std::vector<Vector3f>> frame_buffer_;
    std::vector<float> z_buffer;
    Camera* camera_;
};

bool IsInTriangle(float x, float y, const std::array<Vector4f, 3> &vertexes);

std::array<float, 3> ComputeBarycentric2D(float x, float y, const std::array<Vector4f, 3> &vertexes);

#endif //RENDERER_SCENE_H
