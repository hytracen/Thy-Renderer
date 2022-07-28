//
// Created by Yoyotao on 2022/7/28.
//

#ifndef RENDERER_SHADER_H
#define RENDERER_SHADER_H

#include "Utils/matrix.h"

class Shader {
    virtual Vector4f Vertex(const Vector4f& v) = 0;
    virtual bool Fragment(const Vector3f& bary_coord, Vector3f &color) = 0;
};

#endif //RENDERER_SHADER_H
