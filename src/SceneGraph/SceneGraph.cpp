//=====================================================================
// OpenGLCppWrapper: A C++11 OpenGL 'Core' wrapper.
// Copyright 2018-2020 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of OpenGLCppWrapper.
//
// OpenGLCppWrapper is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// OpenGLCppWrapper is distributedin the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with OpenGLCppWrapper.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#include "SceneGraph.hpp"
#include <iostream>

SceneGraph::~SceneGraph()
{
    release();
}

void SceneGraph::debug()
{
    if (root == nullptr)
        return ;

    root->traverse([](SceneGraph::Node& node)
    {
        std::cout << "Node: " << node.name()
                  << std::endl
                  << " enabled: " << node.enabled()
                  << std::endl
                  << " has " << node.children.size()
                  << " children:" << std::endl;
        for (auto const& it: node.children)
            std::cout << " " <<  it->name();
        std::cout << std::endl;
    });
}

void SceneGraph::setup()
{
    if (root == nullptr)
        return ;

    root->traverse([](SceneGraph::Node& node)
    {
        if (!node.enabled())
            return ;

        node.onSetup();
    });
}

void SceneGraph::update(float const dt)
{
    if (root == nullptr)
        return ;
std::cout << "Update ici" << std::endl;
    root->traverse([dt](SceneGraph::Node& node)
    {std::cout << "Update la" << std::endl;
        if (!node.enabled()) {std::cout << "merde" << std::endl;
            return ;
        }
        std::cout << "Update '" << node.name() << "'" << std::endl;
        // Derived class may override this function for animating nodes.
        node.onUpdate(dt);

        // Update the matrix transform from the parent matrix.
        node.m_world_transform = node.transform.matrix();
        if (node.parent != nullptr)
            node.m_world_transform *= node.parent->m_world_transform;
    });
}

void SceneGraph::draw()
{
    if (root == nullptr)
        return ;

    root->traverse([](SceneGraph::Node& node)
    {
        if ((!node.enabled()) || (node.renderable == nullptr))
            return ;
        std::cout << "Draw " << node.name() << std::endl;
        // TODO: this could be better to create an node like OpenInventor
        // separator instead of this computation made everytime (even if
        // scaling a node it will also scale descendants)? Sometimes you
        // just want to scale the node not its descendants.
        node.renderable->draw(matrix::scale(node.m_world_transform,
                                            node.transform.localScale()));
    });
}

void SceneGraph::release()
{
    if (root == nullptr)
        return ;

    root->traverse([](SceneGraph::Node& node)
    {
        if (!node.enabled())
            return ;

        node.onDisable();
    });

    root->clear();
    root = nullptr;
}
