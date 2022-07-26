//
// Created by Yoyotao on 2022/5/4.
//

#ifndef RAYTRACER_VECTOR3_H
#define RAYTRACER_VECTOR3_H

#include <cmath>
#include <limits>
#include <iostream>
#include <array>
#include <algorithm>
#include <assert.h>

#include "constant_value.h"

//void Assert(bool exp, const std::string& msg) {
//    if (!exp) {
//        std::cerr << msg << std::endl;
//        exit(-1);
//    }
//}

template<typename T, int m, int n>
class Matrix{
public:
    Matrix() = default;

    explicit Matrix(std::array<std::array<T, n>, m> data) : data_(data) {};

    Matrix(std::initializer_list<T> list) {
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                data_.at(i).at(j) = *(list.begin() + (n * i + j));
            }
        }
    }

    Matrix<T,m,n>& operator=(Matrix<T,m,n> mat) {
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                data_.at(i).at(j) = mat.data_.at(i).at(j);
            }
        }
        return *this;
    }

    static Matrix<T, m, m> Identity() {
        Matrix<T, m, m> mat{};
        for (int i = 0; i < m; ++i) {
            mat.data_.at(i).at(i) = 1;
        }
        return mat;
    }

    T GetLen() {
        static_assert(m >= 1 && n == 1);
        T sum = 0;
        for (int i = 0; i < m; ++i) {
            sum += data_.at(i).at(0) * data_.at(i).at(0);
        }
        return sqrt(sum);
    }

    Matrix<T,m,1> GetNorm() {
        Matrix<T,m,1> mat{};
        T len = GetLen();
        for (int i = 0; i < m; ++i) {
            mat.SetData(i,0,data_.at(i).at(0) / len);
        }
        return mat;
    }

    T GetX() const {
        static_assert(m >= 1 && n == 1, "Not a valid vector");
        return data_.at(0).at(0);
    }

    T GetY() const {
        static_assert(m >= 2 && n == 1, "Not a valid vector");
        return data_.at(1).at(0);
    }

    T GetZ() const {
        static_assert(m >= 3 && n == 1, "Not a valid vector");
        return data_.at(2).at(0);
    }

    T GetW() const {
        static_assert(m >= 4 && n == 1, "Not a valid vector");
        return data_.at(3).at(0);
    }

    T SetX(T val) {
        static_assert(m >= 1 && n == 1, "Not a valid vector");
        data_.at(0).at(0) = val;
    }

    T SetY(T val) {
        static_assert(m >= 2 && n == 1, "Not a valid vector");
        data_.at(1).at(0) = val;
    }

    T SetZ(T val) {
        static_assert(m >= 3 && n == 1, "Not a valid vector");
        data_.at(2).at(0) = val;
    }

    T SetW(T val) {
        static_assert(m >= 4 && n == 1, "Not a valid vector");
        data_.at(3).at(0) = val;
    }

    const std::array<std::array<T, n>, m> &GetData() const { return data_; }

    void SetData(int i, int j, T val) {
        assert(i >= 0 && i < m && j >= 0 && j < n);
        data_.at(i).at(j) = val;
    }

    Matrix<T, 3, 1> ToVec3() const {
        assert(m == 4 && n == 1);
        Matrix<T, 3, 1> ordinary_m{};
        for (int i = 0; i < 3; ++i) {
            ordinary_m.SetData(i, 0, data_.at(i).at(0));
        }
        return ordinary_m;
    }

    Matrix<T, 4, 1> GetHomoCoordinate(int type) {
        assert(m == 3 && n == 1 && type >= 1 && type <= 2);
        Matrix<T, 4, 1> homo_m = {0.f,0.f,0.f,(T)type};
        for (int i = 0; i < 3; ++i) {
            homo_m.SetData(i, 0, data_.at(i).at(0));
        }
        return homo_m;
    }

    Matrix<T, 4, 1> GetProjDiv() {
        assert(m == 4 && n == 1);
        Matrix<T, 4, 1> homo_m = *this;
        homo_m.data_.at(0).at(0) /= homo_m.data_.at(3).at(0);
        homo_m.data_.at(1).at(0) /= homo_m.data_.at(3).at(0);
        homo_m.data_.at(2).at(0) /= homo_m.data_.at(3).at(0);
        return homo_m;
    }

