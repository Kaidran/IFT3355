#pragma once

#include <vector>

#define GLM_FORCE_RADIANS
#define GLM_PRECISION_HIGHP_FLOAT
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"

// typedef pour matcher le type utiliser par GLM
typedef glm::float_t decimal;

#define PI (glm::pi<decimal>())
#ifndef INFINITY
#define INFINITY (1e300)
#endif
#define EPSILON 1e-5

