//
// Created by Yoyotao on 2022/7/24.
//

#ifndef RENDERER_SCENE_H
#define RENDERER_SCENE_H

#include <vector>
#include <numeric>

#include "opencv2/opencv.hpp"

#include "triangle.h"
#include "tri_mesh.h"
#include "camera.h"
#include "shader_struct.h"

class Scene {
public:
    Scene(int width, int height);

    void AddTri(Triangle* triangle);

    void AddTriMesh(TriMesh* tri_mesh);

    void Clear();

    void ProcessInput(const int &pressed_key);

    void Render();

    void Rasterize(Triangle &t_viewport, Triangle &t_view);

    void SetCamera(Camera* camera);

    [[nodiscard]] Matrix4f GetViewportMat() const;

    [[nodiscard]] const std::vector<Vector3f> &GetFrameBuffer() const;

    void SetVertShader(const std::function<VertShaderOut(const VertShaderIn &)> &vertShader);

    void SetFragShader(const std::function<Vector3f(const FragShaderIn&)> &frag_shader);

private:

    int GetIndex(int x, int y);

private:
    int width_, height_;
    std::vector<Triangle*> triangles_;
    std::vector<TriMesh*> tri_meshes_;
    std::vector<Vector3f> frame_buffer_;
    std::vector<float> z_buffer_;

    Camera* camera_;
    std::function<Vector3f(const FragShaderIn&)> frag_shader_;
    std::function<VertShaderOut(const VertShaderIn&)> vert_shader_;
};

bool IsInTriangle(float x, float y, const std::array<Vector4f, 3> &vertexes);

std::array<float, 3> ComputeBarycentric2D(float x, float y, const std::array<Vector4f, 3> &vertexes);

#endif //RENDERER_SCENE_H
