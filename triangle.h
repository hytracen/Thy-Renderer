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
    explicit Triangle(std::array<Vector4f, 3> vertexes);

    std::array<float, 4> Get2DBoundingBox();

    void SetVertexes(const std::array<Vector4f, 3> &v);
    void SetVertex(int index, Vector4f vertex);
    void SetNormal(int index, Vector3f normal);

    [[nodiscard]] const std::array<Vector4f, 3> &GetVertexes() const;

    void MoveBy(Vector3f dir, float len);

    /**
     * 逆时针绕过原点的轴旋转
     * @param axis 旋转轴的方向
     * @param angle 旋转角度
     */
    void RotateBy(Vector3f axis, float angle);

private:
    std::array<Vector4f, 3> vertexes_{};
    std::array<Vector3f, 3> normals_{};
};


#endif //RENDERER_TRIANGLE_H
