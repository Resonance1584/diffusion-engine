#pragma once

#include "../vendor/glm/glm.hpp"

namespace diffusion {
namespace camera {

class Camera {
private:
  glm::mat4 viewMatrix;

public:
  glm::mat4 getViewMatrix() { return viewMatrix; }
};
}
}
