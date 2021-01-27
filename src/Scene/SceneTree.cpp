//=====================================================================
// OpenGLCppWrapper: A C++11 OpenGL 'Core' wrapper.
// Copyright 2018-2021 Quentin Quadrat <lecrapouille@gmail.com>
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

#include "Scene/SceneTree.hpp"
#include <iostream>
#include <sstream>

//------------------------------------------------------------------------------
SceneTree::~SceneTree()
{
    release();
}

//------------------------------------------------------------------------------
SceneObject* SceneTree::get(std::string const& path)
{
    SceneObject* found = nullptr;

    if (root == nullptr)
        return nullptr;

    if (path[0] == '/')
    {
        std::stringstream sspath(path);
        std::string name;
        found = root.get();

        while (std::getline(sspath, name, '/'))
        {
            if (name == "")
                continue;

            found = findChild(found, name);
            if (found == nullptr)
                return nullptr;
        }

        return found;
    }
    else
    {
        root->traverse([](SceneObject* node, std::string const& name, SceneObject* found_)
        {
            if (found_ == nullptr)
            {
                if (node->name() == name)
                {
                    found_ = node;
                }
            }
        }, path, found);
    }

    return found;
}

//------------------------------------------------------------------------------
SceneObject* SceneTree::findChild(SceneObject* node, std::string const& name)
{
    for (auto& it: node->children)
    {
        if (it->name() == name)
            return it.get();
    }
    return nullptr;
}

//------------------------------------------------------------------------------
void SceneTree::getByTag(std::string const& tag, std::vector<Node*> found)
{
    if (root == nullptr)
        return ;

    root->traverse([](SceneObject* node, std::string const& tag_, std::vector<Node*> found_)
    {
        if (node->tag == tag_)
        {
            found_.push_back(node);
        }
    }, tag, found);
}

//------------------------------------------------------------------------------
void SceneTree::debug()
{
    if (root == nullptr)
        return ;

    root->traverse([](SceneObject* node)
    {
        std::cout << *node << std::endl;
    });
}

//------------------------------------------------------------------------------
void SceneTree::setup()
{
    if (root == nullptr)
        return ;

    root->traverse([](SceneObject* node)
    {
        if (!node->enabled())
            return ;

        node->onSetup();
    });
}

//------------------------------------------------------------------------------
void SceneTree::update(float const dt)
{
    if (root == nullptr)
        return ;

    root->traverse([dt](SceneObject* node)
    {
        if (!node->enabled())
            return ;

        // Derived class may override this function for animating nodes.
        node->onUpdate(dt);

        // Update the matrix transform from the parent matrix.
        node->m_world_transform = node->transform.matrix();
        if (node->parent != nullptr)
            node->m_world_transform *= node->parent->m_world_transform;
    });
}

//------------------------------------------------------------------------------
void SceneTree::draw()
{
    if (root == nullptr)
        return ;

    root->traverse([](SceneObject* node)
    {
        if (!node->enabled())
            return ;

        // TODO: this could be better to create an node like OpenInventor
        // separator instead of this computation made everytime (even if
        // scaling a node it will also scale descendants)? Sometimes you
        // just want to scale the node not its descendants.
        node->onDraw(matrix::scale(node->m_world_transform,
                                   node->transform.localScale()));
    });
}

//------------------------------------------------------------------------------
void SceneTree::release()
{
    if (root == nullptr)
        return ;

    root->traverse([](SceneObject* node)
    {
        if (!node->enabled())
            return ;

        node->onDisable();
    });

    root->clear();
    root = nullptr;
}
