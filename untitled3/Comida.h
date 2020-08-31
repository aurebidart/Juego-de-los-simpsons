//
// Created by aureb on 28/8/2020.
//

#ifndef UNTITLED3_COMIDA_H
#define UNTITLED3_COMIDA_H

#include <ctime>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

using namespace sf;

class Comida {

public:
    float x;
    float y;
    float vel;
    Sprite sp;

    Comida();
    void setTexture(Texture &tex);
    int comer();
    void caer();
    void dibujar (RenderWindow &w);
};


#endif //UNTITLED3_COMIDA_H
