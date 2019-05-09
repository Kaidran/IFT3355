#pragma once

#include <map>
#include <sstream>

#include "main.h"
#include "basicStructures.h"
#include "geometry.h"
#include "camera.h"
#include "image.h"
#include "texture.h"

// forward declarations
class Light;

class Scene
{
public:
    Scene(std::string sceneFilename);

    Camera cam;
    int maxRecursionLevel;
    glm::vec3 backgroundColor;
    unsigned int supersampleLevel;
    bool jittered;

    std::vector<Geometry*> geometries;
    std::vector<Light*> lights;
    std::map<std::string, Texture*> textures;
    std::map<std::string, Material*> materials;
    
	// image a remplir, qui sera ensuite sauvegardée au format png.
    Image outputImage;

    void render();    
    void saveToFile(std::string filename);
    decimal getClosestIntersection(Ray ray, Intersection& closestIntersection);
    glm::vec3 getIntersectionColor(Ray ray);
};