private:
    std::array<std::array<T, n>, m> data_;
};

template<typename T, int m>
using Vector = Matrix<T, m, 1>;

using Vector2f = Matrix<float, 2, 1>;
using Vector3f = Matrix<float, 3, 1>;
using Vector4f = Matrix<float, 4, 1>;

using Matrix4f = Matrix<float, 4, 4>; // 四阶方阵

template<typename T, int m, int n>
Matrix<T, m, n> operator+(const Matrix<T, m, n> &lm, const Matrix<T, m, n> &rm) {
    Matrix<T, m, n> result_m{};
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            result_m.SetData(i, j, lm.GetData().at(i).at(j) + rm.GetData().at(i).at(j));
        }
    }
    return result_m;
}

template<typename T, int m, int n>
Matrix<T, m, n> operator-(const Matrix<T, m, n> &lm, const Matrix<T, m, n> &rm) {
    Matrix<T, m, n> result_m{};
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            result_m.SetData(i, j, lm.GetData().at(i).at(j) - rm.GetData().at(i).at(j));
        }
    }
    return result_m;
}

template<typename T, int m, int n>
Matrix<T, m, n> operator-(const Matrix<T, m, n> &mat) {
    Matrix<T, m, n> result_m{};
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            result_m.SetData(i, j, -mat.GetData().at(i).at(j));
        }
    }
    return result_m;
}

template<typename T, int m, int n, int t>
Matrix<T, m, t> operator*(const Matrix<T, m, n> &lm, const Matrix<T, n, t> &rm) {
    Matrix<T, m, t> result_m{};
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < t; ++j) {
            float ij_val = 0.f;
            for (int k = 0; k < n; ++k) {
                ij_val += lm.GetData().at(i).at(k) * rm.GetData().at(k).at(j);
            }
            result_m.SetData(i, j, ij_val);
        }
    }
    return result_m;
}

template<typename T, int m, int n>
Matrix<T, m, n> operator*(const Matrix<T, m, n> &mat, T val) {
    Matrix<T, m, n> result_m{};
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            result_m.SetData(i, j, mat.GetData().at(i).at(j) * val);
        }
    }
    return result_m;
}

template<typename T>
Vector<T, 3> Cross(const Vector<T, 3> &lv, const Vector<T, 3> &rv) {
    return {lv.GetY() * rv.GetZ() - lv.GetZ() * rv.GetY(),
            lv.GetZ() * rv.GetX() - lv.GetX() * rv.GetZ(),
            lv.GetX() * rv.GetY() - lv.GetY() * rv.GetX()};
}

template<typename T>
T Dot(const Vector<T, 3> &lv, const Vector<T, 3> &rv) {
    return lv.GetX() * rv.GetX() + lv.GetY() * rv.GetY() + lv.GetZ() * rv.GetZ();
}

template<typename T, int m, int n>
Matrix<T, m, n> WiseProduct(const Matrix<T, m, n> &lv, const Matrix<T, m, n> &rv) {
    Matrix<T, m, n> matrix {};
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            matrix.SetData(i, j, lv.GetData().at(i).at(j) * rv.GetData().at(i).at(j));
        }
    }
    return matrix;
}

