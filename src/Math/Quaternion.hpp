/*
 * quaternion.hpp
 *
 *  Copyright (c) 2014 G. Cross and C. Qu. MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 *        Created on: 12/13/2013
 *  Contributors: G. Cross gcross [dot] code [at] icloud.com
 *                C. Qu quchao [at] seas.upenn.edu
 */

/* Modified code by Quentin Quadrat <lecrapouille@gmail.com>.
 * See original code https://github.com/gareth-cross/quat/blob/master/include/quaternion.hpp */

#ifndef QUATERNION_HPP
#define QUATERNION_HPP

#include <cmath>
#include <limits>
#include <type_traits>

#  pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Wold-style-cast"
#    pragma GCC diagnostic ignored "-Wfloat-equal"
#    pragma GCC diagnostic ignored "-Wsign-conversion"
#      include "units/units.hpp"
#  pragma GCC diagnostic pop

/**
 *  @class Quat
 *  @brief Template representation of a quaternion (a, (b,c,d))
 *  where:
 *    - \c a is the scalar component
 *    - \c b is the first complex dimension (i)
 *    - \c c is the second complex dimension (j)
 *    - \c d is the third complex dimension (k)
 *
 *  Supports multiplication, addition, scaling and integration.
 */
template <typename Scalar> class Quat
{
    static_assert(std::is_fundamental<Scalar>::value &&
                  !std::numeric_limits<Scalar>::is_integer,
                  "Scalar must be a floating point type");

public:

    /**
     *  @brief Create a quaternion with null rotation
     */
    Quat()
    {
        m_q[0] = Scalar(1);
        m_q[1] = m_q[2] = m_q[3] = Scalar(0);
    }

    /**
     *  @brief Construct a quaterion
     *  @param a Scalar parameter
     *  @param b,c,d Complex parameters (i, j, k)
     */
    Quat(Scalar a, Scalar b, Scalar c, Scalar d)
    {
        m_q[0] = a;
        m_q[1] = b;
        m_q[2] = c;
        m_q[3] = d;
    }

    /**
     *  @brief Construct a quaterion
     *  @param s Scalar parameter
     *  @param v Complex parameters (i, j, k)
     */
    Quat(Scalar s, Vector<Scalar, 3_z> const& v)
    {
        m_q[0] = s;
        m_q[1] = v[0];
        m_q[2] = v[1];
        m_q[3] = v[2];
    }

    inline friend std::ostream& operator<<(std::ostream& os, Quat& q)
    {
        return os << q.a() << " + " << q.b() << "i + "
                  << q.c() << "j + " << q.d() << "k";
    }

    units::angle::radian_t angle()
    {
        return units::angle::radian_t(std::acos(a()) * Scalar(2));
    }

    Vector<Scalar, 3_z> axis()
    {
        Scalar tmp1 = static_cast<Scalar>(1) - a() * a();
        if(tmp1 <= static_cast<Scalar>(0))
            return Vector<Scalar, 3_z>(0, 0, 1);
        Scalar tmp2 = static_cast<Scalar>(1) / std::sqrt(tmp1);
        return Vector<Scalar, 3_z>(b() * tmp2, c() * tmp2, d() * tmp2);
    }

    /**
     *  @brief L2 norm of the quaternion
     */
    Scalar norm() const
    {
        return std::sqrt(a() * a() + b() * b() + c() * c() + d() * d());
    }

    /**
     *  @brief Normalize quaternion
     */
    void normalize()
    {
        const Scalar n = norm();
        operator*=(*this, 1 / n);
    }

    /**
     *  @brief Complex conjugate quaternion
     */
    Quat conjugate() const
    {
        return Quat(a(), -b(), -c(), -d());
    }

    /**
     *  @brief Rotate a vector using this quaternion
     *  @param v Vector stored in the three complex terms
     */
    Quat transform(const Quat &v) const
    {
        const Quat &q = *this;
        return q * v * q.conjugate();
    }

    /**
     *  @brief Convert a rotation quaternion to its matrix form
     *  @note The result is not correct if this quaternion is not a member of S(4)
     *  @return 4x4 Rotation matrix
     */
    Matrix<Scalar, 4_z, 4_z> toMatrix() const
    {
		Matrix<Scalar, 4_z, 4_z> Result(matrix::Identity);
		Scalar qxx(b() * b());
		Scalar qyy(c() * c());
		Scalar qzz(d() * d());
		Scalar qxz(b() * d());
		Scalar qxy(b() * c());
		Scalar qyz(c() * d());
		Scalar qwx(a() * b());
		Scalar qwy(a() * c());
		Scalar qwz(a() * d());

		Result[0][0] = Scalar(1) - Scalar(2) * (qyy +  qzz);
		Result[0][1] = Scalar(2) * (qxy + qwz);
		Result[0][2] = Scalar(2) * (qxz - qwy);

		Result[1][0] = Scalar(2) * (qxy - qwz);
		Result[1][1] = Scalar(1) - Scalar(2) * (qxx +  qzz);
		Result[1][2] = Scalar(2) * (qyz + qwx);

		Result[2][0] = Scalar(2) * (qxz + qwy);
		Result[2][1] = Scalar(2) * (qyz - qwx);
		Result[2][2] = Scalar(1) - Scalar(2) * (qxx +  qyy);

		return Result;
#if 0
        Matrix<Scalar, 4_z, 4_z> R(matrix::Identity); // TODO to be optimized

        const Scalar aa = a() * a();
        const Scalar bb = b() * b();
        const Scalar cc = c() * c();
        const Scalar dd = d() * d();

        R(0, 0) = aa + bb - cc - dd;
        R(1, 0) = 2 * b() * c() + 2 * a() * d();
        R(2, 0) = 2 * b() * d() - 2 * a() * c();

        R(0, 1) = 2 * b() * c() - 2 * a() * d();
        R(1, 1) = aa - bb + cc - dd;
        R(2, 1) = 2 * c() * d() + 2 * a() * b();

        R(0, 2) = 2 * b() * d() + 2 * a() * c();
        R(1, 2) = 2 * c() * d() - 2 * a() * b();
        R(2, 2) = aa - bb - cc + dd;

        return R;
#endif
    }

    /**
     *  @brief Integrate a rotation quaternion using 4th order Runge Kutta
     *  @param w Angular velocity (body frame), stored in 3 complex terms
     *  @param dt Time interval in seconds
     *  @param normalize If true, quaternion is normalized after integration
     */
    void integrateRungeKutta4(const Quat &w, Scalar dt, bool normalize = true)
    {
        const static Scalar half = static_cast<Scalar>(0.5);
        const static Scalar two = static_cast<Scalar>(2);

        Quat &q = *this;
        Quat qw = q * w * half;

        Quat k2 = (q + qw * dt * half) * w * half;
        Quat k3 = (q + k2 * dt * half) * w * half;
        Quat k4 = (q + k3 * dt) * w * half;

        q += (qw + k2 * two + k3 * two + k4) * (dt / 6);

        if (normalize)
        {
            q.normalize();
        }
    }

    /**
     *  @brief Integrate a rotation quaterion using Euler integration
     *  @param w Angular velocity (body frame), stored in 3 complex terms
     *  @param dt Time interval in seconds
     *  @param normalize If True, quaternion is normalized after integration
     */
    void integrateEuler(const Quat &w, Scalar dt, bool normalize = true)
    {
        Quat &q = *this;
        q += (q * w * static_cast<Scalar>(0.5)) * dt;

        if (normalize)
        {
            q.normalize();
        }
    }

    /**
     *  @brief Create a rotation quaterion
     *  @param theta Angle of rotation, radians
     *  @param x X component of rotation vector
     *  @param y Y component
     *  @param z Z component
     */
    static Quat rotation(units::angle::radian_t theta, Scalar x, Scalar y, Scalar z)
    {
        const Scalar haversine = std::sin(theta.to<Scalar>() / 2);
        const Scalar havercosine = std::cos(theta.to<Scalar>() / 2);

        return Quat(havercosine, haversine * x, haversine * y, haversine * z);
    }

    /**
     *  @brief Create a rotation quaternion from rotation vector [x,y,z]
     *  @note If x/y/z have 0 norm, this function returns the identity transform
     */
    static Quat rotation(Scalar x, Scalar y, Scalar z)
    {
        const Scalar theta = std::sqrt(x * x + y * y + z * z);

        if (theta < std::numeric_limits<Scalar>::epsilon() * 10)
        {
            return Quat();
        }

        return rotation(theta, x / theta, y / theta, z / theta);
    }

    /**
     *  @brief Create quaternion from matrix
     *  @param m Rotation matrix, should be a member of SO(3).
     *  @param Matrix Any matrix-like type which supports the (i,j) operator.
     *  @note All singularities are handled, provided m belongs to SO(3).
     *  @see
     * http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/index.htm
     */
    static Quat fromMatrix(Matrix<Scalar, 4_z, 4_z> const& m)
    {
        Quat Q;
        const Scalar tr = m(0, 0) + m(1, 1) + m(2, 2); //  trace
        Scalar s = 0;

        if (tr > 0)
        {
            s = 2 * std::sqrt(1 + tr);

            Q.a() = s / 4;
            Q.b() = (m(2, 1) - m(1, 2)) / s;
            Q.c() = (m(0, 2) - m(2, 0)) / s;
            Q.d() = (m(1, 0) - m(0, 1)) / s;
        }
        else if (m(0, 0) > m(1, 1) && m(0, 0) > m(2, 2))
        {
            s = 2 * std::sqrt(1 + m(0, 0) - m(1, 1) - m(2, 2));

            Q.a() = (m(2, 1) - m(1, 2)) / s;
            Q.b() = s / 4;
            Q.c() = (m(0, 1) + m(1, 0)) / s;
            Q.d() = (m(0, 2) + m(2, 0)) / s;
        }
        else if (m(1, 1) > m(2, 2))
        {
            s = 2 * std::sqrt(1 + m(1, 1) - m(0, 0) - m(2, 2));

            Q.a() = (m(0, 2) - m(2, 0)) / s;
            Q.b() = (m(0, 1) + m(1, 0)) / s;
            Q.c() = s / 4;
            Q.d() = (m(1, 2) + m(2, 1)) / s;
        }
        else
        {
            s = 2 * std::sqrt(1 + m(2, 2) - m(0, 0) - m(1, 1));

            Q.a() = (m(1, 0) - m(0, 1)) / s;
            Q.b() = (m(0, 2) + m(2, 0)) / s;
            Q.c() = (m(1, 2) + m(2, 1)) / s;
            Q.d() = s / 4;
        }

        return Q;
    }

    void generateScalarComponent()
    {
        Scalar w = Scalar(1) - (b() * b()) - (c() * c()) - (d() * d());
        if (w < Scalar(0))
            a() = Scalar(0);
        else
            a() = - std::sqrt(w);
    }


    /*
     *  Accessors
     */

    /**
     * @brief operator () Element-wise accessor
     * @param i Index into quaternion, must be less than 4.
     * @return Element i of the quaternion.
     */
    Scalar &operator()(size_t i) { return m_q[i]; }
    const Scalar &operator()(size_t i) const { return m_q[i]; }

    Scalar &a() { return m_q[0]; } /**< Scalar component */
    const Scalar &a() const { return m_q[0]; }

    Scalar &b() { return m_q[1]; } /**< First complex dimension (i) */
    const Scalar &b() const { return m_q[1]; }

    Scalar &c() { return m_q[2]; } /**< Second complex dimension (j) */
    const Scalar &c() const { return m_q[2]; }

    Scalar &d() { return m_q[3]; } /**< Third complex dimension (k) */
    const Scalar &d() const { return m_q[3]; }

    Scalar &w() { return m_q[0]; } /**< Scalar component */
    const Scalar &w() const { return m_q[0]; }

    Scalar &x() { return m_q[1]; } /**< First complex dimension (i) */
    const Scalar &x() const { return m_q[1]; }

    Scalar &y() { return m_q[2]; } /**< Second complex dimension (j) */
    const Scalar &y() const { return m_q[2]; }

    Scalar &z() { return m_q[3]; } /**< Third complex dimension (k) */
    const Scalar &z() const { return m_q[3]; }

private:

    Scalar m_q[4];
};

