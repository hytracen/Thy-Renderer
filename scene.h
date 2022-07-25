//
// Created by Yoyotao on 2022/7/24.
//

#ifndef RENDERER_SCENE_H
#define RENDERER_SCENE_H

#include <vector>
#include <numeric>

#include "triangle.h"

class Scene {
public:
    Scene();

    void AddTri(Triangle* triangle);
    void Render(std::vector<std::vector<Vector3f>> &frame_buffer);

private:
    std::vector<Triangle*> triangles_;
    std::array<float, kWidth * kHeight> z_buffer;
};

bool IsInTriangle(float x, float y, const std::array<Vector4f, 3> &vertexes);

std::array<float, 3> ComputeBarycentric2D(float x, float y, const std::array<Vector4f, 3> &vertexes);

#endif //RENDERER_SCENE_H
