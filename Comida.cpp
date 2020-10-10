//
// Created by aureb on 28/8/2020.
//

#include "Comida.h"

Comida::Comida() {
    vel = 1.7;
    x = 150;
    y = 0;
}

int Comida::comer(){
}

void Comida::caer(){

}

void Comida::setTexture(Texture &tex){
    sp.setTexture(tex);
}


void Comida::dibujar(RenderWindow &w) {
    y += vel;
    sp.setPosition(x, y);
    w.draw(sp);
}