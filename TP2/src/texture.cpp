#include "main.h"
#include "texture.h"
#include <fstream>
#include <iostream>

using namespace glm;
using namespace std;

vec3 TextureCheckerboard::getColor(vec2 uv)
{
    uv = floor(uv * vec2(nbSquaresX, nbSquaresY));
    return (int(uv.x+uv.y) % 2) == 0 ? color1 : color2;
}


vec3 TextureImageFile::getColor(vec2 uv)
{
    auto w = (double)width();
    auto h = (double)height();
    auto u = uv.x * w;
    auto v = uv.y * h;
	if (bilinear) {
		// TODO
		// filtrage bilineaire
        // same as below but using average of 4 texels with respective weight based on distance duv
        auto du = u - floor(u);
        auto dv = v - floor(v);
        vec3 p00 = image.get(clamp(u, 0.0, w - 1), clamp(v, 0.0, h - 1));
        vec3 p10 = image.get(clamp(u+1, 0.0, w - 1), clamp(v, 0.0, h - 1));
        vec3 p01 = image.get(clamp(u, 0.0, w - 1), clamp(v+1, 0.0, h - 1));
        vec3 p11 = image.get(clamp(u+1, 0.0, w - 1), clamp(v+1, 0.0, h - 1));
        //double usn = 1.0;
        //it usnx = usn - floor(usn);
        return (p00 * (1 - du) * (1 - dv) + p10 * du*(1 - dv) + p01 * dv*(1 - du) + p11 * du*dv);
	}
	else {
		// TODO
		// faire correspondre les coordonnées UV dans [0,1]x[0,1] avec les pixels de l'image [0, image.getNbPixelsX()]x[0, image.getNbPixelsY()].
		// attention de ne pas demander de pixels de l'image qui n'existent pas!
		// on peut obtenir la valeur des pixels de l'image avec image.get(x, y).
        return image.get(clamp(u,0.0,w-1), clamp(v,0.0,h-1));

	}
	return vec3(0);
}
