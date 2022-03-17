#pragma once

#include <basis/seadTypes.h>
#include <math/seadMathPolicies.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace sead
{
template <typename T>
class Matrix22;

template <typename T>
class Matrix33;

template <typename T>
class Matrix34;

template <typename T>
class Matrix44;

template <typename T>
class Matrix22 : public Policies<T>::Mtx22Base
{
private:
    using Self = Matrix22<T>;

public:
    Matrix22() {}

    Matrix22(const Self& n) = default;

    Matrix22(T a00, T a01, T a10, T a11);

    T operator()(s32 i, s32 j) const;
    T& operator()(s32 i, s32 j);
    Self& operator=(const Self& n);

    void makeIdentity();
    void makeZero();

    void setInverse(const Self& n);
    void setInverseTranspose(const Self& n);
    void setMul(const Self& a, const Self& b);
    void setTranspose(const Self& n);
    void transpose();

    static const Matrix22 zero;
    static const Matrix22 ident;
};

template <typename T>
class Matrix33 : public Policies<T>::Mtx33Base
{
private:
    using Self = Matrix33<T>;
    using Mtx34 = Matrix34<T>;

    using QuatT = Quat<T>;
    using Vec3T = Vector3<T>;

public:
    Matrix33() {}

    Matrix33(const Self& n) = default;

    Matrix33(T a00, T a01, T a02, T a10, T a11, T a12, T a20, T a21, T a22);

    Matrix33(const Mtx34& mtx34);

    T operator()(s32 i, s32 j) const;
    T& operator()(s32 i, s32 j);
    Self& operator=(const Self& n);

    void makeIdentity();
    void makeZero();

    void setInverse(const Self& n);
    void setInverseTranspose(const Self& n);
    void setMul(const Self& a, const Self& b);
    void setMul(const Mtx34& a, const Self& b);
    void setMul(const Self& a, const Mtx34& b);
    void setTranspose(const Self& n);
    void transpose();

    void fromQuat(const QuatT& q);
    void makeR(const Vec3T& r);
    void makeRIdx(u32 xr, u32 yr, u32 zr);
    void makeRzxyIdx(u32 xr, u32 yr, u32 zr);
    void makeS(const Vec3T& s);
    void makeS(T x, T y, T z);
    void makeSR(const Vec3T& s, const Vec3T& r);
    void makeSRIdx(const Vec3T& s, const Vector3<u32>& r);
    void makeSRzxyIdx(const Vec3T& s, const Vector3<u32>& r);
    void toQuat(QuatT& q) const;

    static const Matrix33 zero;
    static const Matrix33 ident;
};

template <typename T>
class Matrix34 : public Policies<T>::Mtx34Base
{
private:
    using Self = Matrix34<T>;
    using Mtx33 = Matrix33<T>;
    using Mtx44 = Matrix44<T>;

    using QuatT = Quat<T>;
    using Vec3T = Vector3<T>;
    using Vec4T = Vector4<T>;

public:
    Matrix34() {}

    Matrix34(const Self& n) = default;

    Matrix34(T _00, T _01, T _02, T _03, T _10, T _11, T _12, T _13, T _20, T _21, T _22, T _23);

    Matrix34(const Mtx33& mtx33, const Vec3T& t = Vec3T::zero);
    Matrix34(const Mtx44& mtx44);

    T operator()(s32 i, s32 j) const;
    T& operator()(s32 i, s32 j);
    Self& operator=(const Self& n);

    void makeIdentity();
    void makeZero();

    void setInverse(const Self& n);
    void setInverse33(const Self& n);
    void setInverseTranspose(const Self& n);
    void setMul(const Self& a, const Self& b);
    void setMul(const Mtx33& a, const Self& b);
    void setTranspose(const Self& n);
    void transpose();

    void fromQuat(const QuatT& q);
    void makeQT(const QuatT& q, const Vec3T& t);
    void makeR(const Vec3T& r);
    void makeRIdx(u32 xr, u32 yr, u32 zr);
    void makeRT(const Vec3T& r, const Vec3T& t);
    void makeRTIdx(const Vector3<u32>& r, const Vec3T& t);
    void makeRzxyIdx(u32 xr, u32 yr, u32 zr);
    void makeRzxyTIdx(const Vector3<u32>& r, const Vec3T& t);
    void makeS(const Vec3T& s);
    void makeS(T x, T y, T z);
    void makeSR(const Vec3T& s, const Vec3T& r);
    void makeSRIdx(const Vec3T& s, const Vector3<u32>& r);
    void makeSRT(const Vec3T& s, const Vec3T& r, const Vec3T& t);
    void makeSRTIdx(const Vec3T& s, const Vector3<u32>& r, const Vec3T& t);
    void makeSRzxyIdx(const Vec3T& s, const Vector3<u32>& r);
    void makeSRzxyTIdx(const Vec3T& s, const Vector3<u32>& r, const Vec3T& t);
    void makeST(const Vec3T& s, const Vec3T& t);
    void makeT(const Vec3T& t);
    void makeT(T x, T y, T z);
    void toQuat(QuatT& q) const;

    void getBase(Vec3T& o, s32 axis) const;
    void getRow(Vec4T& o, s32 row) const;
    void getTranslation(Vec3T& o) const;

    void scaleAllElements(T s);
    void scaleBases(T sx, T sy, T sz);
    void setBase(s32 axis, const Vec3T& v);
    void setRow(s32 row, const Vec4T& v);
    void setTranslation(const Vec3T& t);
    void setTranslation(T x, T y, T z);

    static const Matrix34 zero;
    static const Matrix34 ident;
};

template <typename T>
class Matrix44 : public Policies<T>::Mtx44Base
{
private:
    using Self = Matrix44<T>;
    using Mtx33 = Matrix33<T>;
    using Mtx34 = Matrix34<T>;

    using QuatT = Quat<T>;
    using Vec3T = Vector3<T>;
    using Vec4T = Vector4<T>;

public:
    Matrix44() {}

    Matrix44(const Self& n) = default;

    Matrix44(T _00, T _01, T _02, T _03, T _10, T _11, T _12, T _13, T _20, T _21, T _22, T _23,
             T _30, T _31, T _32, T _33);

    Matrix44(const Mtx33& mtx33, const Vec3T& t = Vec3T::zero, const Vec4T& vw = Vec4T::ew);
    Matrix44(const Mtx34& mtx34, const Vec4T& vw = Vec4T::ew);

    T operator()(s32 i, s32 j) const;
    T& operator()(s32 i, s32 j);
    Self& operator=(const Self& n);

    void makeIdentity();
    void makeZero();

    void setInverse(const Self& n);
    void setInverseTranspose(const Self& n);
    void setMul(const Self& a, const Self& b);
    void setMul(const Mtx34& a, const Self& b);
    void setMul(const Self& a, const Mtx34& b);
    void setTranspose(const Self& n);
    void transpose();

    void fromQuat(const QuatT& q);
    void makeR(const Vec3T& r);
    void makeRIdx(u32 xr, u32 yr, u32 zr);
    void makeRzxyIdx(u32 xr, u32 yr, u32 zr);
    void toQuat(QuatT& q) const;

    void getCol(Vec4T& o, s32 axis) const;
    void getRow(Vec4T& o, s32 row) const;

    void scaleAllElements(T s);
    void scaleBases(T sx, T sy, T sz, T sw);
    void setCol(s32 axis, const Vec4T& v);
    void setRow(s32 row, const Vec4T& v);

    static const Matrix44 zero;
    static const Matrix44 ident;
};

typedef Matrix22<f32> Matrix22f;
typedef Matrix33<f32> Matrix33f;
typedef Matrix34<f32> Matrix34f;
typedef Matrix44<f32> Matrix44f;

typedef Matrix34<f32> Matrixf;

template <>
const Matrix22f Matrix22f::zero;

template <>
const Matrix22f Matrix22f::ident;

template <>
const Matrix33f Matrix33f::zero;

template <>
const Matrix33f Matrix33f::ident;

template <>
const Matrix34f Matrix34f::zero;

template <>
const Matrix34f Matrix34f::ident;

template <>
const Matrix44f Matrix44f::zero;

template <>
const Matrix44f Matrix44f::ident;

template <>
const Matrix22<f64> Matrix22<f64>::zero;

template <>
const Matrix22<f64> Matrix22<f64>::ident;

template <>
const Matrix33<f64> Matrix33<f64>::zero;

template <>
const Matrix33<f64> Matrix33<f64>::ident;

template <>
const Matrix34<f64> Matrix34<f64>::zero;

template <>
const Matrix34<f64> Matrix34<f64>::ident;

template <>
const Matrix44<f64> Matrix44<f64>::zero;

template <>
const Matrix44<f64> Matrix44<f64>::ident;

}  // namespace sead

#define SEAD_MATH_MATRIX_H_
#include <math/seadMatrix.hpp>
#undef SEAD_MATH_MATRIX_H_
