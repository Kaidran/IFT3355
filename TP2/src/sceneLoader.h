#pragma once

#include <map>
#include <sstream>

#include "main.h"
#include "scene.h"

// ouvre un fichier .scn, et transfert les information dans l'object scene
class SceneLoader
{
private:
    Scene& scene;
    std::string line, paramName, paramVal, elementType;
    std::stringstream lineStream;
    std::map<std::string,std::string> params;
public:
    SceneLoader(Scene& scene, std::string sceneFilename);
    bool getNextParam();
    bool allMandatoryParamsSpecified(std::vector<std::string> mandatoryParamNames);
    bool getParam(std::string inParamName);
    void unknownParameter();
    bool knownMaterial();
    bool knownTexture();
    glm::vec3 stringToVec3(std::string str); // (x,y,z) -> vec3
    glm::uvec2 stringToUvec2(std::string str); // (x,y) -> uvec2
    glm::mat4 transformStringToMat4(std::string str);
};
