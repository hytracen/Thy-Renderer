//
// Created by Yoyotao on 2022/7/31.
//

#ifndef RENDERER_SHADER_STRUCT_H
#define RENDERER_SHADER_STRUCT_H

#include "Utils/matrix.h"
#include "texture.h"

struct VertShaderIn {
    Vector4f v;
};

struct VertShaderOut {
    Vector4f view_vert;
    Vector4f clip_vert;
};

struct FragShaderIn {
    FragShaderIn(const Vector3f &view_pos, const Vector2f &texCoord, const Vector3f &normal,
                 const std::shared_ptr<Texture> &texture)
            : view_pos_(view_pos), n_(normal), tex_coord_(texCoord), texture_(texture) {}

    Vector3f view_pos_ {};
    Vector3f n_ {};
    Vector2f tex_coord_ {};
    std::shared_ptr<Texture> texture_ ;
};

#endif //RENDERER_SHADER_STRUCT_H
