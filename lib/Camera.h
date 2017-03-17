#pragma once

#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"
#include "../vendor/glm/gtc/quaternion.hpp"

namespace diffusion {
namespace camera {

/**
 * @brief      Creates a 3D camera that can be used to generate a View Matrix.
 *
 *             Maintains an internal position and angle which can
 *             accessed and modified with getters and setters. Internally
 *             quaternions are used to prevent gimbal lock.
 */
class Camera3D {
private:
  glm::vec3 position;
  glm::quat orientation;
  glm::mat4 viewMatrix;

  /**
   * @brief      Updates the view matrix - call after any changes to position or
   *             orientation.
   */
  void updateViewMatrix() {
    glm::mat4 rotate = glm::mat4_cast(orientation);
    glm::mat4 translate;
    translate = glm::translate(translate, -position);
    viewMatrix = rotate * translate;
  }

public:
  /**
   * @brief      Constructs a new Camera3D with identity position and
   *             orientation.
   *
   *             Provides a view matrix equivalent to the
   *             identity matrix. This is also equivalent to calling
   *             glm::lookAtMatrix with position=(0,0,0) target=(0,0,-1)
   *             up=(0,1,0).
   */
  Camera3D() { updateViewMatrix(); }

  /**
   * @brief      Constructs a new Camera3D with the given position.
   *
   *             Provides a view matrix equivalent to the
   *             identity matrix translated to the negative value of the given
   *             position. Orientation is unchanged.
   * @param[in]  position  The intial position
   */
  Camera3D(const glm::vec3 &position) : position(position) {
    updateViewMatrix();
  }

  /**
   * @brief      Constructs a new Camera3D with the given position and
   *             orientation.
   *
   *             Provides a view matrix equivalent to the
   *             identity matrix translated to the negative value of the given
   *             position and rotated to the given orientation.
   * @param[in]  position     The initial position
   * @param[in]  orientation  The initial orientation
   */
  Camera3D(const glm::vec3 &position, const glm::quat orientation)
      : position(position) {
    this->orientation = glm::normalize(orientation);
    updateViewMatrix();
  }

  /**
   * @brief      Returns The View Matrix.
   *
   * @return     The View Matrix.
   */
  glm::mat4 getViewMatrix() { return viewMatrix; }

  /**
   * @brief      Rotates the camera by glm::vec3(pitch, yaw, roll).
   *
   * @param[in]  angles  The angles in radians to rotate the camera by
   */
  void rotate(const glm::vec3 &angles) { rotate(glm::quat(angles)); }

  /**
   * @brief      Rotates the camera by the given angle in radians around the
   *             given axis.
   *
   * @param[in]  angle  The angle in radians to rotate the camera by
   * @param[in]  axis   The axis around which to rotate
   */
  void rotate(float angle, const glm::vec3 axis) {
    rotate(glm::angleAxis(angle, axis));
  }

  /**
   * @brief      Rotates the camera by the given Quaternion
   *
   * @param[in]  q     The quaternion to rotate the camera by
   */
  void rotate(const glm::quat &q) {
    orientation = glm::normalize(orientation * q);
    updateViewMatrix();
  }

  /**
   * @brief      Translates the camera by the given values relative to it's
   *             current orientation.
   *
   * @param[in]  x     Amount to translate along relative x axis
   * @param[in]  y     Amount to translate along relative y axis
   * @param[in]  z     Amount to translate along relative z axis
   */
  void translate(float x, float y, float z) { translate(glm::vec3(x, y, z)); }

  /**
   * @brief      Translates the camera by the given vector relative to it's
   *             current orientation
   *
   * @param[in]  v     Vector to translate camera relative to it's orientation
   */
  void translate(const glm::vec3 &v) {
    position = position + (v * orientation);
    updateViewMatrix();
  }

  /**
   * @brief      Rotate the camera by the given pitch in radians
   *
   *             Rotating by pitch is equivalent to rotating around the x axis
   *
   * @param[in]  angle  The angle in radians
   */
  void pitch(float angle) { rotate(angle, glm::vec3(1.0f, 0.0f, 0.0f)); }

