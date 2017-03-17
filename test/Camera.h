#include "../lib/Camera.h"

#include "../vendor/catch.hpp"
#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/epsilon.hpp"

#include <cmath>

const float EPSILON = 1e-6f;

/**
 * @brief      Compares two 4x4 matrices for equality within epsilon
 *
 * @param[in]  a
 * @param[in]  b
 *
 * @return     true if matrices are equivalent within floating point tolerance
 */
bool mat4Equivalent(glm::mat4 a, glm::mat4 b) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (std::abs(a[i][j] - b[i][j]) > EPSILON) {
        return false;
      }
    }
  }
  return true;
}

SCENARIO("Camera3D constructors", "[Camera3D]") {

  GIVEN("Camera3D's Default Constructor") {
    diffusion::camera::Camera3D myCamera;

    WHEN("getViewMatrix is called") {
      glm::mat4 view = myCamera.getViewMatrix();

      THEN("It should return an identity matrix") {
        glm::mat4 identity;

        REQUIRE(mat4Equivalent(view, identity));
      }

      WHEN("Compared to an equivalent lookAt matrix") {
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 target = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::mat4 lookAtMatrix = glm::lookAt(position, target, up);

        THEN("It should be identical") {
          REQUIRE(mat4Equivalent(view, lookAtMatrix));
        }
      }
    }
  }

  GIVEN("Camera3D's Constructor with Position") {
    glm::vec3 myPosition = glm::vec3(1.5f, 0.2f, -1.2f);
    diffusion::camera::Camera3D myCamera(myPosition);

    WHEN("getPosition is called") {
      glm::vec3 cameraPosition = myCamera.getPosition();

      THEN("It should return the position the camera was constructed with") {
        REQUIRE(myPosition == cameraPosition);
      }
    }

    WHEN("getViewMatrix is called") {
      glm::mat4 view = myCamera.getViewMatrix();

      WHEN("Compared to an equivalent lookAt matrix") {
        glm::vec3 position = myPosition;
        // Facing towards -z axis
        glm::vec3 target = myPosition + glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::mat4 lookAtMatrix = glm::lookAt(position, target, up);

        THEN("It should be identical") {
          REQUIRE(mat4Equivalent(view, lookAtMatrix));
        }
      }
    }
  }

  GIVEN("Camera3D's Constructor with Position and Orientation") {
    glm::vec3 myPosition = glm::vec3(1.1f, 0.1f, 0.3f);
    glm::quat myOrientation =
        glm::quat(glm::vec3(0.0f, glm::radians(180.0f), 0.0f));
    diffusion::camera::Camera3D myCamera(myPosition, myOrientation);

    WHEN("getPosition is called") {
      glm::vec3 cameraPosition = myCamera.getPosition();

      THEN("It should return the position the camera was constructed with") {
        REQUIRE(myPosition == cameraPosition);
      }
    }

    WHEN("getOrientation is called") {
      glm::quat cameraOrientation = myCamera.getOrientation();

      THEN("It should return the orientation the camera was constructed with") {
        REQUIRE(myOrientation == cameraOrientation);
      }
    }

    WHEN("Compared to an equivalent lookAt matrix") {
      glm::mat4 view = myCamera.getViewMatrix();

      glm::vec3 position = myPosition;
      // Facing 180 degrees to the identity around the y axis, along the
      // positive z axis
      glm::vec3 target = myPosition + glm::vec3(0.0f, 0.0f, 1.0f);
      glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
      glm::mat4 lookAtMatrix = glm::lookAt(position, target, up);

      THEN("It should be identical") {
        REQUIRE(mat4Equivalent(view, lookAtMatrix));
      }
    }
  }
}