//template<>
//class Vector<2> {
//public:
//    Vector() = default;
//
//    Vector(float x, float y);
//
//    float GetLen();
//
//    float &X();
//
//    float &Y();
//
//private:
//    float x_{}, y_{};
//};
//
//template<>
//class Vector<3> {
//public:
//    Vector()= default;
//
//    Vector(float x, float y, float z);
//
//    float GetLen();
//
//    float &X();
//
//    float &Y();
//
//    float &Z();
//
//private:
//    float x_{}, y_{}, z_{};
//};
//
//template<>
//class Vector<4> {
//public:
//    Vector()= default;
//
//    Vector(float x, float y, float z, float w);
//
//    float GetLen();
//
//    float &X();
//
//    float &Y();
//
//    float &Z();
//
//    float &W();
//
//private:
//    float x_{}, y_{}, z_{}, w_{};
//};
//
//typedef Vector<2> Vector2;
//typedef Vector<3> Vector3;
//typedef Vector<4> Vector4;

//class Vector3 {
//public:
//    Vector3() : x_(0.f), y_(0.f), z_(0.f) {};
//
//    Vector3(float x, float y, float z) : x_(x), y_(y), z_(z) {};
//
//    float GetX() const { return x_; }
//
//    float GetY() const { return y_; }
//
//    float GetZ() const { return z_; }
//
//    void SetX(float x) { x_ = x; }
//
//    void SetY(float y) { y_ = y; }
//
//    void SetZ(float z) { z_ = z; }
//
//    Vector3 Normalize() const;
//
//    float Length() const;
//
//    Vector3 Inv() const;
//
//    Vector3 Cross(Vector3 vertexes_) const;
//
//    float Dot(Vector3 vertexes_) const;
//
//    Vector3 operator*(Vector3 vertexes_) const {
//        return {GetX() * vertexes_.GetX(), GetY() * vertexes_.GetY(), GetZ() * vertexes_.GetZ()};
//    }
//
//    Vector3 &operator+=(Vector3 vertexes_) {
//        this->x_ += vertexes_.GetX();
//        this->y_ += vertexes_.GetY();
//        this->z_ += vertexes_.GetZ();
//        return *this;
//    }
//
//    Vector3 &operator/=(float t) {
//        this->x_ /= t;
//        this->y_ /= t;
//        this->z_ /= t;
//        return *this;
//    }
//
//    /**
//     * 将一个点绕轴旋转给定的角度
//     * @param p 轴点
//     * @param dir 轴的方向
//     * @param angle 逆时针旋转的角度
//     */
//    void PointRotateAroundAxis(const Vector3 &p, const Vector3 &dir, float angle);
//
//    /**
//     * 将一个向量绕轴旋转给定的角度
//     * @param p 轴点
//     * @param dir 轴的方向
//     * @param angle 逆时针旋转的角度
//     */
//    void VectorRotateAroundAxis(const Vector3 &p, const Vector3 &dir, float angle);
//
//private:
//    float x_ = 0.f, y_ = 0.f, z_ = 0.f;
//};
//
//Vector3 operator+(Vector3 v1, Vector3 v2);
//
//Vector3 operator*(float t, Vector3 vertexes_);
//
//Vector3 operator*(Vector3 vertexes_, float t);
//
//Vector3 operator/(Vector3 v1, float t);
//
//Vector3 operator-(Vector3 v1, Vector3 v2);
//
//Vector3 operator-(Vector3 vertexes_);
//
//std::ostream &operator<<(std::ostream &os, const Vector3 &vertexes_);
//
///**
// * 计算两个向量夹角的余弦值
// * @param v1
// * @param v2
// * @return
// */
//float CosThetaOf2Vec(const Vector3 &v1, const Vector3 &v2);
//
//// 计算两点间的距离
//float GetDistanceBetween2Points(Vector3 v1, Vector3 v2);
//
///**
// * 将Local坐标系（以n为z轴）的向量转换到世界坐标系（以（0，0，1）为z轴）下
// * @param vertexes_ 待转换的向量
// * @param n 局部坐标系的z轴
// * @return 转换后的向量
// */
//Vector3 LocalToWorld(Vector3 vertexes_, Vector3 n);
//
///**
// * 判断浮点数是否为0
// */
//bool IsNearZero(float f);

#endif //RAYTRACER_VECTOR3_H
