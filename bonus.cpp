//
// Created by Marco on 7/10/2020.
//

#include "bonus.h"

Cafe::Cafe() {
    vel= 1.5;
    x = 150;
    y = 0;

}

int Cafe::comer(){
}

void Cafe::caer(){
}

void Cafe::setTexture(Texture &tex) {
    sp.setTexture(tex);
}

void Cafe::dibujar(RenderWindow &w) {
    y += vel;
    sp.setPosition(x, y);
    w.draw(sp);
}

