//
// Created by Yoyotao on 2022/7/24.
//

#include "triangle.h"

const std::array<Vector4f, 3> &Triangle::GetVertexes() const {
    return vertexes_;
}

void Triangle::SetVertexes(const std::array<Vector4f, 3> &v) {
    vertexes_ = v;
}

void Triangle::SetVertexes(int index, Vector4f vertex) {
    assert(index >= 0 && index <= 2);
    vertexes_.at(index) = vertex;
}

std::array<float, 4> Triangle::Get2DBoundingBox() {
    float x_min = std::min(vertexes_.at(0).GetX(), std::min(vertexes_.at(1).GetX(), vertexes_.at(2).GetX()));
    float x_max = std::max(vertexes_.at(0).GetX(), std::max(vertexes_.at(1).GetX(), vertexes_.at(2).GetX()));
    float y_min = std::min(vertexes_.at(0).GetY(), std::min(vertexes_.at(1).GetY(), vertexes_.at(2).GetY()));
    float y_max = std::max(vertexes_.at(0).GetY(), std::max(vertexes_.at(1).GetY(), vertexes_.at(2).GetY()));

    return {x_min, x_max, y_min, y_max};
}
