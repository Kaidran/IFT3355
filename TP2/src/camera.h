#pragma once
#include "main.h"

class Camera
{
public:
    glm::vec3 eye;
    glm::vec3 lookAt;
    glm::vec3 up;
    glm::vec3 right;
    decimal fov; // angle de vue total horizontal, en radians
    decimal imageDist; // distance entre l'oeil et le plan image

    Camera() {}
};

