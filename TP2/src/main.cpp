    #include "main.h"
#include <iostream>

#include "basicStructures.h"
#include "scene.h"
#include "material.h"

using namespace glm;
using namespace std;


int main(int argc, const char* argv[])
{
	// parametres du programme
    string sceneFilename;
    string outputFilename;
    if(argc == 1) {
		// parametres hardcod�s
        //string sceneName = "uniformSphere";
        //string sceneName = "uniformPlaneSphere";
        //string sceneName = "uniformCube";
        //string sceneName = "uniformCylinder";
        //string sceneName = "uniformCone";
        //string sceneName = "allGeometries";
        //string sceneName = "allGeometriesTextured"; // not sure what rotation is required
        //string sceneName = "lambertSpheres1";
        //string sceneName = "lambertSpheres2"; 
        //string sceneName = "lambertSpheres3"; 
        //string sceneName = "lambertSpheres4"; 
        //string sceneName = "spotLights";
        //string sceneName = "shadowsOverGeometry";
        //string sceneName = "lambertShadows";
        //string sceneName = "shinySpheres1";
        //string sceneName = "shinySpheres2";
        //string sceneName = "reflection"; 
        //string sceneName = "reflectiveSpheres";
        //string sceneName = "refraction"; // BUGGED  - ray reflect
        //string sceneName = "refractiveSpheres"; 
        //string sceneName = "infinitePlane";
        //string sceneName = "infinitePlaneSupersampled";
        //string sceneName = "infinitePlaneSuperSampledJittered"; // bugged? lack of precision? not certain but slightly different
        //string sceneName = "textureBilinear"; // Bugged - bilinear same as normal
        sceneFilename  = "scenes/"+sceneName+"/"+sceneName+".scn";
        outputFilename = "output/"+sceneName+".png";
    } else {
		// arguments de la ligne de commande
        sceneFilename  = string(argv[1]);
        outputFilename = string(argv[2]);
    }

	cout << "Chargement de la scene... " << sceneFilename << " ." << endl;
	Scene scene(sceneFilename);
	cout << "Rendu de la scene..." << endl;
	scene.render();
	cout << "enregistrement de " << outputFilename << " ..." << endl;
	scene.saveToFile(outputFilename);
	cout << "Termine!" << endl;

	return 0;
}
