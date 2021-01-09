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

#ifndef QUATERNION_HPP
#define QUATERNION_HPP

#include <cmath>
#include <limits>
#include <type_traits>

/**
 *  @class Quat
 *  @brief Template representation of a quaternion.
 *  @note Supports multiplication, addition, scaling and integration.
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
        m_q[0] = 1;
        m_q[1] = m_q[2] = m_q[3] = 0;
    }

    /**
     *  @brief Construct a quaterion
     *  @param a Scalar parameter
     *  @param b,c,d Complex parameters
     */
    Quat(Scalar a, Scalar b, Scalar c, Scalar d)
    {
        m_q[0] = a;
        m_q[1] = b;
        m_q[2] = c;
        m_q[3] = d;
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
     *  @param Matrix Any matrix-like type that supports the (i,j) operator.
     *  @note The result is not correct if this quaternion is not a member of S(4)
     *  @return 4x4 Rotation matrix
     */
    Matrix<Scalar, 4_z, 4_z> toMatrix() const
    {
        Matrix<Scalar, 4_z, 4_z> R(0); // FIXME optim: avoid (0)

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
    static Quat rotation(Scalar theta, Scalar x, Scalar y, Scalar z)
    {
        const Scalar haversine = std::sin(theta / 2);
        const Scalar havercosine = std::cos(theta / 2);

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

    /*
     *  Accessors
     */

    /**
     * @brief operator () Element-wise accessor
     * @param i Index into quaternion, must be less than 4.
     * @return Element i of the quaternion.
     */
    Scalar &operator()(unsigned int i) { return m_q[i]; }
    const Scalar &operator()(unsigned int i) const { return m_q[i]; }

    Scalar &a() { return m_q[0]; } /**< Scalar component */
    const Scalar &a() const { return m_q[0]; }

    Scalar &b() { return m_q[1]; } /**< First complex dimension (i) */
    const Scalar &b() const { return m_q[1]; }

    Scalar &c() { return m_q[2]; } /**< Second complex dimension (j) */
    const Scalar &c() const { return m_q[2]; }

    Scalar &d() { return m_q[3]; } /**< Third complex dimension (k) */
    const Scalar &d() const { return m_q[3]; }

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

    for (int i = 0; i < 4; i++)
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
    for (int i = 0; i < 4; i++)
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

    for (int i = 0; i < 4; i++)
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
    for (int i = 0; i < 4; i++)
    {
        a(i) += b(i);
    }

    return a;
}

template <typename Scalar>
Quat<Scalar> angleAxis(Scalar const angle, Vector<Scalar, 3u> const& v)
{
    Scalar const s = std::sin(angle * static_cast<Scalar>(0.5));
    return { std::cos(angle * static_cast<Scalar>(0.5)),
             v.x * s, v.y * s, v.z * s };
}

#endif // QUATERNION_HPP
