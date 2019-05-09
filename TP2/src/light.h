#pragma once
#include "main.h"

class Light
{
public:
	glm::vec3 color; // la couleur inclus l'intensité de la lumière
    bool castsShadows = true;
    
	// retourne la direction (vecteur unitaire) de p vers la lumière.
    virtual glm::vec3 normalizedVectorToLight(glm::vec3 p) = 0;
    
	// retourne le facteur multiplicatif d'atténuation, causé par e.g. la distance de p à la lumière.
    virtual decimal attenuationFactor(glm::vec3 p) = 0;

	// retourne la distance entre p et la lumière.
    virtual decimal distanceToLight(glm::vec3 p) = 0;
};

class PointLight : public Light
{
public:
    glm::vec3 position;
    
    PointLight() {}
    glm::vec3 normalizedVectorToLight(glm::vec3 p);
    decimal attenuationFactor(glm::vec3 p);
    decimal distanceToLight(glm::vec3 p);
};

class DirectionalLight : public Light
{
public:
    glm::vec3 direction;
    
    DirectionalLight() {}
    glm::vec3 normalizedVectorToLight(glm::vec3 p);
    decimal attenuationFactor(glm::vec3 p);
    decimal distanceToLight(glm::vec3 p);
};

class SpotLight : public Light
{
public:
    glm::vec3 position;
    glm::vec3 direction;
    decimal maxAngle, falloffExponent;
    
    SpotLight() {}
    glm::vec3 normalizedVectorToLight(glm::vec3 p);
    decimal attenuationFactor(glm::vec3 p);
    decimal distanceToLight(glm::vec3 p);
};





