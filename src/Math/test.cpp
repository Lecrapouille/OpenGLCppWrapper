#include "Math/Transformable.hpp"
#include <iostream>

using namespace units::angle;

int main()
{
    Matrix44f const I(matrix::Identity);
    Vector4f x(1,2,3,1); // Initial position
    Vector4f R; // Final position

#if 0
    // -------------------------------------------------------------------------
    // Rotation order: translate a point (1,2,3) through a transform matrix
    // translation by (1,2,3).
    {
        // Translation matrix
        Matrix44f M({1, 0, 0, 1,   0, 1, 0, 2,   0, 0, 1, 3,   0, 0, 0, 1});

        // Correct formula (Scilab code):
        // [1 0 0 1; 0 1 0 2; 0 0 1 3; 0 0 0 1] * [1 2 3 1]'
        // Where ' is the transpose operation.
        std::cout << "M . x = " << M * x
                  << std::endl << std::endl;

        // Incorrect formula (Scilab code):
        // [1 2 3 1] * [1 0 0 0; 0 1 0 0; 0 0 1 0; 1 2 3 1]
        std::cout << "x . M = " << x * M
                  << std::endl << std::endl;
    }

    // -------------------------------------------------------------------------
    // But because OpenGL needs transposed matrix matrices operations are
    // inverted: transpose(A.B) == transpose(B) * transpose(A)
    {
        Matrix44f M = matrix::translate(I, Vector3f(1,2,3));
        R = x * M;
        std::cout << "Translation: "
                  << x << " * " << M << " = " << R
                  << std::endl << std::endl;
    }

    // -------------------------------------------------------------------------
    {
        Matrix44f M = matrix::scale(I, Vector3f(1,2,3));
        R = x * M;
        std::cout << "Scale: "
                  << x << " * " << M << " = " << R
                  << std::endl << std::endl;
    }

    // -------------------------------------------------------------------------
    {
        x = Vector3f(1,0,0);
        degree_t angle(90.0f);
        Matrix44f M = matrix::rotate(I, angle, Vector3f::UNIT_Y);
        R = x * M;
        std::cout << "Rotate: "
                  << x << " * " << M << " = " << R
                  << std::endl << std::endl;
        R = R * M;
        R = R * M;
        R = R * M;
        std::cout << "3 Rotates: "
                  << x << " * " << M << " = " << R
                  << std::endl << std::endl;
    }

    // -------------------------------------------------------------------------
    {
        Transformable3D M;
        std::cout << "Identity transform: " << M.matrix()
                  << std::endl << std::endl;
    }

    // -------------------------------------------------------------------------
    {
        Transformable3D M;

        std::cout << "Up: " << M.up() << std::endl;
        std::cout << "Right: " << M.right() << std::endl;
        std::cout << "Forward: " << M.forward() << std::endl;
        std::cout << "Direction: " << M.direction() << std::endl;

        std::cout << "Rotation: " << M.rotation() << std::endl;
        M.rotate(degree_t(45.0f), Vector3f(1,0,0));
        std::cout << "Rotation: " << M.rotation() << std::endl;
        M.position(Vector3f(1,2,3));
        M.roll(degree_t(45.0f));
        std::cout << "Transform: " << M.matrix()
                  << std::endl << std::endl;
    }
#endif

    // -------------------------------------------------------------------------
    {
        Transformable3D M;

        M.position(Vector3f(10,0,0));
        std::cout << "Position " << M.position() << std::endl;

        M.origin(Vector3f(1,0,0));
        std::cout << "Position " << M.position() << std::endl;
        std::cout << "Local Position " << M.local_position() << std::endl;

        M.local_position(Vector3f(10,0,0));
        std::cout << "Position " << M.position() << std::endl;
        std::cout << "Local Position " << M.local_position() << std::endl;

        std::cout << "-----" << std::endl;
        M.translate(Vector3f(10,0,0), Space::World);
        std::cout << "Position " << M.position() << std::endl;
        std::cout << "Local Position " << M.local_position() << std::endl;

        std::cout << "-----" << std::endl;
        M.translate(Vector3f(10,0,0), Space::Self);
        std::cout << "Position " << M.position() << std::endl;
        std::cout << "Local Position " << M.local_position() << std::endl;

        std::cout << "-----" << std::endl;
        M.position(Vector3f(10,0,0));
        M.yaw(degree_t(45.0f));
        std::cout << "Quat " << M.attitude() << std::endl;
        std::cout << "Position " << M.position() << std::endl;
        std::cout << "Local Position " << M.local_position() << std::endl;
        M.translate(Vector3f(10,0,0), Space::Self);
        std::cout << "Position " << M.position() << std::endl;
        std::cout << "Local Position " << M.local_position() << std::endl;

        std::cout << "=====" << std::endl;
        M.position(Vector3f(10,0,0));
        M.yaw(degree_t(45.0f), Space::World);
        std::cout << "Quat " << M.attitude() << std::endl;
    }

    return 0;
}