/*
 *  Typedefs for common types
 */
typedef Quat<double> Quatd;
typedef Quat<float> Quatf;

/**
 *  @brief Multiply two quaternions
 *  @param a Left quaternion
 *  @param b Right quaternion
 *  @return Product of both quaternions
 */
template <typename Scalar>
Quat<Scalar> operator*(const Quat<Scalar> &a, const Quat<Scalar> &b)
{
    Quat<Scalar> lhs;

    lhs(0) = a(0) * b(0) - a(1) * b(1) - a(2) * b(2) - a(3) * b(3);
    lhs(1) = a(0) * b(1) + a(1) * b(0) + a(2) * b(3) - a(3) * b(2);
    lhs(2) = a(0) * b(2) - a(1) * b(3) + a(2) * b(0) + a(3) * b(1);
    lhs(3) = a(0) * b(3) + a(1) * b(2) - a(2) * b(1) + a(3) * b(0);

    return lhs;
}

/**
 *  @brief Divide a quaternion by a scalar
 *  @param a Quaternion
 *  @param s Scalar
 *  @return Scaled quaternion
 */
template <typename Scalar>
Quat<Scalar> operator/(const Quat<Scalar> &a, const Scalar s)
{
    return operator*(a, 1 / s);
}

/**
 *  @brief Right-multiply a quaternion by a scalar
 *  @param a Quaternion
 *  @param s Scalar
 *  @return Scaled quaterion
 */
