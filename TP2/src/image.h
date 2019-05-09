#pragma once
#include "main.h"
#include <string>

// Importe/exporte les images au format png à l'aide de la librairie lodepng.
class Image
{
private:
	// en mémoire, le pixel 0 est en haut a gauche, puis on va vers la droite 
	// pour couvrir la rangée du haut, et on descend rangée par rangée.
	// data utilise 4 channels (r,g,b,alpha), chaque channel est un byte dans [0,255]
	std::vector<unsigned char> data;
    unsigned int nbPixelsX, nbPixelsY;
    
public:
	// crée une image vide avec les dimensions données
    Image(unsigned int width, unsigned int height);
	// charge une image à partir d'un fichier png.
    Image(std::string filename);
    
	// lorsqu'on utilise les pixels, le pixel 0 est en bas à gauche, puis on va
	// vers la droite pour couvrir la rangée du bas, puis on monte rangée par rangée.
	// on utilise 3 channels (r,g,b), chacun est un float dans [0,1].
    glm::vec3 get(unsigned int x, unsigned int y);
    void set(unsigned int x, unsigned int y, glm::vec3 color);
    void saveToFile(std::string filename);
    unsigned int getNbPixelsX() {return nbPixelsX;}
    unsigned int getNbPixelsY() {return nbPixelsY;}
};




