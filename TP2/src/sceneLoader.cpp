#include <fstream>
#include <iostream>
#include <set>

#include "main.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform2.hpp"
#include "sceneLoader.h"
#include "material.h"

using namespace glm;
using namespace std;


SceneLoader::SceneLoader(Scene& scene, string sceneFilename):
    scene(scene)
{
    set<string> elementTypesEncountered;

    size_t pos = sceneFilename.find_last_of("/\\");
    if (pos==string::npos) {pos=sceneFilename.size();}
    string scenePath = sceneFilename.substr(0,pos);
    
    ifstream sceneFile(sceneFilename);
    if(!sceneFile.is_open()) {
        cout << "Cannot open scene file " << sceneFilename << endl;
        return;
    }
    while(getline(sceneFile, line)) {
        cout << "--------------------" << endl;
        
        auto backR = line.find('\r');
        if(backR != string::npos) {line.erase(backR);}

        if(line=="" || line[0]=='#') continue;
        lineStream.clear();
        lineStream.str(line);
        getline(lineStream, elementType, ' ');
        elementTypesEncountered.insert(elementType);
        
        params.clear();
        while(getNextParam()) {
            params.emplace(paramName, paramVal);
        }

//==============================================================================
        if(elementType=="material") {
            if(!allMandatoryParamsSpecified({"type", "name"})) {continue;}
            Material* mat = NULL;
            if(getParam("type")) {
                if(paramVal=="uniform") {
                    MaterialUniform* matUniform = new MaterialUniform();
                    if(getParam("color")) {
                        matUniform->color = stringToVec3(paramVal);
                    }
                    mat = matUniform;
                } else if(paramVal=="lambert") {
                    MaterialLambert* matLambert = new MaterialLambert();
                    if(getParam("albedo")) {
                        matLambert->albedo = stringToVec3(paramVal);
                    }
                    mat = matLambert;
                } else if(paramVal=="lambertTextured") {
                    if(!allMandatoryParamsSpecified({"texture"})) {continue;}
                    if(getParam("texture")) {
                        if(knownTexture()) {
                            mat = new MaterialLambertTextured(scene.textures.at(paramVal));
                        } else { unknownParameter(); delete mat; continue; }
                    }
                } else if(paramVal=="phong") {
                    MaterialPhong* matBF = new MaterialPhong();
                    if(getParam("diffuseAlbedo")) {
                        matBF->diffuseAlbedo = stringToVec3(paramVal);
                    }
                    if(getParam("specularAlbedo")) {
                        matBF->specularAlbedo = stringToVec3(paramVal);
                    }
                    if(getParam("ambientColor")) {
                        matBF->ambientColor = stringToVec3(paramVal);
                    }
                    if(getParam("shininess")) {
                        matBF->shininess = atof(paramVal.c_str());
                    }
                    mat = matBF;
                } else if(paramVal=="reflective") {
                    MaterialReflective* matRef = new MaterialReflective();
                    if(getParam("ambientColor")) {
                        matRef->ambientColor = stringToVec3(paramVal);
                    }
                    if(getParam("reflectiveness")) {
                        matRef->reflectiveness = atof(paramVal.c_str());
                    }
                    mat = matRef;
                } else if(paramVal=="refractive") {
                    MaterialRefractive* matRef = new MaterialRefractive();
                    if(getParam("ambientColor")) {
                        matRef->ambientColor = stringToVec3(paramVal);
                    }
                    if(getParam("refractiveness")) {
                        matRef->refractiveness = atof(paramVal.c_str());
                    }
                    if(getParam("refractiveIndex")) {
                        matRef->refractiveIndex = atof(paramVal.c_str());
                    }
                    mat = matRef;
                } else { unknownParameter(); delete mat; continue; }
            }
            if(getParam("name")) {
                mat->name = paramVal;
            }
            scene.materials.emplace(mat->name, mat);
        }
//==============================================================================
        else if(elementType=="texture") {
            if(!allMandatoryParamsSpecified({"type", "name"})) {continue;}
            Texture* tex = NULL;
            if(getParam("type")) {
                if(paramVal=="checkerboard") {
                    TextureCheckerboard* texChecker = new TextureCheckerboard();
                    if(getParam("color1")) {
                        texChecker->color1 = stringToVec3(paramVal);
                    }
                    if(getParam("color2")) {
                        texChecker->color2 = stringToVec3(paramVal);
                    }
                    if(getParam("nbSquaresX")) {
                        texChecker->nbSquaresX = atoi(paramVal.c_str());
                    }
                    if(getParam("nbSquaresY")) {
                        texChecker->nbSquaresY = atoi(paramVal.c_str());
                    }
                    tex = texChecker;
                } else if(paramVal=="image") {
                    if(!allMandatoryParamsSpecified({"filename"})) {continue;}
                    TextureImageFile* texImage;
                    if(getParam("filename")) {
                        texImage = new TextureImageFile(scenePath+"/"+paramVal);
                    }
                    if(getParam("bilinear")) {
                        texImage->bilinear = (paramVal=="true");
                    }
                    tex = texImage;
                } else { unknownParameter(); delete tex; continue; }
            }
            if(getParam("name")) {
                tex->name = paramVal;
            }
            scene.textures.emplace(tex->name, tex);
        }
//==============================================================================
        else if(elementType=="geometry") {
            if(!allMandatoryParamsSpecified({"type", "material"})) {continue;}
            Geometry* geo = NULL;
            if(getParam("type")) {
                if(paramVal=="sphere") {
                    geo = new Sphere();
                } else if(paramVal=="plane") {
                    geo = new Plane();
                } else if(paramVal=="cone") {
                    geo = new Cone();
                } else if(paramVal=="cube") {
                    geo = new Cube();
                } else if(paramVal=="cylinder") {
                    geo = new Cylinder();
                } else { unknownParameter(); delete geo; continue; }
            }
            if(getParam("material")) {
                if(knownMaterial()) {
                    geo->material = scene.materials.at(paramVal);
                } else { unknownParameter(); delete geo; continue; }
            }
            if(getParam("transform")) {
                geo->transform = transformStringToMat4(paramVal);
            }
            scene.geometries.push_back(geo);
        }
//==============================================================================
        else if(elementType=="light") {
            if(!allMandatoryParamsSpecified({"type", "color"})) {continue;}
            Light* light = NULL;
            if(getParam("type")) {
                if(paramVal=="point") {
                    if(!allMandatoryParamsSpecified({"position"})) {continue;}
                    PointLight* pointLight = new PointLight();
                    if(getParam("position")) {
                        pointLight->position = stringToVec3(paramVal);
                    }
                    light = pointLight;
                } else if(paramVal=="directional") {
                    if(!allMandatoryParamsSpecified({"direction"})) {continue;}
                    DirectionalLight* dirLight = new DirectionalLight();
                    if(getParam("direction")) {
                        dirLight->direction = stringToVec3(paramVal);
                    }
                    light = dirLight;
                } else if(paramVal=="spot") {
                    if(!allMandatoryParamsSpecified({"direction", "position"})) {continue;}
                    SpotLight* spotLight = new SpotLight();
                    if(getParam("position")) {
                        spotLight->position = stringToVec3(paramVal);
                    }
                    if(getParam("direction")) {
                        spotLight->direction = stringToVec3(paramVal);
                    }
                    if(getParam("maxAngle")) {
                        spotLight->maxAngle = atof(paramVal.c_str())*PI/180.0;
                    }
                    if(getParam("falloffExponent")) {
                        spotLight->falloffExponent = atof(paramVal.c_str());
                    }
                    light = spotLight;
                } else { unknownParameter(); delete light; continue; }
            }
            if(getParam("color")) {
                light->color = stringToVec3(paramVal);
            }
            if(getParam("shadows")) {
                if(paramVal=="true") {
                    light->castsShadows = true;
                } else if(paramVal=="false") {
                    light->castsShadows = false;
                } else { unknownParameter(); delete light; continue; }
            }
            scene.lights.push_back(light);
        }
//==============================================================================
        else if(elementType=="camera") {
            if(!allMandatoryParamsSpecified({"eye", "lookAt", "up", "fov"})) {continue;}
            if(getParam("eye")) {
                scene.cam.eye = stringToVec3(paramVal);
            }
            if(getParam("lookAt")) {
                scene.cam.lookAt = normalize(stringToVec3(paramVal));
            }
            if(getParam("up")) {
                scene.cam.up = normalize(stringToVec3(paramVal));
            }
            if(getParam("fov")) {
                scene.cam.fov = atof(paramVal.c_str());
            }
            if(getParam("imageDist")) {
                scene.cam.imageDist = atof(paramVal.c_str());
            }
            scene.cam.right = normalize(cross(scene.cam.lookAt, scene.cam.up));
            scene.cam.up = cross(scene.cam.right, scene.cam.lookAt);
        }
//==============================================================================
        else if(elementType=="scene") {
            if(!allMandatoryParamsSpecified({"maxRecursionLevel"})) {continue;}
            if(getParam("backgroundColor")) {
                scene.backgroundColor = stringToVec3(paramVal);
            }
            if(getParam("maxRecursionLevel")) {
                scene.maxRecursionLevel = atoi(paramVal.c_str());
            }
            if(getParam("supersampleLevel")) {
                scene.supersampleLevel = atoi(paramVal.c_str());
            }
            if(getParam("jittered")) {
                scene.jittered = (paramVal=="true");
            }
        }
//==============================================================================
        else if(elementType=="output") {
            if(!allMandatoryParamsSpecified({})) {continue;}
            if(getParam("imageSize")) {
                uvec2 size = stringToUvec2(paramVal);
                scene.outputImage = Image(size.x, size.y);
            }
        }
//==============================================================================

    }
    
    // verify mandatory elements
    if(elementTypesEncountered.find("camera") == elementTypesEncountered.end()) {
        cout << "Camera info not specified in scene file, expect unexpected behavior." << endl; }
    if(elementTypesEncountered.find("output") == elementTypesEncountered.end()) {
        cout << "Output info not specified in scene file, expect unexpected behavior." << endl; }
    
    sceneFile.close();
}



