//
// Created by Marco on 7/10/2020.
//

#ifndef UNTITLED3_BONUS_H
#define UNTITLED3_BONUS_H

#include <ctime>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

using namespace sf;

class Cafe {

public:
    float x;
    float y;
    float vel;
    Sprite sp;

    Cafe();
    void setTexture(Texture &tex);
    int comer();
    void caer();
    void dibujar (RenderWindow &w);



};


#endif //UNTITLED3_BONUS_H
