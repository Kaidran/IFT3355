#include <fstream>
#include <iostream>

#include "scene.h"
#include "sceneLoader.h"
#include "material.h"
#include "glm/gtc/random.hpp"

using namespace glm;
using namespace std;


Scene::Scene(string sceneFilename) :
    outputImage(0,0), supersampleLevel(1)
{
    SceneLoader sceneLoader(*this, sceneFilename);
}


void Scene::render()
{
    // pre-calcule les matrices de transformation inverses de chaque objet
    for(Geometry* geo : geometries) {
        geo->inverseTransform = glm::inverse(geo->transform);
    }

    decimal imageHalfWidth = cam.imageDist * tan((cam.fov*PI/180.f)/2.f);
    decimal imageHalfHeight = imageHalfWidth * float(outputImage.getNbPixelsY()) / float(outputImage.getNbPixelsX());
    
	//Pour chaque pixel, on détermine le centre du pixel puis on lance un rayon à travers le centre.
    for(unsigned int i=0; i<outputImage.getNbPixelsX(); i++) {
        for(unsigned int j=0; j<outputImage.getNbPixelsY(); j++) {
            vec3 color_tot = vec3(0);
            double dx = 0;
            double dy = 0;

            for (unsigned int k = 1; k <= 2 * supersampleLevel - 1; k += 2) {
                for (unsigned int l = 1; l <= 2 * supersampleLevel - 1; l += 2) {
                    if (jittered)
                    {
                        dx = 2 / (1+rand()) - 1;
                        dy = 2 / (1+rand()) - 1;
                    }
			        vec3 pixelCenter = cam.eye
								        + cam.imageDist*cam.lookAt
								        - imageHalfWidth * cam.right
								        - imageHalfHeight * cam.up
								        + (i + (0.5*(k + dx)) / supersampleLevel) / outputImage.getNbPixelsX() * 2 * imageHalfWidth * cam.right
								        + (j + (0.5*(l + dy)) / supersampleLevel) / outputImage.getNbPixelsY() * 2 * imageHalfHeight * cam.up;
                    Ray ray(cam.eye, normalize(pixelCenter-cam.eye), 0);
                    color_tot += getIntersectionColor(ray) / pow(supersampleLevel, 2);
                }
            }
            outputImage.set(i, j, color_tot);
        }
		//Output percentage of completion 
        if(i%10==0) {cout << 100.f*float(i)/(outputImage.getNbPixelsX()-1) << "%" << endl;}
    }
    
    
}


void Scene::saveToFile(string filename)
{
    outputImage.saveToFile(filename);
}


decimal Scene::getClosestIntersection(Ray ray, Intersection& closestIntersection) {
    Intersection tempInter;
    decimal minT = INFINITY;
    for(Geometry* geo : geometries) {

        // we want a ray in our objectspace
        Ray modelRay(vec3(geo->inverseTransform*vec4(ray.origin,1)), 
            normalize(vec3(geo->inverseTransform*vec4(ray.direction,0))), 
            ray.recursionLevel);
        if(geo->closestIntersection(modelRay, tempInter)) {
            ray.isInsideGeometry = dot(modelRay.direction, tempInter.normal) > 0;
            tempInter.incidentRay = ray;
            tempInter.position = vec3(geo->transform*vec4(tempInter.position,1));
            tempInter.normal = normalize(vec3(transpose(geo->inverseTransform) * vec4(tempInter.normal, 1)));
            if (ray.isInsideGeometry)
                tempInter.normal *= -1;

            decimal dist = length(tempInter.position-ray.origin);

            if(dist < minT) {
                minT = dist;
                closestIntersection = tempInter;
            }
        }
    }
    return minT;
}


vec3 Scene::getIntersectionColor(Ray ray)
{
    // intersect scene
    Intersection closestIntersection;
    decimal minT = getClosestIntersection(ray, closestIntersection);
    
    // compute material color
    if(minT < INFINITY) {
        return closestIntersection.material->shade(closestIntersection, *this);
    } else {
        return backgroundColor;
    }
}


