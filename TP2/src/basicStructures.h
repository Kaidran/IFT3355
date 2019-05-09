#pragma once
#include "main.h"

// forward declarations
class Material;
class Scene;

class Ray
{
public:
    glm::vec3 origin;
	glm::vec3 direction; // longueur unitaire
    int recursionLevel; // commence � 0, incr�mente � chaque rebond.
	bool isInsideGeometry = false; // utile pour gérer les rayons réfractés au fil des rebonds
    
    Ray() :
        origin(glm::vec3(0)), direction(glm::vec3(0)), recursionLevel(0), isInsideGeometry(false) {}
    Ray(glm::vec3 origin, glm::vec3 direction, int recursionLevel) :
        origin(origin), direction(direction), recursionLevel(recursionLevel), isInsideGeometry(false) {}
};

class Intersection
{
public:
	Ray incidentRay;
	glm::vec3 position;
	glm::vec3 normal; // longueur unitaire
	glm::vec2 uv; // dans [0,1]x[0,1]
	Material* material;
    
    Intersection() :
        incidentRay(Ray()), position(glm::vec3(0)), normal(glm::vec3(0)), uv(glm::vec3(0)), material(NULL) {}
};





