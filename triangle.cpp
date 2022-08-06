//
// Created by Yoyotao on 2022/7/24.
//

#include "triangle.h"

Triangle::Triangle(std::array<Vector4f, 3> vertexes) : vertexes_(vertexes) {

}

const std::array<Vector4f, 3> &Triangle::GetVertexes() const {
    return vertexes_;
}

void Triangle::SetVertexes(const std::array<Vector4f, 3> &v) {
    vertexes_ = v;
}

void Triangle::SetVertex(int index, Vector4f vertex) {
    assert(index >= 0 && index <= 2);
    vertexes_.at(index) = vertex;
}

void Triangle::SetNormal(int index, Vector3f normal) {
    assert(index >= 0 && index <= 2);
    normals_.at(index) = normal;
}

void Triangle::SetTexCoord(int index, Vector2f tex_coord) {
    assert(index >= 0 && index <= 2);
    tex_coords_.at(index) = tex_coord;
}

std::array<float, 4> Triangle::Get2DBoundingBox() {
    float x_min = std::min(vertexes_.at(0).GetX(), std::min(vertexes_.at(1).GetX(), vertexes_.at(2).GetX()));
    float x_max = std::max(vertexes_.at(0).GetX(), std::max(vertexes_.at(1).GetX(), vertexes_.at(2).GetX()));
    float y_min = std::min(vertexes_.at(0).GetY(), std::min(vertexes_.at(1).GetY(), vertexes_.at(2).GetY()));
    float y_max = std::max(vertexes_.at(0).GetY(), std::max(vertexes_.at(1).GetY(), vertexes_.at(2).GetY()));

    return {x_min, x_max, y_min, y_max};
}

void Triangle::MoveBy(Vector3f dir, float len) {
    Vector3f t = dir * len;
    Matrix4f mat = {1.f, 0.f, 0.f, t.GetX(),
                    0.f, 1.f, 0.f, t.GetY(),
                    0.f, 0.f, 1.f, t.GetZ(),
                    0.f, 0.f, 0.f, 1.f};
    for (int i : {0,1,2}) {
        vertexes_.at(i) = (mat * vertexes_.at(i)).GetProjDiv();
        normals_.at(i) = (mat * (normals_.at(i)).GetHomoCoordinate(0)).ToVec3();
    }
    for (auto &v: vertexes_) {
        v = (mat * v).GetProjDiv();
    }
}

void Triangle::RotateBy(Vector3f axis, float angle) {
    float radian = angle / 180.f * (float) M_PI;
    std::array<Vector3f, 3> vertexes_3{};
    std::transform(vertexes_.begin(), vertexes_.end(), vertexes_3.begin(),
                   [](const Vector4f &v) { return Vector3f{v.GetX(), v.GetY(), v.GetZ()}; });
    for (int i : {0, 1, 2}) {
        Vector3f v = vertexes_3.at(i);
        v = v * cosf(radian) + Cross(axis, v) * sinf(radian) + axis * Dot(axis,v)*(1- cosf(radian));
        vertexes_.at(i) = v.GetHomoCoordinate(1);
        //todo:法线旋转
        Vector3f n = normals_.at(i);
        n = n * cosf(radian) + Cross(axis, n) * sinf(radian) + axis * Dot(axis,n)*(1- cosf(radian));
        normals_.at(i) = n;
    }
}

const std::array<Vector3f, 3> &Triangle::GetNormals() const {
    return normals_;
}

const Vector2f &Triangle::GetTexCoord(int index) {
    assert(index >= 0 && index <= 2);
    return tex_coords_.at(index);
}

const Vector3f &Triangle::GetNormal(int index) const {
    assert(index >= 0 && index <= 2);
    return normals_.at(index);
}

const Vector4f &Triangle::GetVertex(int index) const {
    assert(index >= 0 && index <= 2);
    return vertexes_.at(index);
}