bool SceneLoader::getNextParam()
{
    if(!getline(lineStream, paramName, '=')) {return false;}
    if(!getline(lineStream, paramVal,  ' ')) {return false;}
    return true;
}

bool SceneLoader::allMandatoryParamsSpecified(vector<string> mandatoryParamNames)
{
    for(string paramName : mandatoryParamNames) {
        paramName = paramName;
        auto it = params.find(paramName);
        if(it == params.end()) {
            cout << "Missing mandatory parameter \"" << paramName <<
                    "\" for \"" << elementType << "\" element." << endl;
            return false;
        }
    }
    return true;
}

bool SceneLoader::getParam(string inParamName)
{
    this->paramName = inParamName;
    auto it = params.find(paramName);
    if(it != params.end()) {
        paramVal = it->second;
        return true;
    } else {
        cout << "Default value used for parameter \"" << paramName << "\" for \"" <<
                elementType << "\" element." << endl;
        return false;
    }
}

void SceneLoader:: unknownParameter()
{
    cout << "Unknown value \"" << paramVal << "\" for parameter \"" << paramName
         << "\" in \"" << elementType << "\" element." << endl;
}

bool SceneLoader::knownMaterial() {
    return (scene.materials.find(paramVal) != scene.materials.end());
}

bool SceneLoader::knownTexture() {
    return (scene.textures.find(paramVal) != scene.textures.end());
}

