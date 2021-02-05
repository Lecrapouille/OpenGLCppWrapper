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

#include "Components/Physics/BulletWrapper.hpp"
#include <iostream>

PhysicWorld::PhysicWorld()
{
    collisionConfiguration = new btDefaultCollisionConfiguration();
    if (collisionConfiguration == nullptr)
        std::cerr << "collisionConfiguration == nullptr" << std::endl;
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    if (dispatcher == nullptr)
        std::cerr << "dispatcher == nullptr" << std::endl;
    broadphase = new btDbvtBroadphase();
    if (broadphase == nullptr)
        std::cerr << "broadphase == nullptr" << std::endl;
    solver = new btSequentialImpulseConstraintSolver;
    if (solver == nullptr)
        std::cerr << "solver == nullptr" << std::endl;
    dynamic = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
    if (dynamic == nullptr)
        std::cerr << "dynamic == nullptr" << std::endl;
    std::cerr << "ICI"  << std::endl;
    dynamic->setGravity(btVector3(0.0f, 0.0f, -9.8f));
}
