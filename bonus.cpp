//
// Created by Marco on 7/10/2020.
//

#include "bonus.h"

Bonus::Bonus(){
    vel= 1.5;
    x = 150;
    y = 0;

}

int Bonus::comer(){
}

void Bonus::caer(){
}

void Bonus::setTexture(Texture &tex) {
    sp.setTexture(tex);
}

void Bonus::dibujar(RenderWindow &w) {
    y += vel;
    sp.setPosition(x, y);
    w.draw(sp);
}

