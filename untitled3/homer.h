//
// Created by aureb on 27/8/2020.
//

#ifndef UNTITLED3_HOMER_H
#define UNTITLED3_HOMER_H

#include <ctime>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

using namespace sf;


class homer {

public:
    float x;
    float y;
    float vel;
    Texture tex;
    Sprite sp;

    homer(Texture &tex);
    void mover (bool side);
    void setTexture(Texture &tex);
    void dibujar (RenderWindow &w);
};


#endif //UNTITLED3_HOMER_H