vec3 SceneLoader::stringToVec3(string str) {
    // str should have format (x,y,z)
    string errorMsg = "Error in vec3 argument.";
    stringstream ss(str);
    string word;
    float x, y, z;
    
    word = ss.get(); // (
    if(word != "(") {cout << errorMsg << endl; return vec3(0);}
    
    getline(ss, word, ','); // x
    if(ss.fail()) {cout << errorMsg << endl; return vec3(0);}
    x = (float) atof(word.c_str());
    
    getline(ss, word, ','); // y
    if(ss.fail()) {cout << errorMsg << endl; return vec3(0);}
    y = (float) atof(word.c_str());
    
    getline(ss, word, ')'); // z
    if(ss.fail()) {cout << errorMsg << endl; return vec3(0);}
    z = (float) atof(word.c_str());
    
    return vec3(x,y,z);
}

uvec2 SceneLoader::stringToUvec2(string str) {
    // str should have format (x,y)
    string errorMsg = "Error in uvec2 argument.";
    stringstream ss(str);
    string word;
    int x, y;
    
    word = ss.get(); // (
    if(word != "(") {cout << errorMsg << endl; return uvec2(0);}
    
    getline(ss, word, ','); // x
    if(ss.fail()) {cout << errorMsg << endl; return uvec2(0);}
    x = atoi(word.c_str());
    
    getline(ss, word, ')'); // y
    if(ss.fail()) {cout << errorMsg << endl; return uvec2(0);}
    y = atoi(word.c_str());
    
    return uvec2(x,y);
}

