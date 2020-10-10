
#ifndef UNTITLED3_BASURA_H
#define UNTITLED3_BASURA_H

#include <ctime>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

using namespace sf;

class Basura {

public:
    float x;
    float y;
    float vel;
    Sprite sp;

    Basura();
    void setTexture(Texture &tex);
    int comer();
    void caer();
    void dibujar (RenderWindow &w);
};


#endif //UNTITLED3_BASURA_H
