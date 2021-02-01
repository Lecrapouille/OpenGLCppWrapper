#  pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Wold-style-cast"
#    pragma GCC diagnostic ignored "-Wfloat-equal"
#    pragma GCC diagnostic ignored "-Wsign-conversion"
#    pragma GCC diagnostic ignored "-Wconversion"
#      include <glm/glm.hpp>
#      include <glm/ext.hpp>
#      include <glm/gtx/string_cast.hpp>
#  pragma GCC diagnostic pop

#include "Transformation.hpp"
#include "Quaternion.hpp"

void glm_lookAt(glm::vec3 const &position, glm::vec3 const &target, glm::vec3 const &up)
{
    std::cout << "GLM:" << std::endl;
    glm::mat4 mat = glm::lookAt(position, target, up);
    std::cout << "  glm::lookAt: " << glm::to_string(mat) << std::endl;

    glm::quat orientation = glm::conjugate(glm::quat_cast(mat));
    std::cout << "  glm::quat conjug: " << glm::to_string(orientation) << std::endl;
    std::cout << "  =>: " << glm::angle(orientation) * 57.2958f << " "
              << glm::to_string(glm::axis(orientation)) << std::endl;

    // Camera::view
    //std::cout << "  POS " << glm::to_string(position) << std::endl;
    glm::mat4 trans = glm::translate(glm::mat4(1.0f), -position);
    std::cout << "  glm::trans " << glm::to_string(trans) << std::endl;
    glm::mat4 rot = glm::mat4_cast(orientation);
    std::cout << "  glm::rot " << glm::to_string(rot) << std::endl;
    glm::mat4 view = rot * trans;
    std::cout << "  glm::view " << glm::to_string(view) << std::endl;

    {
        glm::mat4 tra({1, 0, 0, 0,
                       0, 1, 0, 0,
                       0, 0, 1, 0,
                       -1, 0, 0, 1});
        glm::mat4 rot({0, 0, -1, 0,
                       0, 1, 0, 0,
                       1, 0, 0, 0,
                       0, 0, 0, 1});

        std::cout << glm::to_string(rot * tra) << std::endl;
    }
}

void qq_lookAt(Vector3f const &position, Vector3f const &target, Vector3f const &up)
{
    std::cout << "\n\nQQ:" << std::endl;
    Matrix44f mat = matrix::lookAt(position, target, up);
    std::cout << "  qq::LookAt: " << mat << std::endl;

    Quatf orientation = Quatf::fromMatrix(mat).conjugate();
    std::cout << "  qq::quat: " << orientation << std::endl;
    std::cout << "  =>: " << orientation.angle() * 57.2958f << " angle: "
              << orientation.axis() << std::endl;

    // Camera::view
    Matrix44f I(matrix::Identity);
    Matrix44f trans = matrix::translate(I, -position);
    std::cout << "  qq::trans " << trans << std::endl;

    Matrix44f rot = orientation.toMatrix();
    std::cout << "  qq::rot " << rot << std::endl;

    Matrix44f view = trans * rot; // FIXME shall be rot * trans
    std::cout << "  qq::view " << view << std::endl;
    //std::cout << "  qq::view\n" << operator*(rot, trans) << std::endl;

    {
        Matrix44f tra({1, 0, 0, 0,
                       0, 1, 0, 0,
                       0, 0, 1, 0,
                       -1, 0, 0, 1});
        Matrix44f rot({0, 0, -1, 0,
                       0, 1, 0, 0,
                       1, 0, 0, 0,
                       0, 0, 0, 1});

        std::cout << rot * tra << std::endl;
    }
}


// g++ -W -Wall --std=c++14 -I/home/qq/MyGitHub/OpenGLCppWrapper/external/glm -I/home/qq/MyGitHub/OpenGLCppWrapper/external/units/include/ -I.. test_glm.cpp -o prog
int main()
{
    glm_lookAt(glm::vec3(1,0,0),
               glm::vec3(0,0,0),
               glm::vec3(0,1,0));

    qq_lookAt(Vector3f(1,0,0),
              Vector3f(0,0,0),
              Vector3f(0,1,0));

    return 0;
}