  /**
   * @brief      Rotate the camera by the given yaw in radians
   *
   *             Rotating by yaw is equivalent to rotating around the y axis
   *
   * @param[in]  angle  The angle in radians
   */
  void yaw(float angle) { rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f)); }

  /**
   * @brief      Rotate the camera by the given roll in radians
   *
   *             Rotating by roll is equivalent to rotating around the z axis
   *
   * @param[in]  angle  The angle in radians
   */
  void roll(float angle) { rotate(angle, glm::vec3(0.0f, 0.0f, 1.0f)); }

  /**
   * @brief      Get the current orientation of the camera as a vector of euler
   *             angles (pitch, yaw, roll)
   *
   * @return     A vector containing pitch, yaw, and roll
   */
  glm::vec3 getEulerAngles() { return glm::eulerAngles(orientation); }

  /**
   * @brief      Sets the orientation of the camera using euler angles (pitch,
   *             yaw, and roll)
   *
   * @param[in]  angles  A vector containing pitch, yaw and roll
   */
  void setEulerAngles(const glm::vec3 &angles) {
    orientation = glm::normalize(glm::quat(angles));
    updateViewMatrix();
  }

  /**
   * @brief      Gets the current orientation of the camera as a Quaternion
   *
   * @return     The orientation.
   */
  glm::quat getOrientation() { return orientation; }

  /**
   * @brief      Sets the orientation of the camera to the given Quaternion
   *
   * @param[in]  q     The quarternion
   */
  void setOrientation(const glm::quat &q) {
    orientation = glm::normalize(q);
    updateViewMatrix();
  }

  /**
   * @brief      Gets the pitch in radians
   *
   * @return     The pitch in radians
   */
  float getPitch() { return glm::pitch(orientation); }

  /**
   * @brief      Sets the pitch in radians
   *
   * @param[in]  radians  The pitch in radians
   */
  void setPitch(float radians) {
    glm::vec3 angles = getEulerAngles();
    angles[0] = radians;
    setEulerAngles(angles);
  }

  /**
   * @brief      Gets the yaw in radians.
   *
   * @return     The yaw in radians.
   */
  float getYaw() { return glm::yaw(orientation); }

  /**
   * @brief      Sets the yaw in radians.
   *
   * @param[in]  radians  The yaw in radians
   */
  void setYaw(float radians) {
    glm::vec3 angles = getEulerAngles();
    angles[1] = radians;
    setEulerAngles(angles);
  }

  /**
   * @brief      Gets the roll in radians.
   *
   * @return     The roll in radians.
   */
  float getRoll() { return glm::roll(orientation); }

  /**
   * @brief      Sets the roll in radians.
   *
   * @param[in]  radians  The roll in radians
   */
  void setRoll(float radians) {
    glm::vec3 angles = getEulerAngles();
    angles[2] = radians;
    setEulerAngles(angles);
  }

  /**
   * @brief      Gets the current position of the camera.
   *
   * @return     The position as a vector
   */
  glm::vec3 getPosition() { return position; }

  /**
   * @brief      Sets the position of the camera.
   *
   * @param[in]  v     Vector containing xyz position
   */
  void setPosition(const glm::vec3 &v) {
    position = v;
    updateViewMatrix();
  }

  /**
   * @brief      Gets the x coordinate of the cameras position.
   *
   * @return     The x coordinate of the cameras position
   */
  float getX() { return position[0]; }

  /**
   * @brief      Sets the x coordinate of the cameras position.
   *
   * @param[in]  x
   */
  void setX(float x) {
    position[0] = x;
    updateViewMatrix();
  }

  /**
   * @brief      Gets the y coordinate of the cameras position.
   *
   * @return     The y coordinate of the cameras position
   */
  float getY() { return position[1]; }

  /**
   * @brief      Sets the y coordinate of the cameras position.
   *
   * @param[in]  y
   */
  void setY(float y) {
    position[1] = y;
    updateViewMatrix();
  }

  /**
   * @brief      Gets the z coordinate of the cameras position.
   *
   * @return     The z coordinate of the cameras position
   */
  float getZ() { return position[2]; }

  /**
   * @brief      Sets the z coordinate of the cameras position.
   *
   * @param[in]  z
   */
  void setZ(float z) {
    position[2] = z;
    updateViewMatrix();
  }
};
}
}
