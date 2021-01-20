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

    inline friend std::ostream& operator<<(std::ostream& os, Quat& q)
    {
        return os << "(" << q.x() << ", " << q.y() << ", "
                  << q.z() << ", " << q.w() << ")";
    }

    Scalar angle()
    {
        return std::acos(w()) * Scalar(2);
    }

    Vector<Scalar, 3_z> axis()
    {
        Scalar tmp1 = static_cast<Scalar>(1) - w() * w();
        if(tmp1 <= static_cast<Scalar>(0))
            return Vector<Scalar, 3_z>(0, 0, 1);
        Scalar tmp2 = static_cast<Scalar>(1) / std::sqrt(tmp1);
        return Vector<Scalar, 3_z>(x() * tmp2, y() * tmp2, z() * tmp2);
    }

    /**
     *  @brief L2 norm of the quaternion
     */
    Scalar norm() const
    {
        return std::sqrt(x() * x() + y() * y() + z() * z() + w() * w());
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
        return Quat(x(), -y(), -z(), -w());
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

        const Scalar aa = x() * x();
        const Scalar bb = y() * y();
        const Scalar cc = z() * z();
        const Scalar dd = w() * w();

        R(0, 0) = aa + bb - cc - dd;
        R(1, 0) = 2 * y() * z() + 2 * x() * w();
        R(2, 0) = 2 * y() * w() - 2 * x() * z();

        R(0, 1) = 2 * y() * z() - 2 * x() * w();
        R(1, 1) = aa - bb + cc - dd;
        R(2, 1) = 2 * z() * w() + 2 * x() * y();

        R(0, 2) = 2 * y() * w() + 2 * x() * z();
        R(1, 2) = 2 * z() * w() - 2 * x() * y();
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

            Q.x() = s / 4;
            Q.y() = (m(2, 1) - m(1, 2)) / s;
            Q.z() = (m(0, 2) - m(2, 0)) / s;
            Q.w() = (m(1, 0) - m(0, 1)) / s;
        }
        else if (m(0, 0) > m(1, 1) && m(0, 0) > m(2, 2))
        {
            s = 2 * std::sqrt(1 + m(0, 0) - m(1, 1) - m(2, 2));

            Q.x() = (m(2, 1) - m(1, 2)) / s;
            Q.y() = s / 4;
            Q.z() = (m(0, 1) + m(1, 0)) / s;
            Q.w() = (m(0, 2) + m(2, 0)) / s;
        }
        else if (m(1, 1) > m(2, 2))
        {
            s = 2 * std::sqrt(1 + m(1, 1) - m(0, 0) - m(2, 2));

            Q.x() = (m(0, 2) - m(2, 0)) / s;
            Q.y() = (m(0, 1) + m(1, 0)) / s;
            Q.z() = s / 4;
            Q.w() = (m(1, 2) + m(2, 1)) / s;
        }
        else
        {
            s = 2 * std::sqrt(1 + m(2, 2) - m(0, 0) - m(1, 1));

            Q.x() = (m(1, 0) - m(0, 1)) / s;
            Q.y() = (m(0, 2) + m(2, 0)) / s;
            Q.z() = (m(1, 2) + m(2, 1)) / s;
            Q.w() = s / 4;
        }

        return Q;
    }

    void generateW()
    {
        Scalar sw = Scalar(1) - (x() * x()) - (y() * y()) - (z() * z());
        if (sw < Scalar(0))
            w() = Scalar(0);
        else
            w() = - std::sqrt(sw);
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

    Scalar &x() { return m_q[0]; } /**< Scalar component */
    const Scalar &x() const { return m_q[0]; }

    Scalar &y() { return m_q[1]; } /**< First complex dimension (i) */
    const Scalar &y() const { return m_q[1]; }

    Scalar &z() { return m_q[2]; } /**< Second complex dimension (j) */
    const Scalar &z() const { return m_q[2]; }

    Scalar &w() { return m_q[3]; } /**< Third complex dimension (k) */
    const Scalar &w() const { return m_q[3]; }

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
Quat<Scalar> angleAxis(Scalar const angle, Vector<Scalar, 3u> const& v)
{
    Scalar const s = std::sin(angle * static_cast<Scalar>(0.5));
    return { std::cos(angle * static_cast<Scalar>(0.5)),
             v.x * s, v.y * s, v.z * s };
}

#endif // QUATERNION_HPP
