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

#include "main.hpp"

#define protected public
#define private public
#  include "Common/Tree.hpp"
#undef protected
#undef private

TEST(Tree, Nominal)
{
    Tree<int> tr;
    tr.print();
    tr.traverse([](Tree<int>::Node const& node) { std::cout << node.data << std::endl; });

    ASSERT_EQ(tr.size(), 0u);
    ASSERT_EQ(tr.empty(), true);
    ASSERT_EQ(tr.m_root, nullptr);

    tr.insert(42);
    ASSERT_EQ(tr.size(), 1u);
    ASSERT_EQ(tr.empty(), false);
    ASSERT_NE(tr.m_root, nullptr);
    ASSERT_EQ(tr.m_root->data, 42);
    ASSERT_EQ(tr.m_root->parent, nullptr);
    ASSERT_EQ(tr.m_root->children.size(), 0u);
    ASSERT_EQ(&(tr.m_root->root()), tr.m_root.get());
    ASSERT_EQ(&(tr.root()), tr.m_root.get());

    Tree<int>::Node& n1 = tr.insert(43);
    ASSERT_EQ(tr.size(), 2u);
    ASSERT_EQ(tr.empty(), false);
    ASSERT_NE(tr.m_root, nullptr);
    ASSERT_EQ(tr.m_root->data, 42);
    ASSERT_EQ(tr.m_root->parent, nullptr);
    ASSERT_EQ(tr.m_root->children.size(), 1u);
    ASSERT_EQ(tr.m_root->children[0]->data, 43);
    ASSERT_EQ(tr.m_root->children[0]->children.size(), 0u);
    ASSERT_EQ(tr.m_root->children[0]->parent, tr.m_root.get());
    ASSERT_EQ(tr.m_root->children[0].get(), &n1);
    ASSERT_EQ(&(n1.root()), tr.m_root.get());

    Tree<int>::Node& n2 = tr.insert(44);
    ASSERT_EQ(tr.size(), 3u);
    ASSERT_EQ(tr.empty(), false);
    ASSERT_NE(tr.m_root, nullptr);
    ASSERT_EQ(tr.m_root->data, 42);
    ASSERT_EQ(tr.m_root->parent, nullptr);
    ASSERT_EQ(tr.m_root->children.size(), 2u);
    ASSERT_EQ(tr.m_root->children[0]->data, 43);
    ASSERT_EQ(tr.m_root->children[0]->children.size(), 0u);
    ASSERT_EQ(tr.m_root->children[0]->parent, tr.m_root.get());
    ASSERT_EQ(tr.m_root->children[1]->data, 44);
    ASSERT_EQ(tr.m_root->children[1]->children.size(), 0u);
    ASSERT_EQ(tr.m_root->children[1]->parent, tr.m_root.get());
    ASSERT_EQ(tr.m_root->children[1].get(), &n2);
    ASSERT_EQ(&(n2.root()), tr.m_root.get());
    
    Tree<int>::Node& n3 = tr.insert(n1, 431);
    ASSERT_EQ(tr.size(), 4u);
    ASSERT_EQ(tr.empty(), false);
    ASSERT_NE(tr.m_root, nullptr);
    ASSERT_EQ(tr.m_root->data, 42);
    ASSERT_EQ(tr.m_root->parent, nullptr);
    ASSERT_EQ(tr.m_root->children.size(), 2u);
    ASSERT_EQ(tr.m_root->children[0]->data, 43);
    ASSERT_EQ(tr.m_root->children[0]->children.size(), 1u);
    ASSERT_EQ(tr.m_root->children[0]->parent, tr.m_root.get());
    ASSERT_EQ(tr.m_root->children[0]->children[0]->data, 431);
    ASSERT_EQ(tr.m_root->children[0]->children[0]->children.size(), 0u);
    ASSERT_EQ(tr.m_root->children[0]->children[0]->parent, &n1);
    ASSERT_EQ(tr.m_root->children[1]->data, 44);
    ASSERT_EQ(tr.m_root->children[1]->children.size(), 0u);
    ASSERT_EQ(tr.m_root->children[1]->parent, tr.m_root.get());
    ASSERT_EQ(&(n3.root()), tr.m_root.get());

    tr.insert(n1, { 432, 433 });
    ASSERT_EQ(tr.size(), 6u);
    ASSERT_EQ(tr.empty(), false);
    ASSERT_NE(tr.m_root, nullptr);
    ASSERT_EQ(tr.m_root->data, 42);
    ASSERT_EQ(tr.m_root->parent, nullptr);
    ASSERT_EQ(tr.m_root->children.size(), 2u);
    ASSERT_EQ(tr.m_root->children[0]->data, 43);
    ASSERT_EQ(tr.m_root->children[0]->children.size(), 3u);
    ASSERT_EQ(tr.m_root->children[0]->parent, tr.m_root.get());
    ASSERT_EQ(tr.m_root->children[0]->children[0]->data, 431);
    ASSERT_EQ(tr.m_root->children[0]->children[0]->children.size(), 0u);
    ASSERT_EQ(tr.m_root->children[0]->children[0]->parent, &n1);
    ASSERT_EQ(tr.m_root->children[0]->children[1]->data, 432);
    ASSERT_EQ(tr.m_root->children[0]->children[1]->children.size(), 0u);
    ASSERT_EQ(tr.m_root->children[0]->children[1]->parent, &n1);
    ASSERT_EQ(tr.m_root->children[0]->children[2]->data, 433);
    ASSERT_EQ(tr.m_root->children[0]->children[2]->children.size(), 0u);
    ASSERT_EQ(tr.m_root->children[0]->children[2]->parent, &n1);
    ASSERT_EQ(tr.m_root->children[1]->data, 44);
    ASSERT_EQ(tr.m_root->children[1]->children.size(), 0u);
    ASSERT_EQ(tr.m_root->children[1]->parent, tr.m_root.get());
    ASSERT_EQ(&(n3.root()), tr.m_root.get());

    tr.print();
    tr.traverse([](Tree<int>::Node const& node) { std::cout << node.data << std::endl; });

    Tree<int> const tr1({55, 56, 57}); tr1.print();
    ASSERT_EQ(tr1.size(), 3u);
    ASSERT_EQ(tr1.empty(), false);
    ASSERT_NE(tr1.m_root, nullptr);
    ASSERT_EQ(tr1.m_root->data, 55);
    ASSERT_EQ(tr1.m_root->parent, nullptr);
    ASSERT_EQ(tr1.m_root->children.size(), 2u);
    ASSERT_EQ(tr1.m_root->children[0]->data, 56);
    ASSERT_EQ(tr1.m_root->children[0]->children.size(), 0u);
std::cout << "QQ " << tr1.m_root->children[0]->parent->data << std::endl;
    //ASSERT_EQ(tr1.m_root->children[0]->parent, tr.m_root.get());
    ASSERT_EQ(tr1.m_root->children[1]->data, 57);
    ASSERT_EQ(tr1.m_root->children[1]->children.size(), 0u);
    //ASSERT_EQ(tr1.m_root->children[1]->parent, tr.m_root.get());
    tr1.print();
}