template <typename Scalar>
Quat<Scalar> operator*(const Quat<Scalar> &a, const Scalar s)
{
    Quat<Scalar> lhs;

    size_t i = 4u;
    while (i--)
    {
        lhs(i) = a(i) * s;
    }

    return lhs;
}

/**
 *  @brief Left-multiply a quaternion by a scalar
 *  @param a Quaternion
 *  @param s Scalar
 *  @return Scaled quaterion
 */
template <typename Scalar>
Quat<Scalar> operator*(const Scalar s, const Quat<Scalar> &a)
{
    return operator*(a, s);
}

/**
 *  @brief Multiply a quaternion by a scalar, in place
 *  @param a Quaternion to scale
 *  @param s Scalar
 *  @return a
 */
template <typename Scalar>
Quat<Scalar> &operator*=(Quat<Scalar> &a, const Scalar s)
{
    size_t i = 4u;
    while (i--)
    {
        a(i) *= s;
    }

    return a;
}

/**
 *  @brief Divide a quaternion by a scalar, in place
 *  @param a Quaternion to scale
 *  @param s Scalar
 *  @return a
 */
template <typename Scalar>
Quat<Scalar> &operator/=(Quat<Scalar> &a, const Scalar s)
{
    return operator*=(a, 1 / s);
}

