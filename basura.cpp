
#include "basura.h"

Basura::Basura() {
    vel = 2;
    x = 150;
    y = 0;
}

int Basura::comer(){
}

void Basura::caer(){

}

void Basura::setTexture(Texture &tex){
    sp.setTexture(tex);
}


void Basura::dibujar(RenderWindow &w) {
    y += vel;
    sp.setPosition(x, y);
    w.draw(sp);
}