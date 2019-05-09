#include "geometry.h"
#include "basicStructures.h"

using namespace glm;
using namespace std;

//function to find the closest root of a quadratic function
//t is the root parameter
decimal setClosestRoot(decimal t, decimal a, decimal b, decimal c)
{
    // computes discriminant
    auto delta = b * b - 4 * a*c;
    if (delta < 0) {} //no root, do nothing
    else if (delta == 0)
    {
        t = (-0.5 * b) / a;
    }
    else
    {
        // calculate both roots and keep closest positive one
        auto q = (b > 0) ?
            -0.5 * (b + sqrt(delta)) :
            -0.5 * (b - sqrt(delta));
        auto root1 = q / a;
        auto root2 = c / q;
        if (root1 < 0 && root2 < 0) {} // no positive root, do nothing
        else
        {
            if (root1 > 0 && root2 > 0)
                t = min(root1, root2);
            else
                t = max(root1, root2);
        }
    }
    return t;
}
bool Sphere::closestIntersection(Ray ray, Intersection& inter)
{
    // À COMPLÉTER
    
    // p is our origin, d is the normalized direction in worldspace
    vec3 p = ray.origin;
    vec3 d = ray.direction;
    
    // Geometry sphere : r = x^2 + y^2 + z^2
    // if a ray is outside the sphere, then it will collide with its surface
    // then we have that the coordinates of a point from the ray
    // |O + tD|^2 - r^2 = 0
    // We can write the aboves as f(x) = ax^2 + bx + c where
    // a = D^2, b = 2*O*D, C = O^2 - r^2
    // with r^2 = 1 for our unit sphere. Thus,
    auto a = (d.x*d.x) + (d.y*d.y) + (d.z*d.z);
    auto b = 2*(p.x*d.x + p.y*d.y + p.z*d.z);
    auto c = ((p.x * p.x) + (p.y * p.y) + (p.z * p.z)) - 1;
    
    // We can find x by computing the roots, given that there exists any.
    decimal t = INFINITY;

    t = setClosestRoot(t, a, b, c);
    if (t == INFINITY)
        return false;
    // update intersection data
    inter.material = material;
    inter.position = p + t * d;
    inter.normal  = inter.position;
    // calculating UV
    auto u = fmod(0.5f + 
        acos(inter.position.x / (sqrt(pow(inter.position.x, 2) + pow(inter.position.z, 2)))) / (2*pi())
        ,1);
    if (inter.position.z > 0)
        u = 1 - u;
    auto v = 1 - (acos(inter.position.y) / pi());
    inter.uv = vec2(u, v);
    return true;
}


bool Plane::closestIntersection(Ray ray, Intersection& inter)
{
    vec3 p = ray.origin;
    vec3 d = ray.direction;

    decimal minT = INFINITY, t;
    vec3 tempP;

    if(d.y != 0) {
        t = (0-p.y)/d.y;
        tempP = p + t*d;
        if(t > 0 && t < minT) {
            minT = t;
            inter.normal = vec3(0,1,0);
            if(dot(inter.normal,ray.direction)>0) inter.normal = -inter.normal;
            inter.uv = vec2(mod<decimal>(tempP.x,1), mod<decimal>(-tempP.z,1));
        }
    }
    inter.position = p + minT*d;
    inter.material = material;

    return minT!=INFINITY;
}

bool Cube::closestIntersection(Ray ray, Intersection& inter)
{
	// À COMPLÉTER
    // p is our origin, d is the normalized direction in our space
    vec3 p = ray.origin;
    vec3 d = ray.direction;

    // Geometry cube: 6 faces, 6 plans. Simplify the problem by forcing
    // an axis aligned cube which corresponds to an AABB. We then have
    // to simply verify for each plane where x,y,z +-= 1.
    // find param t
    decimal t[6] = {INFINITY,INFINITY,INFINITY,INFINITY,INFINITY,INFINITY};
    if (d.x != 0)
    {
        t[0] = (1 - p.x) / d.x;
        t[1] = -(1 + p.x) / d.x;
    }
    if (d.y != 0)
    {
        t[2] = (1 - p.y) / d.y;
        t[3] = -(1 + p.y) / d.y;
    }
    if (d.z != 0)
    {
        t[4] = (1 - p.z) / d.z;
        t[5] = -(1 + p.z) / d.z;
    }
    // compute plans from the above parameters
    vec3 plans[6];
    for (int i = 0; i < 6; i++)
    {
        plans[i] = p + t[i] * d;
    }
    // find smallest positive interaction that intersect inside the cube
    decimal tmin = INFINITY;
    vec3 tnorm;
    vec2 tUV;
    for (int i = 0; i < 6; i++)
    {
        // only verify if a positive plan was intersected
        if (t[i] == INFINITY || t[i] <= 0)
            continue;
        float sign = 1+(-2 * (i % 2));
        switch(i)
        {
            case 0:
            case 1:
                if (plans[i].y > -1 && plans[i].y < 1 && plans[i].z > -1 && plans[i].z < 1) {
                    if (t[i] < tmin) {
                        tmin = t[i];
                        tnorm = vec3(sign,0,0);
                        tUV = vec2((-sign*plans[i].z + 1) / 2, (-sign*plans[i].y + 1) / 2);
                    }
                }
                break;
            case 2:
            case 3:
                if (plans[i].x > -1 && plans[i].x < 1 && plans[i].z > -1 && plans[i].z < 1)
                    if (t[i] < tmin) {
                        tmin = t[i];
                        tnorm = vec3(0, sign, 0);
                        tUV = vec2((-sign*plans[i].x + 1) / 2, (-sign*plans[i].z + 1) / 2);
                    }
                break;
            case 4:
            case 5:
                if (plans[i].x > -1 && plans[i].x < 1 && plans[i].y > -1 && plans[i].y < 1)
                    if (t[i] < tmin) {
                        tmin = t[i];
                        tnorm = vec3(0, 0, sign);
                        tUV = vec2((sign*plans[i].x + 1) / 2, (sign*plans[i].y + 1) / 2);
                    }
                break;
            default:
                break;
        }
    }
    if (tmin == INFINITY)
        return false;
    inter.position = p + tmin * d;
    inter.normal = tnorm;
    inter.material = material;
    inter.uv = tUV;
	return true;
}

