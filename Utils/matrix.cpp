//
// Created by Yoyotao on 2022/5/4.
//

#include "matrix.h"


//Vector<2>::Vector(float x, float y) : x_(x), y_(y) {}
//
//float Vector<2>::GetLen() { return sqrtf(x_ * x_ + y_ * y_); }
//
//float &Vector<2>::X() { return x_; }
//
//float &Vector<2>::Y() { return y_; }
//
//
//Vector<3>::Vector(float x, float y, float z) : x_(x), y_(y), z_(z) {}
//
//float &Vector<3>::X() { return x_; }
//
//float &Vector<3>::Y() { return y_; }
//
//float &Vector<3>::Z() { return z_; }
//
//float Vector<3>::GetLen() { return sqrtf(x_ * x_ + y_ * y_ + z_ * z_); }
//
//
//Vector<4>::Vector(float x, float y, float z, float w) : x_(x), y_(y), z_(z), w_(w) {}
//
//float &Vector<4>::X() { return x_; }
//
//float &Vector<4>::Y() { return y_; }
//
//float &Vector<4>::Z() { return z_; }
//
//float &Vector<4>::W() { return w_; }
//
//float Vector<4>::GetLen() { return sqrtf(x_ * x_ + y_ * y_ + z_ * z_ + w_ * w_); }

//Vector3 Vector3::Normalize() const {
//    return {x_ / Length(), y_ / Length(), z_ / Length()};
//}
//
//float Vector3::Length() const {
//    return std::sqrt(x_ * x_ + y_ * y_ + z_ * z_);
//}
//
//Vector3 Vector3::Cross(Vector3 vertexes_) const {
//    return Vector3{this->GetY() * vertexes_.GetZ() - this->GetZ() * vertexes_.GetY(), this->GetZ() * vertexes_.GetX() - this->GetX() * vertexes_.GetZ(),
//                   this->GetX() * vertexes_.GetY() - this->GetY() * vertexes_.GetX()};
//}
//
//float Vector3::Dot(Vector3 vertexes_) const {
//    return this->GetX() * vertexes_.GetX() + this->GetY() * vertexes_.GetY() + this->GetZ() * vertexes_.GetZ();
//}
//
//Vector3 Vector3::Inv() const {
//    return {1.f / x_, 1.f / y_, 1.f / z_};
//}
//
//Vector3 operator+(Vector3 v1, Vector3 v2) {
//    return {v1.GetX() + v2.GetX(), v1.GetY() + v2.GetY(), v1.GetZ() + v2.GetZ()};
//}
//
//Vector3 operator*(float t, Vector3 vertexes_) {
//    return {t * vertexes_.GetX(), t * vertexes_.GetY(), t * vertexes_.GetZ()};
//}
//
//Vector3 operator*(Vector3 vertexes_, float t) {
//    return t * vertexes_;
//}
//
//Vector3 operator/(Vector3 v1, float t) {
//    return (1 / t) * v1;
//}
//
//Vector3 operator-(Vector3 v1, Vector3 v2) {
//    return {v1.GetX() - v2.GetX(), v1.GetY() - v2.GetY(), v1.GetZ() - v2.GetZ()};
//}
//
//Vector3 operator-(Vector3 vertexes_) {
//    return {-vertexes_.GetX(), -vertexes_.GetY(), -vertexes_.GetZ()};
//}
//
//std::ostream &operator<<(std::ostream &os, const Vector3 &vertexes_) {
//    os << "{ " << vertexes_.GetX() << ", " << vertexes_.GetY() << ", " << vertexes_.GetZ() << "}";
//    return os;
//}
//
//float GetDistanceBetween2Points(Vector3 v1, Vector3 v2) {
//    return sqrtf(powf(v1.GetX() - v2.GetX(), 2) + powf(v1.GetY() - v2.GetY(), 2) + powf(v1.GetZ() - v2.GetZ(), 2));
//}
//
//float CosThetaOf2Vec(const Vector3 &v1, const Vector3 &v2) {
//    return v1.Dot(v2) / (v1.Length() * v2.Length());
//}
//
//void Vector3::PointRotateAroundAxis(const Vector3 &p, const Vector3 &dir, float angle) {
//    *this = *this - p;
//    VectorRotateAroundAxis(p, dir, angle);
//    *this = *this + p;
//}
//
//void Vector3::VectorRotateAroundAxis(const Vector3 &p, const Vector3 &dir, float angle) {
//    float radian = angle / 180.f * (float) M_PI;
//    *this = *this * cosf(radian) + dir.Cross(*this) * sinf(radian) + dir * dir.Dot(*this) * (1 - cosf(radian));
//}
//
//Vector3 LocalToWorld(Vector3 v_loc, Vector3 w) {
//    Vector3 a;
//    if (1.f - fabsf(CosThetaOf2Vec(w, {1.f, 0.f, 0.f})) <= 0.1f) { // 和x轴几乎重合
//        a = {0.f, 1.f, 0.f};
//    } else { // 和x轴不重合
//        a = {1.f, 0.f, 0.f};
//    }
//    Vector3 u = w.Cross(a).Normalize(); // w和a的夹角并非90度，因此叉乘结果的模不为1，需要进行归一化
//    Vector3 vertexes_ = w.Cross(u);
//
//    return v_loc.GetX() * u + v_loc.GetY() * vertexes_ + v_loc.GetZ() * w;
//}
//
//bool IsNearZero(float f) {
//    return fabsf(f) < kEpsilon;
//}