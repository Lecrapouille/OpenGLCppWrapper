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

using namespace units::mass;

//------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os, btVector3 const& v)
{
    return os << "[" << v[0] << ", " << v[1] << ", " << v[2] << ']';
}

//------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os, btQuaternion const& q)
{
    return os << "(" << q.getW() << ", (" << q.getX() << ", "
              << q.getY() << ", " << q.getZ() << "))";
}

//------------------------------------------------------------------------------
inline static btVector3 cast_vector(Vector3f const& v)
{
    return btVector3(v[0], v[1], v[2]);
}

//------------------------------------------------------------------------------
inline static Vector3f cast_vector(btVector3 const& v)
{
    return Vector3f(v.getX(), v.getY(), v.getZ());
}

//------------------------------------------------------------------------------
inline static btQuaternion cast_quaternion(Quatf const& q)
{
    return btQuaternion(q.x(), q.y(), q.z(), q.w());
}

//------------------------------------------------------------------------------
inline static Quatf cast_quaternion(btQuaternion const& q)
{
    return Quatf(q.getW(), q.getX(), q.getY(), q.getZ());
}

//------------------------------------------------------------------------------
DynamicWorld::DynamicWorld(Vector3f const& gravity)
{
    m_collisionConfiguration = std::make_unique<btDefaultCollisionConfiguration>();
    m_dispatcher = std::make_unique<btCollisionDispatcher>(m_collisionConfiguration.get());
    m_broadphase = std::make_unique<btDbvtBroadphase>();
    m_solver = std::make_unique<btSequentialImpulseConstraintSolver>();
    m_dynamic = std::make_unique<btDiscreteDynamicsWorld>
                (m_dispatcher.get(), m_broadphase.get(),
                 m_solver.get(), m_collisionConfiguration.get());
    m_dynamic->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
}

//------------------------------------------------------------------------------
void DynamicWorld::attach(RigidBody& obj)
{
    m_dynamic->addRigidBody(&obj.rigidBody());
    m_objects.push_front(&obj);
}

//------------------------------------------------------------------------------
void DynamicWorld::detach(RigidBody& obj)
{
    m_dynamic->removeRigidBody(&obj.rigidBody());
    m_objects.push_front(&obj);
}

//------------------------------------------------------------------------------
void DynamicWorld::update(float dt)
{
    m_dynamic->stepSimulation(dt);

    for (auto it: m_objects)
    {
        it->update();
    }
}

//------------------------------------------------------------------------------
void DynamicWorld::reset()
{
    for (auto it: m_objects)
    {
        it->reset();
    }
}

//------------------------------------------------------------------------------
RigidBody::RigidBody(Transformable3D& transform,
                     std::unique_ptr<btCollisionShape> shape,
                     kilogram_t mass,
                     btScalar restitution,
                     btScalar friction)
    : m_transform(transform),
      m_initial_transform(cast_quaternion(transform.attitude()),
                          cast_vector(transform.position()))
{
    m_collision_shape = std::move(shape);
    btTransform t(btQuaternion(0,0,0,1), btVector3(0,0,0));
    m_motion = std::make_unique<btDefaultMotionState>(t);
    btVector3 inertia(0,0,0);
    btRigidBody::btRigidBodyConstructionInfo
            info(mass.to<btScalar>(),
                 m_motion.get(),
                 m_collision_shape.get(),
                 inertia);
    info.m_restitution = restitution;
    info.m_friction = friction;
    m_rigid_body = std::make_unique<btRigidBody>(info);
    setMass(mass);
}

//------------------------------------------------------------------------------
void RigidBody::setInitialTransform(Transformable3D const& transform, bool apply)
{
    btTransform t(cast_quaternion(transform.attitude()),
                  cast_vector(transform.position()));
    m_initial_transform = t;
    if (apply) { reset(); }
}

//------------------------------------------------------------------------------
void RigidBody::setMass(kilogram_t mass)
{
    btVector3 inertia(0, 0, 0);
    btScalar m = mass.to<btScalar>();

    if (m > 0) // Dynamic object
    {
        m_collision_shape->calculateLocalInertia(m, inertia);
    }

    m_rigid_body->setMassProps(m, inertia);
    m_mass = mass;
}

//------------------------------------------------------------------------------
void RigidBody::reset()
{
    m_rigid_body->setWorldTransform(m_initial_transform);
    m_rigid_body->getMotionState()->setWorldTransform(m_initial_transform);
    m_rigid_body->clearForces();

    if (m_mass.to<btScalar>() > 0) // Dynamic object
    {
        m_rigid_body->setLinearVelocity(btVector3(0, 0, 0));
        m_rigid_body->setAngularVelocity(btVector3(0, 0, 0));
        m_rigid_body->activate(); // Required if the object was asleep
    }

    m_transform.attitude(cast_quaternion(m_initial_transform.getRotation()));
    m_transform.position(cast_vector(m_initial_transform.getOrigin()));
}

//------------------------------------------------------------------------------
void RigidBody::update()
{
    btTransform wTrans;
    m_rigid_body->getMotionState()->getWorldTransform(wTrans);
    m_transform.attitude(cast_quaternion(wTrans.getRotation()));
    m_transform.position(cast_vector(wTrans.getOrigin()));

    std::cout << "------------" << std::endl;
    std::cout << "bullet\n  pos: " << wTrans.getOrigin() << std::endl;
    std::cout << "  qua: " << wTrans.getRotation() << std::endl;

    std::cout << "qq\n  pos: " << m_transform.position() << std::endl;
    std::cout << "  qua: " << m_transform.attitude() << std::endl;
}

//------------------------------------------------------------------------------
static btVector3 thickness2norm(Vector3f const& thickness)
{
    float dx = std::max(thickness.x, 0.01f);
    float dy = std::max(thickness.y, 0.01f);
    float dz = std::max(thickness.z, 0.01f);

    btVector3 norm;
    if (dx < dy && dx < dz)
        norm = btVector3(1.0f, 0.0f, 0.0f);
    else if (dy < dx && dy < dz)
        norm = btVector3(0.0f, 1.0f, 0.0f);
    else
        norm = btVector3(0.0f, 0.0f, 1.0f);

    return norm;
}

namespace rigidbody
{

    //--------------------------------------------------------------------------
    WorldPlane::WorldPlane(Vector3f const& thickness,
                           float restitution, float friction)
        : RigidBody(m_transform_,
                    std::make_unique<btStaticPlaneShape>(thickness2norm(thickness), 0),
                    kilogram_t(0), restitution, friction)
    {}

    //--------------------------------------------------------------------------
    Sphere::Sphere(Transformable3D& transform, float radius, kilogram_t mass,
                   float restitution, float friction)
        : RigidBody(transform, std::make_unique<btSphereShape>(radius),
                    mass, restitution, friction)
    {}

} // namespace rigidbody