bool Cylinder::closestIntersection(Ray ray, Intersection& inter)
{
	// À COMPLÉTER
    vec3 p = ray.origin;
    vec3 d = ray.direction;

    // Geometry cylinder: the sides follow sphere-like patterns in 2-dimensions
    // where x^2 + z^2 = 1, while the other dimension is two circles at h/2
    
    // proceed like with the sphere for f(x) = ax^2 + bx + c
    auto a = d.x*d.x + d.z*d.z;
    auto b = 2 * (p.x*d.x + p.z*d.z);
    auto c = p.x*p.x + p.z*p.z - 1;

    // compute closest root
    decimal t = INFINITY;
    vec3 tnorm;
    t = setClosestRoot(t, a, b, c);
    vec2 tUV(0);
    // as with sphere, verify that it intersects within the cylinder space
    if (p.y + t * d.y >= -1 && p.y + t * d.y <= 1)
    {
        tnorm = p + t * d;
        tnorm.y = 0;
        tUV = vec2(fmod(atan2(-tnorm.z, tnorm.x) / (2 * pi()) + 0.5, 1), (p.y + t*d.y + 1) / 2);
    }
    else 
    {
        t = INFINITY;
    }
    // as with cubes, find the plans on the y plane and see if it intersects
    vec3 plans[2];
    decimal tp[2] = { INFINITY, INFINITY };
    if (d.y != 0)
    {
        tp[0] = (1 - p.y) / d.y;
        tp[1] = -(1 + p.y) / d.y;
    }
    for (int i = 0; i < 2; i++)
    {
        plans[i] = p + tp[i] * d;
        if ((tp[i] < t) && (tp[i] > 0) && (plans[i].x*plans[i].x + plans[i].z * plans[i].z <= 1))
        {
            t = tp[i];
            float sign = 1 - 2*i;
            tnorm = vec3(0, sign, 0);
            decimal u = fmod(atan2(-plans[i].z, plans[i].x) / (2 * pi()) + 0.5, 1);
            decimal v = sqrt(pow(plans[i].x, 2) + pow(plans[i].z, 2));
            tUV = vec2(u,v);
        }
    }
    if (t == INFINITY)
        return false;
    inter.position = p + t * d ;
    inter.normal = tnorm;
    inter.material = material;
    inter.uv = tUV;//vec2(fmod(atan2(-inter.position.z, inter.position.x) / (2 * pi()) + 0.5, 1), (inter.position.y + 1)/2);
	return true;
    
}

bool Cone::closestIntersection(Ray ray, Intersection& inter)
{
	// À COMPLÉTER
    vec3 p = ray.origin;
    vec3 d = ray.direction;

    // similar to the cylinder, except that it varies in respect to theta where
    // x^2 + z^2 = 1 is now x^2 + z^2 = y^2 tan(theta)

    decimal a = d.x*d.x + d.z*d.z - 0.25*(d.y*d.y);
    decimal b = 2*(d.x*p.x + d.z*p.z) + 0.5*(d.y * (1-p.y));
    decimal c = p.x*p.x + p.z*p.z - 0.25*((p.y-1) * (p.y-1));

    decimal t = INFINITY;
    vec3 tnorm;
    vec2 tUV;
    t = setClosestRoot(t, a, b, c);

    //verify that it intersects with the cone
    if (p.y + t * d.y >= -1 && p.y + t * d.y <= 1)
    {
        //compute cone norm (currently cylinder copy pasta)
        vec3 p2 = p + t * d;
        auto r = sqrt(p2.x * p2.x + p2.z * p2.z);
        auto R = sqrt(p2.x * p2.x + p2.z * p2.z + p.y * p.y);
        if (R != 0)
        {
            tnorm.x = 2*p2.x / R ;
            tnorm.y = r / R;
            tnorm.z = 2*p2.z / R;
            tUV = vec2(fmod(atan2(p2.x, p2.z) / (2 * pi()) + 0.5, 1), (p2.y + 1) / 2);
        }
    }
    else
    {
        t = INFINITY;
    }
    // only have one plan instead of two
    vec3 plan;
    decimal tp = INFINITY;

    if (d.y != 0)
    {
        tp = -(1 + p.y) / d.y;
    }
    plan = p + tp * d;
    if (plan.x * plan.x + plan.z * plan.z <= 1 && tp < t && tp > 0)
    {
        t = tp;
        tnorm = vec3(0, -1, 0);
        decimal u = fmod(atan2(-plan.z, plan.x) / (2 * pi()) + 0.5, 1);
        decimal v = sqrt(pow(plan.x, 2) + pow(plan.z, 2));
        tUV = vec2(u, v);
    }
    if (t == INFINITY)
        return false;
    inter.position = p + t * d;
    inter.material = material;
    inter.normal = tnorm;
    decimal radius = sqrt(inter.position.x * inter.position.x + inter.position.z * inter.position.z);
    inter.uv = tUV;
	return true;
}