mat4 SceneLoader::transformStringToMat4(string str) {
    stringstream ss(str), ssMat;
    string mat, word;
    char temp[3];
    mat4 totalTransform = mat4();
    
    while(getline(ss, mat, '*')) {
        ssMat.clear();
        ssMat.str(mat);
        ssMat.get(temp, 2+1);
        word = temp;
        if(word == "tr") {
            getline(ssMat, word);
            vec3 vec = stringToVec3(word);
            totalTransform = totalTransform * glm::translate(mat4(), vec);
        } else if(word == "sh") {
            string errorMsg = "Error in shearing parameter.";
            ssMat.get(); // (
            getline(ssMat, word, ',');
            if(ssMat.fail()) { cout << errorMsg << endl; return mat4(); }
            string axis = word;
            getline(ssMat, word, ','); // arg1
            if(ssMat.fail()) { cout << errorMsg << endl; return mat4(); }
            decimal arg1 = atof(word.c_str());
            getline(ssMat, word, ')'); // arg2
            if(ssMat.fail()) { cout << errorMsg << endl; return mat4(); }
            decimal arg2 = atof(word.c_str());
            if(axis=="x") {
                totalTransform = totalTransform * glm::shearX3D(mat4(), arg1, arg2);
            } else if(axis=="y") {
                totalTransform = totalTransform * glm::shearY3D(mat4(), arg1, arg2);
            } else if(axis=="z") {
                totalTransform = totalTransform * glm::shearZ3D(mat4(), arg1, arg2);
            } else {
                cout << errorMsg << endl; return mat4();
            }
        } else if(word == "sc") {
            getline(ssMat, word);
            vec3 vec = stringToVec3(word);
            totalTransform = totalTransform * glm::scale(mat4(), vec);
        } else if(word == "ro") {
            string errorMsg = "Error in rotation parameter.";
            ssMat.get(); // (
            getline(ssMat, word, ',');
            if(ssMat.fail()) { cout << errorMsg << endl; return mat4(); }
            string axis = word;
            getline(ssMat, word, ','); // angle
            if(ssMat.fail()) { cout << errorMsg << endl; return mat4(); }
            decimal angle = PI/180.f*atof(word.c_str());
            if(axis=="x") {
                totalTransform = totalTransform * glm::rotate(mat4(), angle, vec3(1,0,0));
            } else if(axis=="y") {
                totalTransform = totalTransform * glm::rotate(mat4(), angle, vec3(0,1,0));
            } else if(axis=="z") {
                totalTransform = totalTransform * glm::rotate(mat4(), angle, vec3(0,0,1));
            } else {
                cout << errorMsg << endl; return mat4();
            }
        } else {
            cout << "Unknown transformation type \"" << word << "\"." << endl; return mat4();
        }
    }
    return totalTransform;
}







