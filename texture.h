//
// Created by Yoyotao on 2022/7/31.
//

#ifndef RENDERER_TEXTURE_H
#define RENDERER_TEXTURE_H

#include <cfloat>

#include <opencv2/opencv.hpp>

#include "Utils/matrix.h"

class Texture {
public:
    explicit Texture(const std::string &name);

    Vector3f GetColor(float u, float v);

private:
    int width_, height_;
    cv::Mat data_;
};


#endif //RENDERER_TEXTURE_H
