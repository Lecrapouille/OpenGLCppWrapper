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

#ifndef BULLETWRAPPER_HPP
#  define BULLETWRAPPER_HPP

#    pragma GCC diagnostic push
#      pragma GCC diagnostic ignored "-Wsign-conversion"
#      pragma GCC diagnostic ignored "-Wold-style-cast"
#      pragma GCC diagnostic ignored "-Wfloat-equal"
#      pragma GCC diagnostic ignored "-Wold-style-cast"
#      pragma GCC diagnostic ignored "-Wcast-qual"
#      pragma GCC diagnostic ignored "-Wfloat-conversion"
#      pragma GCC diagnostic ignored "-Wconversion"
#      define BT_THREADSAFE 0
#      include "bullet/btBulletDynamicsCommon.h"
#    pragma GCC diagnostic pop

#  include "Math/Transformable.hpp"
#  include "Math/Vector.hpp"
#  include <forward_list>

class RigidBody;

class DynamicWorld
{
public:

    DynamicWorld(Vector3f const& gravity = Vector3f(0.0f, -9.8f, 0.0f));
    void attach(RigidBody& obj);
    void detach(RigidBody& obj);
    void update(float dt); // FIXME in seconds
    void reset(); //     void setInitState();
    btDynamicsWorld& dynamic() { return *m_dynamic; }

private:

    std::unique_ptr<btDefaultCollisionConfiguration> m_collisionConfiguration;
    std::unique_ptr<btCollisionDispatcher> m_dispatcher;
    std::unique_ptr<btDbvtBroadphase> m_broadphase;
    std::unique_ptr<btSequentialImpulseConstraintSolver> m_solver;
    std::unique_ptr<btDynamicsWorld> m_dynamic;
    std::forward_list<RigidBody*> m_objects;
};


// *****************************************************************************
//! \brief Base class for a moving SceneTree node. Integrate a physical object
//! (btRigidBody), a collision shape (btCollisionShape) and the transform
//! instance of the  SceneNode owning this instance.
// *****************************************************************************
class RigidBody
{
public:

    //--------------------------------------------------------------------------
    //! \brief Creates a new rigid body as a btRigidBody object.
    //! \param[in] reference to the transform attitude and position of the
    //!  SceneTree node owning this class instance.
    //! \param[in] collision_shape
    //! \param[in] mass
    //! \param[in] restitution
    //! \param[in] friction
    //--------------------------------------------------------------------------
    RigidBody(Transformable3D& transform,
              std::unique_ptr<btCollisionShape> collision_shape,
              units::mass::kilogram_t mass,
              float restitution,
              float friction);

    //--------------------------------------------------------------------------
    //! \brief Save the initial transform applied on reset events to restore
    //! states. Call this method after the constructor in the case the attitude
    //! and position passed as parameter of the constructor was not init.
    //! \param[in] transform the attitude and position to apply on reset.
    //! \param[in] reset if set to true for
    //--------------------------------------------------------------------------
    void setInitialTransform(Transformable3D const& transform, bool reset = true);

    //--------------------------------------------------------------------------
    //! \brief Set the object's mass and replace the one used as param in the
    //! constructor.
    //! \param mass specifies mass in kg units.
    //! \note If mass is > 0 the object will be dynamic, else will be static.
    //--------------------------------------------------------------------------
    void setMass(units::mass::kilogram_t mass);

    //--------------------------------------------------------------------------
    //! \brief Return the Bullet3 rigid body instance
    //--------------------------------------------------------------------------
    inline btRigidBody& rigidBody()
    {
        // Never null since set by the constructor
        return *m_rigid_body;
    }

    //--------------------------------------------------------------------------
    //! \brief Reposition object to its inital state.
    //--------------------------------------------------------------------------
    void reset();

    //--------------------------------------------------------------------------
    //! \brief Update callback.
    //! This function is called automatically by DynamicWorld::update() for each
    //! registered object. It update position and attitude of graphical object
    //! from its physhical state.
    //--------------------------------------------------------------------------
    void update();

protected:

    //! \brief Transform instance of the SceneTree node owner (3d model).
    Transformable3D& m_transform;
    //! \brief Inital transform state (orientation and position). Applied on
    //! reset events to restore states.
    btTransform m_initial_transform;
    //! \brief Object's collision shape.
    std::unique_ptr<btCollisionShape> m_collision_shape;
    //! \brief Object's rigid body
    std::unique_ptr<btRigidBody> m_rigid_body;
    //! \brief Memorize this state to be released during the destruction of this
    //! instance
    std::unique_ptr<btDefaultMotionState> m_motion = nullptr;
    //! \brief Mass of the object (0 for static object)
    units::mass::kilogram_t m_mass;
};

namespace rigidbody
{

    // *************************************************************************
    //! \brief
    // *************************************************************************
    class WorldPlane: public RigidBody
    {
    public:

        WorldPlane(Vector3f const& thickness,
                   float restitution = 0.0f, float friction = 0.5f);

    private:

        Transformable3D m_transform_;
    };

    // *************************************************************************
    //! \brief
    // *************************************************************************
    class Sphere: public RigidBody
    {
    public:

        Sphere(Transformable3D& transform, float radius, units::mass::kilogram_t mass,
               float restitution = 0.0f, float friction = 0.5f);
    };

} // namespace rigidbody

#endif // BULLETWRAPPER_HPP
