TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

DESTDIR = ./
OBJECTS_DIR = obj/

#unix {
    QMAKE_CXXFLAGS += -std=c++11
#}


INCLUDEPATH += \
    libs/glm \
    libs/lodepng

DEPENDPATH += \
    libs/glm \
    libs/lodepng


HEADERS += \
    libs/lodepng/lodepng.h \
    src/main.h \
    src/material.h \
    src/scene.h \
    src/texture.h \
    src/geometry.h \
    src/camera.h \
    src/light.h \
    src/sceneLoader.h \
    src/basicStructures.h \
    src/image.h
    

SOURCES += \
    libs/lodepng/lodepng.cpp \
    src/main.cpp \
    src/material.cpp \
    src/scene.cpp \
    src/texture.cpp \
    src/geometry.cpp \
    src/light.cpp \
    src/sceneLoader.cpp \
    src/image.cpp



DISTFILES += \
    scenes/allGeometries/allGeometries.scn \
    scenes/lambertSpheres1/lambertSpheres1.scn \
    scenes/lambertSpheres2/lambertSpheres2.scn \
    scenes/lambertSpheres3/lambertSpheres3.scn \
    scenes/lambertSpheres4/lambertSpheres4.scn \
    scenes/uniformSphere/uniformSphere.scn \
    scenes/uniformCube/uniformCube.scn \
    scenes/uniformCylinder/uniformCylinder.scn \
    scenes/uniformCone/uniformCone.scn \
    scenes/allGeometriesTextured/allGeometriesTextured.scn \
    scenes/shinySpheres1/shinySpheres1.scn \
    scenes/shinySpheres2/shinySpheres2.scn \
    scenes/infinitePlane/infinitePlane.scn \
    scenes/reflectiveSpheres1/reflectiveSpheres1.scn \
    scenes/refraction/refraction.scn \
    scenes/reflection/reflection.scn \
    scenes/everythingMaterial/everythingMaterial.scn \
    scenes/lambertShadows/lambertShadows.scn \
    scenes/shadowsOverGeometry/shadowsOverGeometry.scn \
    scenes/infinitePlaneSupersampled/infinitePlaneSupersampled.scn \
    scenes/infinitePlaneSupersampledJittered/infinitePlaneSupersampledJittered.scn \
    scenes/textureBilinear/textureBilinear.scn \
    scenes/spotLights/spotLights.scn

OTHER_FILES += \
    scenes/allGeometries/allGeometries.scn \
    scenes/lambertSpheres1/lambertSpheres1.scn \
    scenes/lambertSpheres2/lambertSpheres2.scn \
    scenes/lambertSpheres3/lambertSpheres3.scn \
    scenes/lambertSpheres4/lambertSpheres4.scn \
    scenes/uniformSphere/uniformSphere.scn \
    scenes/uniformCube/uniformCube.scn \
    scenes/uniformCylinder/uniformCylinder.scn \
    scenes/uniformCone/uniformCone.scn \
    scenes/allGeometriesTextured/allGeometriesTextured.scn \
    scenes/shinySpheres1/shinySpheres1.scn \
    scenes/shinySpheres2/shinySpheres2.scn \
    scenes/infinitePlane/infinitePlane.scn \
    scenes/reflectiveSpheres1/reflectiveSpheres1.scn \
    scenes/refraction/refraction.scn \
    scenes/reflection/reflection.scn \
    scenes/everythingMaterial/everythingMaterial.scn \
    scenes/lambertShadows/lambertShadows.scn \
    scenes/shadowsOverGeometry/shadowsOverGeometry.scn \
    scenes/infinitePlaneSupersampled/infinitePlaneSupersampled.scn \
    scenes/infinitePlaneSupersampledJittered/infinitePlaneSupersampledJittered.scn \
    scenes/textureBilinear/textureBilinear.scn \
    scenes/spotLights/spotLights.scn






