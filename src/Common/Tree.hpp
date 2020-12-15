//=====================================================================
// OpenGLCppWrapper: A C++11 OpenGL 'Core' wrapper.
// Copyright 2018-2019 Quentin Quadrat <lecrapouille@gmail.com>
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

#ifndef GENERAL_TREE_HPP
#  define GENERAL_TREE_HPP

#  include <vector>
#  include <memory>
#  include <iostream>

//------------------------------------------------------------------------------
//! \brief
//------------------------------------------------------------------------------
template<class T>
class Tree
{
public:

    struct Node
    {
        template<typename X = T>
        Node(X&& ele, Node* root = nullptr)
            : parent(root),
              data{std::forward<X>(ele)}
        {}

        template<typename X = T>
        void attach(X&& x, Node* root = nullptr)
        {
            children.push_back(std::make_unique<Node>(std::forward<X>(x), root));
        }

        std::vector<std::unique_ptr<Node>> children;
        Node* parent = nullptr;
        T data;
    };

private:

    std::unique_ptr<Node> root = nullptr;
    std::size_t sz = 0u;

public:

    Tree() = default;

    Tree(const Tree& other)
        : root{clone(other.root)}
    {}

    Tree& operator=(const Tree& other)
    {
        // copy and swap idiom
        Tree tmp(other);
        std::swap(root, tmp.root);
        std::swap(sz, tmp.sz);
        return *this;
    }

    Tree(Tree&& other) noexcept
        : Tree()
    {
        std::swap(root, other.root);
        std::swap(sz, other.sz);
    }

    Tree& operator=(Tree&& other) noexcept
    {
        std::swap(root, other.root);
        std::swap(sz, other.sz);
        return *this;
    }

    inline void clear() noexcept
    {
        root.reset(nullptr);
    }

    inline bool empty() const noexcept
    {
        return sz == 0;
    }

    inline const std::size_t& size() const noexcept
    {
        return sz;
    }

    void print() const// noexcept
    {
        if (root == nullptr)
            return ;

        const_traverse(root, [](auto& node)
        {
            std::cout << "Node: " << node.data;
            std::cout << " has " << node.children.size()
                      << " children:";
            for (auto const& it: node.children)
                std::cout << " " <<  it->data;
            std::cout << std::endl;
        });
    }

    template<typename Functor>
    void traverse(Functor functor)
    {
        traverse(root, functor);
    }

    template<typename Functor>
    void const_traverse(Functor functor) const// noexcept
    {
        traverse(root, functor);
    }

    bool search(const T& x) const noexcept
    {
        return search(x, root);
    }

    template<typename X = T>
    Node& attach(X&& x)
    {
        ++sz;
        if (root == nullptr)
        {
            root = std::make_unique<Node>(std::forward<X>(x), nullptr);
            return *root;
        }
        else
        {
            root->attach(std::forward<X>(x), root.get());
            return *(root->children.back());
        }
    }

private:

    template<typename Functor>
    void traverse(std::unique_ptr<Node>& t, Functor functor)
    {
        functor(*t);

        for (auto& child: t->children)
        {
            traverse(child, functor);
        }
    }

    template<typename Functor>
    void const_traverse(std::unique_ptr<Node> const& t, Functor functor) const
    {
        functor(*t);

        for (auto& child: t->children)
        {
            const_traverse(child, functor);
        }
    }
};

#endif
