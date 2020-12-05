#ifndef SHAPE_HPP
#  define SHAPE_HPP

#  include "Material/Geometry.hpp"
#  include "Material/Material.hpp"

template<class Geometry, class Material>
class Shape
{
public:

    Shape(std::string const& name)
        : m_name(name),
          vao(name)
    {}

    bool compile()
    {
        if (!material.compile())
            return false;

        geometry.generate(vao.vector3f("position"),
                          vao.vector3f("normals"),
                          vao.vector2f("uv"),
                          vao.index());

        material.program.bind(vao);
        return true;
    }

    void draw()
    {
        material.program.draw(vao, Mode::TRIANGLES, 0u, geometry.vertices().size());
    }

    std::string const& name() { return m_name; }

public:

    std::string m_name;
    Material material;
    Geometry geometry;

private:

    GLVAO32 vao;
};

#endif
