//
// Created by Yoyotao on 2022/7/31.
//

#include "texture.h"

Texture::Texture(const std::string &name) {
    data_ = cv::imread(name);
    cv::cvtColor(data_, data_, cv::COLOR_RGB2BGR);
    width_ = data_.cols;
    height_ = data_.rows;
}

Vector3f Texture::GetColor(float u, float v) {
    if (u < 0) u = 0;
    if (v < 0) v = 0;
    if (u > 1) u = 1;
    if (v > 1) v = 1;

    auto u_img = u * static_cast<float>(width_);
    auto v_img = (1 - v) * static_cast<float>(height_);
    cv::Vec<float, 3> color = data_.at<cv::Vec3b>(static_cast<int>(v_img), static_cast<int>(u_img));
    return {color[0], color[1], color[2]};
}
