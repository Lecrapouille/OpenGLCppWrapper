/*
 * Quaternion.hpp
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

// *****************************************************************************
// Modified code by Quentin Quadrat <lecrapouille@gmail.com>.
// See original code https://github.com/gareth-cross/quat/blob/master/include/quaternion.hpp */
// *****************************************************************************

#ifndef QUATERNION_HPP
#  define QUATERNION_HPP

#  include <cmath>
#  include <limits>
#  include <type_traits>

#  pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Wold-style-cast"
#    pragma GCC diagnostic ignored "-Wfloat-equal"
#    pragma GCC diagnostic ignored "-Wsign-conversion"
#      include "units/units.hpp"
#  pragma GCC diagnostic pop

// *****************************************************************************
//! \class Quat
//! \brief Template representation of a quaternion (a, (b,c,d))
//!
//! where:
//!   - \c a is the scalar component
//!   - \c b is the first complex dimension (i-axis)
//!   - \c c is the second complex dimension (j-axis)
//!   - \c d is the third complex dimension (k-axis)
//!
//! Supports multiplication, addition, scaling and integration.
// *****************************************************************************
template <typename Scalar> class Quat
{
    static_assert(std::is_fundamental<Scalar>::value &&
                  !std::numeric_limits<Scalar>::is_integer,
                  "Scalar must be a floating point type");

public:

    //--------------------------------------------------------------------------
    //! \brief Create a quaternion with null rotation (1, (0,0,0))
    //--------------------------------------------------------------------------
    Quat()
    {
        data[0] = Scalar(1);
        data[1] = data[2] = data[3] = Scalar(0);
    }

    //--------------------------------------------------------------------------
    //! \brief Construct a quaterion a + b i + c j + d k
    //! \param[in] a Scalar parameter
    //! \param[in] b Complex parameters complex i-axis
    //! \param[in] c Complex parameters complex j-axis
    //! \param[in] d Complex parameters complex k-axis
    //--------------------------------------------------------------------------
    Quat(Scalar a, Scalar b, Scalar c, Scalar d)
    {
        data[0] = a;
        data[1] = b;
        data[2] = c;
        data[3] = d;
    }

    //--------------------------------------------------------------------------
    //! \brief Construct a quaterion a + b i + c j + d k
    //! \param[in] a Scalar parameter
    //! \param[in] bcd Complex parameters (i, j, k)
    //! \note do not confuse with angleAxis() which builds a quaternion from a
    //!   given angle and a given rotation axis.
    //--------------------------------------------------------------------------
    Quat(Scalar a, Vector<Scalar, 3_z> const& bcd)
    {
        data[0] = a;
        data[1] = bcd[0];
        data[2] = bcd[1];
        data[3] = bcd[2];
    }

    //--------------------------------------------------------------------------
    //! \brief Construct an unit quaterion a + b i + c j + d k where the scalar
    //! component \c a is not given but rebuild.
    //! \param[in] b Complex parameters complex i-axis
    //! \param[in] c Complex parameters complex j-axis
    //! \param[in] d Complex parameters complex k-axis
    //! \note the length of (b, c, d) shall be <= 1.
    //--------------------------------------------------------------------------
    Quat(Scalar b, Scalar c, Scalar d)
    {
        Scalar n = (b * b) + (c * c) + (d * d);
        assert(n <= Scalar(1));

        data[0] = -std::sqrt(Scalar(1) - n);
        data[1] = b;
        data[2] = c;
        data[3] = d;
    }

    //--------------------------------------------------------------------------
    //! \brief Copy constructor
    //--------------------------------------------------------------------------
    Quat(Quat const& other)
    {
        *this = other;
    }

    //--------------------------------------------------------------------------
    //! \brief Construct a quaternion from a rotation matrix
    //--------------------------------------------------------------------------
    Quat(Matrix<Scalar, 4_z, 4_z> const& m)
    {
        *this = Quat::fromMatrix(m);
    }

    //--------------------------------------------------------------------------
    //! \brief Copy operator
    //--------------------------------------------------------------------------
    Quat& operator=(Quat const& other)
    {
        if (this != &other)
        {
            data[0] = other.data[0];
            data[1] = other.data[1];
            data[2] = other.data[2];
            data[3] = other.data[3];
        }

        return *this;
    }

    //--------------------------------------------------------------------------
    //! \brief Creates a rotation which rotates angle degrees around axis.
    //--------------------------------------------------------------------------
    static Quat<Scalar> fromAngleAxis(units::angle::radian_t const angle, Vector<Scalar, 3u> const& v)
    {
        Scalar const s = std::sin(angle.to<Scalar>() * static_cast<Scalar>(0.5));
        Quat<Scalar> q(std::cos(angle.to<Scalar>() * static_cast<Scalar>(0.5)),
                       v.x * s, v.y * s, v.z * s);
        q.normalize();
        return q;
    }

    //--------------------------------------------------------------------------
    //! \brief Construct a quaternion from an angle/axis
    //--------------------------------------------------------------------------
    Quat(units::angle::radian_t const angle, Vector<Scalar, 3u> const& axis)
    {
        *this = fromAngleAxis(angle, axis);
    }

    //--------------------------------------------------------------------------
    //! \brief Print quaternion
    //--------------------------------------------------------------------------
    inline friend std::ostream& operator<<(std::ostream& os, Quat& q)
    {
        return os << "Quat(" << q.a() << ",  (" << q.b() << ", "
                  << q.c() << ", " << q.d() << "))";
    }

    //--------------------------------------------------------------------------
    //! \brief Return the angle of rotation in radian.
    //--------------------------------------------------------------------------
    units::angle::radian_t angle()
    {
        return units::angle::radian_t(std::acos(a()) * Scalar(2));
    }

    //--------------------------------------------------------------------------
    //! \brief Return the rotation axis (x,y,z).
    //--------------------------------------------------------------------------
    Vector<Scalar, 3_z> axis()
    {
        Scalar tmp1 = static_cast<Scalar>(1) - a() * a();
        if(tmp1 <= static_cast<Scalar>(0))
            return Vector<Scalar, 3_z>(0, 0, 1);
        Scalar tmp2 = static_cast<Scalar>(1) / std::sqrt(tmp1);
        return Vector<Scalar, 3_z>(b() * tmp2, c() * tmp2, d() * tmp2);
    }

    //--------------------------------------------------------------------------
    //! \brief Return the L2 norm of the quaternion.
    //--------------------------------------------------------------------------
    Scalar norm() const
    {
        return std::sqrt(a() * a() + b() * b() + c() * c() + d() * d());
    }

    //--------------------------------------------------------------------------
    //! \brief Normalize quaternion.
    //--------------------------------------------------------------------------
    void normalize()
    {
        const Scalar n = norm();
        operator*=(*this, 1 / n);
    }

    //--------------------------------------------------------------------------
    //! \brief Complex conjugate quaternion.
    //--------------------------------------------------------------------------
    Quat conjugate() const
    {
        return Quat(a(), -b(), -c(), -d());
    }

    Quat operator-() const
    {
        return {
            -data[0], -data[1], -data[2], -data[3]
        };
    }

    //--------------------------------------------------------------------------
    //! \brief Rotate a vector using this quaternion.
    //! \param[in] v Vector stored in the three complex terms
    //--------------------------------------------------------------------------
    Quat transform(const Quat &v) const
    {
        const Quat &q = *this;
        return q * v * q.conjugate();
    }

    //--------------------------------------------------------------------------
    //! \brief Convert a rotation quaternion to its matrix form.
    //! \note The result is not correct if this quaternion is not a member of
    //!   S(4).
    //! \return 4x4 Rotation matrix.
    //--------------------------------------------------------------------------
    Matrix<Scalar, 4_z, 4_z> toMatrix() const
    {
        // glm code
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

#if 0 // FIXME original code
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

    //--------------------------------------------------------------------------
    //! \brief Integrate a rotation quaternion using 4th order Runge Kutta.
    //! \param[in] w Angular velocity (body frame), stored in 3 complex terms.
    //! \param[in] dt Time interval in seconds.
    //! \param[in] normalize If true, quaternion is normalized after integration.
    //--------------------------------------------------------------------------
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

    //--------------------------------------------------------------------------
    //! \brief Integrate a rotation quaterion using Euler integration.
    //! \param[in] w Angular velocity (body frame), stored in 3 complex terms.
    //! \param[in] dt Time interval in seconds.
    //! \param[in] normalize If True, quaternion is normalized after integration.
    //--------------------------------------------------------------------------
    void integrateEuler(const Quat &w, Scalar dt, bool normalize = true)
    {
        Quat &q = *this;
        q += (q * w * static_cast<Scalar>(0.5)) * dt;

        if (normalize)
        {
            q.normalize();
        }
    }

    //--------------------------------------------------------------------------
    //! \brief Create a rotation quaterion
    //! \param[in] theta Angle of rotation (unit: radian).
    //! \param[in] x X component of rotation vector.
    //! \param[in] y Y component of rotation vector.
    //! \param[in] z Z component of rotation vector.
    //--------------------------------------------------------------------------
    static Quat rotation(units::angle::radian_t theta, Scalar x, Scalar y, Scalar z)
    {
        const Scalar haversine = std::sin(theta.to<Scalar>() / 2);
        const Scalar havercosine = std::cos(theta.to<Scalar>() / 2);

        return Quat(havercosine, haversine * x, haversine * y, haversine * z);
    }

    //--------------------------------------------------------------------------
    //! \brief Create a rotation quaternion from rotation vector [x,y,z].
    //! \note If x/y/z have 0 norm, this function returns the identity transform.
    //--------------------------------------------------------------------------
    static Quat rotation(Scalar x, Scalar y, Scalar z)
    {
        const Scalar theta = std::sqrt(x * x + y * y + z * z);

        if (theta < std::numeric_limits<Scalar>::epsilon() * 10)
        {
            return Quat();
        }

        return rotation(theta, x / theta, y / theta, z / theta);
    }

    //--------------------------------------------------------------------------
    //! \brief Create quaternion from matrix.
    //! \param[in] m Rotation matrix, should be a member of SO[3].
    //! \param[in] Matrix Any matrix-like type which supports the (i,j) operator.
    //! \note All singularities are handled, provided \c m belongs to SO[3].
    //! \see http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/index.htm
    //--------------------------------------------------------------------------
    static Quat fromMatrix(Matrix<Scalar, 4_z, 4_z> const& m)
    {
        Quat Q;
        const Scalar trace = m(0, 0) + m(1, 1) + m(2, 2);
        Scalar s = 0;

        if (trace > 0)
        {
            s = 2 * std::sqrt(1 + trace);

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

    //--------------------------------------------------------------------------
    //! \brief Pointer accessor for direct copying
    //-------------------------------------------------------------------------
    inline Scalar* ptr() { return data; }
    inline const Scalar* ptr() const { return data; }

    //--------------------------------------------------------------------------
    //! \brief operator () Element-wise accessor.
    //! \param[in] i Index into quaternion, must be less than 4 (not checked).
    //! \return Element i of the quaternion.
    //--------------------------------------------------------------------------
    Scalar &operator[](size_t const i) { assert(i < 4u); return data[i]; }
    const Scalar &operator[](size_t const i) const { assert(i < 4u); return data[i]; }

    //--------------------------------------------------------------------------
    //! \brief Scalar component
    //--------------------------------------------------------------------------
    Scalar &a() { return data[0]; }
    const Scalar &a() const { return data[0]; }

    //--------------------------------------------------------------------------
    //! \brief First complex dimension (i)
    //--------------------------------------------------------------------------
    Scalar &b() { return data[1]; }
    const Scalar &b() const { return data[1]; }

    //--------------------------------------------------------------------------
    //! \brief Second complex dimension (j)
    //--------------------------------------------------------------------------
    Scalar &c() { return data[2]; }
    const Scalar &c() const { return data[2]; }

    //--------------------------------------------------------------------------
    //! \brief Third complex dimension (k)
    //--------------------------------------------------------------------------
    Scalar &d() { return data[3]; }
    const Scalar &d() const { return data[3]; }

    //--------------------------------------------------------------------------
    //! \brief Scalar component
    //--------------------------------------------------------------------------
    Scalar &w() { return data[0]; }
    const Scalar &w() const { return data[0]; }

    //--------------------------------------------------------------------------
    //! \brief First complex dimension (i)
    //--------------------------------------------------------------------------
    Scalar &x() { return data[1]; }
    const Scalar &x() const { return data[1]; }

    //--------------------------------------------------------------------------
    //! \brief Second complex dimension (j)
    //--------------------------------------------------------------------------
    Scalar &y() { return data[2]; }
    const Scalar &y() const { return data[2]; }

    //--------------------------------------------------------------------------
    //! \brief Third complex dimension (k)
    //--------------------------------------------------------------------------
    Scalar &z() { return data[3]; }
    const Scalar &z() const { return data[3]; }

private:

    Scalar data[4];
};

//------------------------------------------------------------------------------
// \brief Typedefs for common float64 quaternion.
//------------------------------------------------------------------------------
typedef Quat<double> Quatd;

//------------------------------------------------------------------------------
// \brief Typedefs for common float32 quaternion.
//------------------------------------------------------------------------------
typedef Quat<float> Quatf;

//------------------------------------------------------------------------------
//! \brief Multiply two quaternions.
//! \param[in] a Left quaternion
//! \param[in] b Right quaternion
//! \return Product of both quaternions
//------------------------------------------------------------------------------
template <typename Scalar>
Quat<Scalar> operator*(const Quat<Scalar> &a, const Quat<Scalar> &b)
{
    Quat<Scalar> lhs;

    lhs[0] = a[0] * b[0] - a[1] * b[1] - a[2] * b[2] - a[3] * b[3];
    lhs[1] = a[0] * b[1] + a[1] * b[0] + a[2] * b[3] - a[3] * b[2];
    lhs[2] = a[0] * b[2] - a[1] * b[3] + a[2] * b[0] + a[3] * b[1];
    lhs[3] = a[0] * b[3] + a[1] * b[2] - a[2] * b[1] + a[3] * b[0];

    return lhs;
}

//------------------------------------------------------------------------------
//! \brief Rotation of a vector by a quaternion
//------------------------------------------------------------------------------
template <typename Scalar>
Vector3f operator*(const Quat<Scalar> &q, const Vector3f& v)
{
    Vector3f uv, uuv;
    Vector3f qvec(q.b(), q.c(), q.d());

    uv = qvec % v; // Cross product
    uuv = qvec % uv;
    uv *= (2.0f * q.a());
    uuv *= 2.0f;

    return v + uv + uuv;
}

//------------------------------------------------------------------------------
//! \brief Divide a quaternion by a scalar.
//! \param[in] a Quaternion.
//! \param[in] s Scalar.
//! \return Scaled quaternion.
//------------------------------------------------------------------------------
template <typename Scalar>
Quat<Scalar> operator/(const Quat<Scalar> &q, const Scalar s)
{
    return operator*(q, Scalar(1) / s);
}

//------------------------------------------------------------------------------
//! \brief Right-multiply a quaternion by a scalar
//! \param[in] a Quaternion
//! \param[in] s Scalar
//! \return Scaled quaterion
//------------------------------------------------------------------------------
template <typename Scalar>
Quat<Scalar> operator*(const Quat<Scalar> &q, const Scalar s)
{
    return {
        q.data[0] * s,
        q.data[1] * s,
        q.data[2] * s,
        q.data[3] * s
    };
}

//------------------------------------------------------------------------------
//! \brief Left-multiply a quaternion by a scalar
//! \param[in] a Quaternion
//! \param[in] s Scalar
//! \return Scaled quaterion
//------------------------------------------------------------------------------
template <typename Scalar>
Quat<Scalar> operator*(const Scalar s, const Quat<Scalar> &q)
{
    return operator*(q, s);
}

//------------------------------------------------------------------------------
//! \brief Multiply a quaternion by a scalar, in place
//! \param[in] a Quaternion to scale
//! \param[in] s Scalar
//! \return a
//------------------------------------------------------------------------------
template <typename Scalar>
Quat<Scalar> &operator*=(Quat<Scalar> &q, const Scalar s)
{
    q.a() *= s;
    q.b() *= s;
    q.c() *= s;
    q.d() *= s;

    return q;
}

//------------------------------------------------------------------------------
//! \brief Divide a quaternion by a scalar, in place
//! \param[in] a Quaternion to scale
//! \param[in] s Scalar
//! \return a
//------------------------------------------------------------------------------
template <typename Scalar>
Quat<Scalar> &operator/=(Quat<Scalar> &q, const Scalar s)
{
    return operator*=(q, Scalar(1) / s);
}

//------------------------------------------------------------------------------
//! \brief Add two quaternions (element-wise summation)
//! \param[in] a First quaternion
//! \param[in] b Second quaternion
//! \return Sum
//------------------------------------------------------------------------------
template <typename Scalar>
Quat<Scalar> operator+(const Quat<Scalar> &a, const Quat<Scalar> &b)
{
    return {
        a.a() + b.a(),
        a.b() + b.b(),
        a.c() + b.c(),
        a.d() + b.d()
    };
}

//------------------------------------------------------------------------------
//! \brief Add-in place quaterion
//! \param[in] a First quaternion, is modified
//! \param[in] b Second quaternion
//! \return Sum
//------------------------------------------------------------------------------
template <typename Scalar>
Quat<Scalar> &operator+=(Quat<Scalar> &a, const Quat<Scalar> &b)
{
    a.a() += b.a();
    a.b() += b.b();
    a.c() += b.c();
    a.d() += b.d();

    return a;
}

#endif // QUATERNION_HPP
