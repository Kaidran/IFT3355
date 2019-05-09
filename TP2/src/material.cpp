#include "main.h"
#include "material.h"
#include "scene.h"
#include <iostream>

using namespace glm;
using namespace std;

vec3 MaterialUniform::shade(Intersection, Scene&)
{
    return color;
}


vec3 MaterialLambert::shade(Intersection inter, Scene& scene)
{
	// À COMPLÉTER
    vec3 tlight = vec3(0);

    for (auto light : scene.lights) {

        auto lgt_n = light->normalizedVectorToLight(inter.position);
        auto lgt_af = light->attenuationFactor(inter.position);
        bool interact = true;
        // case with shades
        if (light->castsShadows) {
            // take a step in direction of the light vector due to shades
            vec3 p_dp = inter.position + (EPSILON * lgt_n);
            Intersection inter_shade;
            Ray ray2(p_dp, lgt_n, 1);
            // show lightning only if it doesn't intersect an object due to it's shadows
            if (!(scene.getClosestIntersection(ray2, inter_shade) == INFINITY))
            {
                interact = false;
            }
        }
        if (interact)
        {
            tlight += fmax(dot(inter.normal, lgt_n), 0)*albedo*(light->color*lgt_af);
        }
    }

    return tlight;
}


vec3 MaterialLambertTextured::shade(Intersection inter, Scene& scene)
{
	// À COMPLÉTER
    vec3 tlight = vec3(0);

    for (auto light : scene.lights) {

        auto lgt_n = light->normalizedVectorToLight(inter.position);
        auto lgt_af = light->attenuationFactor(inter.position);
        auto at_color = albedoTexture->getColor(inter.uv);
        bool interact = true;
        //case with shades
        if (light->castsShadows) {
            // take a step in direction of the light vector due to shades
            vec3 p_dp = inter.position + (EPSILON * lgt_n);
            Intersection inter_shade;
            Ray ray2(p_dp, lgt_n, 1);
            // show lightning only if it doesn't intersect an object due to it's shadows
            if (!(scene.getClosestIntersection(ray2, inter_shade) == INFINITY) )
            {
                interact = false;
            }
        }
        if (interact)
        {
            tlight += fmax(dot(lgt_n, inter.normal), 0)*at_color*(light->color*lgt_af);
        }
    }

    return tlight;
}


vec3 MaterialPhong::shade(Intersection inter, Scene& scene)
{
	// À COMPLÉTER
    // Quite similar to materialLambert, difference being that we have two albedo to work with
    vec3 tlight = vec3(0);

    for (auto light : scene.lights) {

        auto lgt_n = light->normalizedVectorToLight(inter.position);
        auto lgt_af = light->attenuationFactor(inter.position);
        bool interact = true;
        //case with shades
        if (light->castsShadows) {

            vec3 p_dx = inter.position - inter.incidentRay.direction * EPSILON;
            Intersection inter_shade;
            Ray ray2(p_dx, lgt_n, 1);

            // show lightning only if it doesn't intersect an object
            if (!(scene.getClosestIntersection(ray2, inter_shade) == INFINITY)) {
                interact = false;
            }
        }
        if (interact)
        {
            // add diffused and spec lights
            tlight += clamp(dot(lgt_n, inter.normal), 0.0, 1.0)*diffuseAlbedo*light->color*lgt_af;
            vec3 gd = normalize(lgt_n - inter.incidentRay.direction);
            tlight += pow(dot(gd, inter.normal), shininess) * specularAlbedo * light->color*lgt_af;
        }
    }
    // Ambiant light is always added regardless of shade
    tlight += ambientColor;

    return tlight;
}


vec3 MaterialReflective::shade(Intersection inter, Scene &scene)
{
	// À COMPLÉTER
    // if we have reached max recursiveness, return black color
    if (inter.incidentRay.recursionLevel >= scene.maxRecursionLevel)
      return vec3(0);

    vec3 tlight = vec3(0);
    vec3 r_dir = inter.incidentRay.direction;
    vec3 r_ray = reflect(r_dir, inter.normal);
    vec3 p_dx = -r_dir * EPSILON;
    Ray ray2(inter.position + p_dx, r_ray, inter.incidentRay.recursionLevel + 1);

    tlight += reflectiveness * scene.getIntersectionColor(ray2);
    tlight += (1 - reflectiveness)*ambientColor;
	return tlight;
}


vec3 MaterialRefractive::shade(Intersection inter, Scene &scene)
{
	// À COMPLÉTER
	vec3 tlight = vec3(0);
    if (inter.incidentRay.recursionLevel >= scene.maxRecursionLevel)
        return tlight;
    vec3 r_dir = inter.incidentRay.direction;
    vec3 r_ray = refract(r_dir, inter.normal, inter.incidentRay.isInsideGeometry ? refractiveIndex : 1.0 / refractiveIndex);
    //check if reflected or refracted
    if (r_ray == vec3(0))
    {
        r_ray = reflect(r_dir, inter.normal); // this is bugged
    }
    vec3 p_dx = r_ray * EPSILON;
    Ray ray2(inter.position+p_dx, r_ray, inter.incidentRay.recursionLevel+1);

    tlight += refractiveness * scene.getIntersectionColor(ray2);
    tlight += (1 - refractiveness)*ambientColor;
    return tlight;
}