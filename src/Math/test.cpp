#include "Math/Transformable.hpp"
#include <iostream>

int main()
{
    Transformable3D tr;

    std::cout << "Up: " << tr.up() << std::endl;
    std::cout << "Right: " << tr.right() << std::endl;
    std::cout << "Forward: " << tr.forward() << std::endl;
    std::cout << "Direction: " << tr.direction() << std::endl;

    std::cout << "Rotation: " << tr.rotation() << std::endl;
    tr.rotate(maths::toRadian(45.0f), Vector3f(1,0,0));
    std::cout << "Rotation: " << tr.rotation() << std::endl;
    tr.position(Vector3f(1,2,3));
    tr.roll(maths::toRadian(45.0f));
    std::cout << "Transform: " << tr.matrix() << std::endl;

    return 0;
}
