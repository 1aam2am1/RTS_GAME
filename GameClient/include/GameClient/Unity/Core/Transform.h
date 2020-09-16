//
// Created by Michal_Marszalek on 15.09.2020.
//

#ifndef RTS_GAME_TRANSFORM_H
#define RTS_GAME_TRANSFORM_H


#include <SFML/System/Vector3.hpp>
#include "Component.h"
#include "Quaternion.h"
#include <GameClient/TPtr.h>

/// Position, rotation and scale of an object
class Transform : public Component {
public:
    /// The coordinate space in which to operate.
    enum class Space {
        World, ///< Applies transformation relative to the world coordinate system.
        Self ///< Applies transformation relative to the local coordinate system.
    };

    /// The number of children the parent Transform has.
    int childCount;

    /// The rotation as Euler angles in degrees.
    /// \note Euler angles can represent a three dimensional rotation by performing three separate rotations
    /// around individual axes. In Unity these rotations are performed around the \b Z axis, the \b X axis, and the \b Y axis,
    /// in that order.
    sf::Vector3f eulerAngles;

    /// The rotation as Euler angles in degrees relative to the parent transform's rotation.
    sf::Vector3f localEulerAngles;

    /// Position of the transform relative to the parent transform.
    sf::Vector3f localPosition;

    /// The rotation of the transform relative to the transform rotation of the parent.
    Quaternion localRotation;

    /// The scale of the transform relative to the GameObjects parent.
    sf::Vector3f localScale;

    /// The parent of the transform.
    /// \note Changing the parent will modify the parent-relative position, scale and rotation
    /// but keep the world space position, rotation and scale the same.
    TPTR_PT(Transform, parent);

    /// Returns the topmost transform in the hierarchy.
    /// \note This never returns null, if this Transform doesn't have a parent it returns itself.
    TPTR_PT(Transform, root);

    /// A Quaternion that stores the rotation of the Transform in world space.
    Quaternion rotation;

    /// The world space position of the Transform
    sf::Vector3f position;

    /// Unparents all children.
    /// \note Useful if you want to destroy the root of a hierarchy without destroying the children.
    void DetachChildren();

    /// Finds a child by n and returns it.
    /// \param n Name of child to be found.
    /// \return If no child with n can be found, null is returned.
    TPtr<Transform> Find(std::string_view n);

    /// Returns a transform child by index.
    /// \param index Index of the child transform to return. Must be smaller than Transform.childCount.
    /// \return Transform Transform child by index.
    TPtr<Transform> GetChild(int index);

    /// Gets the sibling index.
    int GetSiblingIndex();

    /// Transforms a direction from world space to local space. The opposite of Transform.TransformDirection.
    /// \note This operation is unaffected by scale.
    /// \details rotation only
    sf::Vector3f InverseTransformDirection(sf::Vector3f direction);

    /// Transforms position from world space to local space.
    /// \details position, rotation, and scale
    sf::Vector3f InverseTransformPoint(sf::Vector3f position);

    /// Transforms a vector from world space to local space. The opposite of Transform.TransformVector.
    /// \details rotation and scale only
    sf::Vector3f InverseTransformVector(sf::Vector3f vector);

    /// Is this transform a child of parent?
    /// \return Returns a boolean value that indicates whether the transform is a child of a given transform.
    /// true if this transform is a child, deep child (child of a child) or identical to this transform, otherwise false.
    bool IsChildOf(const TPtr<Transform> &parent);

    /// Rotates the transform so the forward vector points at /target/'s current position.
    /// \param target Object to point towards.
    /// \param worldUp Vector specifying the upward direction.
    void LookAt(Transform target, sf::Vector3f worldUp);

    /// Rotates the transform so the forward vector points at worldPosition.
    /// \param worldPosition Point to look at.
    /// \param worldUp Vector specifying the upward direction.
    void LookAt(sf::Vector3f worldPosition, sf::Vector3f worldUp);

    /// Applies a rotation of eulerAngles.z degrees around the z-axis, eulerAngles.x degrees around the x-axis,
    /// and eulerAngles.y degrees around the y-axis (in that order).
    /// \param eulers The rotation to apply.
    /// \param relativeTo Determines whether to rotate the GameObject either locally to the GameObject or relative to the Scene in world space.
    void Rotate(sf::Vector3f eulers, Space relativeTo = Space::Self);

    /// Rotates the transform about axis passing through point in world coordinates by angle degrees.
    /// \note This modifies both the position and the rotation of the transform.
    void RotateAround(sf::Vector3f point, sf::Vector3f axis, float angle);

    /// Set the parent of the transform.
    /// \param parent The parent Transform to use.
    /// \param worldPositionStays If true, the parent-relative position, scale and rotation are modified such that
    /// the object keeps the same world space position, rotation and scale as before.
    void SetParent(const TPtr<Transform> &parent, bool worldPositionStays);

    /// Sets the world space position and rotation of the Transform component.
    void SetPositionAndRotation(sf::Vector3f position, Quaternion rotation);

    /// Transforms direction from local space to world space.
    sf::Vector3f TransformDirection(sf::Vector3f direction);

    /// Transforms position from local space to world space.
    sf::Vector3f TransformPoint(sf::Vector3f position);

    /// Transforms vector from local space to world space.
    sf::Vector3f TransformVector(sf::Vector3f vector);

    /// Moves the transform in the direction and distance of translation.
    /// \note If relativeTo is left out or set to Space.Self the movement is applied relative to the transform's local axes.
    /// (the x, y and z axes shown when selecting the object inside the Scene View.)
    /// If relativeTo is Space.World the movement is applied relative to the world coordinate system.
    void Translate(sf::Vector3f translation, Space relativeTo = Space::Self);
};


#endif //RTS_GAME_TRANSFORM_H