SCENARIO("Camera3D rotate", "[Camera3D][rotate]") {

  GIVEN("Camera3D's rotate by euler angles") {
    diffusion::camera::Camera3D myCamera;
    myCamera.rotate(glm::vec3(0.0f, glm::radians(-90.0f), 0.0f));
    glm::mat4 view = myCamera.getViewMatrix();

    WHEN("Compared to an equivalent lookAt matrix") {
      glm::mat4 lookAtMatrix = glm::lookAt(
          glm::vec3(0.0f, 0.0f, 0.0f),
          // Point 90 degrees to the left, along the negative x axis
          glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

      THEN("It should be identical") {
        REQUIRE(mat4Equivalent(view, lookAtMatrix));
      }
    }
  }

  GIVEN("Camera3D's rotate by an angle and an axis") {
    diffusion::camera::Camera3D myCamera;
    myCamera.rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 view = myCamera.getViewMatrix();

    WHEN("Compared to an equivalent lookAt matrix") {
      glm::mat4 lookAtMatrix = glm::lookAt(
          glm::vec3(0.0f, 0.0f, 0.0f),
          // Point 90 degrees down, along the negative y axis
          glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

      THEN("It should be identical") {
        REQUIRE(mat4Equivalent(view, lookAtMatrix));
      }
    }
  }

  GIVEN("Camera3D's rotate by a Quaternion") {
    diffusion::camera::Camera3D myCamera;
    myCamera.rotate(glm::quat(glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f)));
    glm::mat4 view = myCamera.getViewMatrix();

    WHEN("Compared to an equivalent lookAt matrix") {
      glm::mat4 lookAtMatrix =
          glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f),
                      // Point 90 degrees up, along the positive y axis
                      glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

      THEN("It should be identical") {
        REQUIRE(mat4Equivalent(view, lookAtMatrix));
      }
    }
  }

  GIVEN("Camera3D is rotated via it's pitch method") {
    diffusion::camera::Camera3D myCamera;
    myCamera.pitch(glm::radians(90.0f));
    glm::mat4 view = myCamera.getViewMatrix();

    WHEN("Compared to an equivalent lookAt matrix") {
      glm::mat4 lookAtMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f),
                                           // Looking down -y axis
                                           glm::vec3(0.0f, -1.0f, 0.0f),
                                           glm::vec3(0.0f, 1.0f, 0.0f));

      THEN("It should be identical") {
        REQUIRE(mat4Equivalent(view, lookAtMatrix));
      }
    }
  }

  GIVEN("Camera3D is rotated via it's yaw method") {
    diffusion::camera::Camera3D myCamera;
    myCamera.yaw(glm::radians(90.0f));
    glm::mat4 view = myCamera.getViewMatrix();

    WHEN("Compared to an equivalent lookAt matrix") {
      glm::mat4 lookAtMatrix =
          glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f),
                      // Looking down x axis
                      glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

      THEN("It should be identical") {
        REQUIRE(mat4Equivalent(view, lookAtMatrix));
      }
    }
  }

  GIVEN("Camera3D is rotated via it's roll method") {
    diffusion::camera::Camera3D myCamera;
    myCamera.roll(glm::radians(90.0f));
    glm::mat4 view = myCamera.getViewMatrix();

    WHEN("Compared to an equivalent lookAt matrix") {
      glm::mat4 lookAtMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f),
                                           // Looking down -z axis
                                           glm::vec3(0.0f, 0.0f, -1.0f),
                                           // Up becomes x axis
                                           glm::vec3(1.0f, 0.0f, 0.0f));

      THEN("It should be identical") {
        REQUIRE(mat4Equivalent(view, lookAtMatrix));
      }
    }
  }
}

SCENARIO("Camera3D Translate", "[Camera3D][translate]") {

  GIVEN("Camera3D's translate by floats") {
    diffusion::camera::Camera3D myCamera;
    myCamera.translate(0.0f, 0.5f, 0.0f);
    glm::mat4 view = myCamera.getViewMatrix();

    WHEN("Compared to an equivalent lookAt matrix") {
      glm::mat4 lookAtMatrix =
          glm::lookAt(glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.0f, 0.5f, -1.0f),
                      glm::vec3(0.0f, 1.0f, 0.0f));

      THEN("It should be identical") {
        REQUIRE(mat4Equivalent(view, lookAtMatrix));
      }
    }
  }

  GIVEN("Camera3D's translate by vector") {
    diffusion::camera::Camera3D myCamera;
    myCamera.translate(glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 view = myCamera.getViewMatrix();

    WHEN("Compared to an equivalent lookAt matrix") {
      glm::mat4 lookAtMatrix = glm::lookAt(glm::vec3(1.0f, 0.0f, 0.0f),
                                           // Looking down the negative z axis
                                           glm::vec3(1.0f, 0.0f, -1.0f),
                                           glm::vec3(0.0f, 1.0f, 0.0f));

      THEN("It should be identical") {
        REQUIRE(mat4Equivalent(view, lookAtMatrix));
      }
    }
  }

  GIVEN("Camera3D's translate by vector when already rotated") {
    diffusion::camera::Camera3D myCamera;
    myCamera.rotate(glm::radians(90.0), glm::vec3(0.0, 1.0, 0.0));
    myCamera.translate(glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 view = myCamera.getViewMatrix();

    WHEN("Compared to an equivalent lookAt matrix") {
      glm::mat4 lookAtMatrix =
          glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f),
                      // Looking down the positive x axis
                      glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

      THEN("It should be identical") {
        REQUIRE(mat4Equivalent(view, lookAtMatrix));
      }
    }
  }
}