/**
 *  @brief Add two quaternions (element-wise summation)
 *  @param a First quaternion
 *  @param b Second quaternion
 *  @return Sum
 */
template <typename Scalar>
Quat<Scalar> operator+(const Quat<Scalar> &a, const Quat<Scalar> &b)
{
    Quat<Scalar> lhs;

    size_t i = 4u;
    while (i--)
    {
        lhs(i) = a(i) + b(i);
    }

    return lhs;
}

/**
 *  @brief Add-in place quaterion
 *  @param a First quaternion, is modified
 *  @param b Second quaternion
 *  @return Sum
 */
template <typename Scalar>
Quat<Scalar> &operator+=(Quat<Scalar> &a, const Quat<Scalar> &b)
{
    size_t i = 4u;
    while (i--)
    {
        a(i) += b(i);
    }

    return a;
}

/**
 * @brief Creates a rotation which rotates angle degrees around axis.
 */
template <typename Scalar>
Quat<Scalar> angleAxis(units::angle::radian_t const angle, Vector<Scalar, 3u> const& v)
{
    Scalar const s = std::sin(angle.to<Scalar>() * static_cast<Scalar>(0.5));
    return { std::cos(angle.to<Scalar>() * static_cast<Scalar>(0.5)),
             v.x * s, v.y * s, v.z * s };
}

#endif // QUATERNION_HPP
