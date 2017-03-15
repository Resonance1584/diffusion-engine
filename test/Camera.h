#include "../lib/Camera.h"

#include "../vendor/catch.hpp"
#include "../vendor/glm/glm.hpp"

SCENARIO("Camera constructor", "[Camera]") {

  GIVEN("A default constructed Camera") {
    diffusion::camera::Camera myCamera;

    WHEN("getViewMatrix is called") {
      glm::mat4 view = myCamera.getViewMatrix();

      THEN("it should return an identity matrix") {
        glm::mat4 identity;

        REQUIRE(view == identity);
      }
    }
  }
}
