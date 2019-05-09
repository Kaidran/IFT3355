#include "light.h"

using namespace glm;
using namespace std;

vec3 PointLight::normalizedVectorToLight(vec3 p) {
	// � COMPL�TER
    return normalize(position - p);
}


decimal PointLight::attenuationFactor(vec3 p) {
	// � COMPL�TER
    if (distanceToLight(p) == 0)
        return 0;
    return 1.0 / pow(distanceToLight(p),2);
}


decimal PointLight::distanceToLight(vec3 p) {
	// � COMPL�TER
    return length(position - p);
}


vec3 DirectionalLight::normalizedVectorToLight(vec3 p) {
	// � COMPL�TER
    return normalize(-direction);
}


decimal DirectionalLight::attenuationFactor(vec3 p) {
	// � COMPL�TER
    return 1.0;
}


decimal DirectionalLight::distanceToLight(vec3 p) {
	// � COMPL�TER
    return INFINITY;
}


vec3 SpotLight::normalizedVectorToLight(vec3 p) {
	// � COMPL�TER
    return normalize(position - p);
}


decimal SpotLight::attenuationFactor(vec3 p) {
	// � COMPL�TER
    // clamp spotlight A
    decimal angle = clamp(acos(dot(normalizedVectorToLight(p), -direction) 
                         / length(normalizedVectorToLight(p))
                         * length(-direction)), 0.0, 180.0);

    if (angle > maxAngle) {
        return 0;
    }
    else 
    {
        decimal A = clamp(angle / maxAngle, 0.0, 0.999);
        decimal dl = 1.0 / pow((cos(A*pi() / 2)), falloffExponent);
        return 1.0 / (pow(distanceToLight(p), 2)*dl);
    }
}


decimal SpotLight::distanceToLight(vec3 p) {
	// � COMPL�TER
	return length(position - p);
}
