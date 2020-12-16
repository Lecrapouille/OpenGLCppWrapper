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
        Node(X&& d, Node* p = nullptr)
            : parent(p),
              data{std::forward<X>(d)}
        {}

        Node& operator[](size_t const i)
        {
            return children[i];
        }

        Node& root()
        {
            Node* n = this;
            while (n->parent != nullptr)
                n = n->parent;
            return *n;
        }

        //! \brief Getter
        inline operator const T&() const
        {
            return data;
        }

        //! \brief Setter
        inline operator T&()
        {
            return data;
        }

        Node* parent = nullptr;
        std::vector<std::unique_ptr<Node>> children;
        T data;
    };

public:

    Tree() = default;

    template<typename X = T>
    Tree(X&& data)
    {
        m_root = std::make_unique<Node>(std::forward<X>(data), nullptr);
        m_size = 1u;
    }

    template<typename X = T>
    Tree(std::initializer_list<X> list)
    {
        for (auto& it: list)
            insert(it);
    }

    Tree(const Tree& other)
        : m_root{clone(other.m_root)}
    {}

    ~Tree()
    {
        clear();
    }

    Tree& operator=(const Tree& other)
    {
        // copy and swap idiom
        Tree tmp(other);
        std::swap(m_root, tmp.m_root);
        std::swap(m_size, tmp.m_size);
        return *this;
    }

    Tree(Tree&& other) noexcept
        : Tree()
    {
        std::swap(m_root, other.m_root);
        std::swap(m_size, other.m_size);
    }

    Tree& operator=(Tree&& other) noexcept
    {
        std::swap(m_root, other.m_root);
        std::swap(m_size, other.m_size);
        return *this;
    }

    void clear()
    {
        if (m_root != nullptr)
            clear(*m_root);
        m_root = nullptr;
    }

    void clear(Node& node)
    {
        Node* current = &node;

        while ((current != &node) || (!node.children.empty()))
        {
            if (!current->children.empty())
            {
                current = current->children.back().get();
            }
            else
            {
                current = current->parent;
                current->children.pop_back();
                --m_size;
            }
        }
        --m_size;
    }

    inline bool empty() const noexcept
    {
        return m_size == 0u;
    }

    inline const std::size_t& size() const noexcept
    {
        return m_size;
    }

    void print() const// noexcept
    {
        if (m_root == nullptr)
            return ;

        traverse(*m_root, [](Node const& node)
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
        if (m_root == nullptr)
            return ;

        traverse(*m_root, functor);
    }

    template<typename Functor>
    void traverse(Functor functor) const// noexcept
    {
        if (m_root == nullptr)
            return ;

        traverse(*m_root, functor);
    }

    template<typename Functor>
    void traverse(Node& node, Functor functor)
    {
        functor(node);

        for (auto& child: node.children)
        {
            traverse(*child, functor);
        }
    }

    template<typename Functor>
    void traverse(Node const& node, Functor functor) const
    {
        functor(node);

        for (auto& child: node.children)
        {
            traverse(*child, functor);
        }
    }

    bool search(const T& x) const noexcept
    {
        if (m_root == nullptr)
            return false;

        return search(x, m_root);
    }

    Node& root()
    {
        return *m_root;
    }

    template<typename X = T>
    Node& root(X&& x)
    {
        m_size = 1u;
        m_root = std::make_unique<Node>(std::forward<X>(x), nullptr);
        return *m_root;
    }

    template<typename X = T>
    Node& insert(X&& x)
    {
        if (m_root == nullptr)
        {
            ++m_size;
            m_root = std::make_unique<Node>(std::forward<X>(x), nullptr);
            return *m_root;
        }
        else
        {
            return insert(*m_root, std::forward<X>(x));
        }
    }

    template<typename X = T>
    Node& insert(Node& parent, X&& x)
    {
        ++m_size;
        parent.children.push_back(std::make_unique<Node>(std::forward<X>(x), &parent));
        return *(m_root->children.back());
    }

    template<typename X = T>
    void insert(Node& parent, std::initializer_list<X> list)
    {
        m_size += list.size();
        for (auto& it: list)
            parent.children.push_back(std::make_unique<Node>(it, &parent));
    }

private:

    std::unique_ptr<Node> m_root = nullptr;
    std::size_t m_size = 0u;
};

#endif
