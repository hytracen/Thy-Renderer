//
// Created by Yoyotao on 2022/7/24.
//

#ifndef RENDERER_TRIANGLE_H
#define RENDERER_TRIANGLE_H

#include <array>

#include "Utils/matrix.h"

class Triangle {
public:
    Triangle() = default;

    std::array<float, 4> Get2DBoundingBox();

    void SetVertexes(const std::array<Vector4f, 3> &v);
    void SetVertexes(int index, Vector4f vertex);

    [[nodiscard]] const std::array<Vector4f, 3> &GetVertexes() const;

private:
    std::array<Vector4f, 3> vertexes_;

};


#endif //RENDERER_TRIANGLE_H
