//
// Created by aureb on 27/8/2020.
//

#include "homer.h"


homer::homer(Texture &tex){
    vel = 4;
    x = 420;
    y = 180;
    sp.setTexture(tex);
}

void homer::mover(bool side) {
    if(side){
        x += vel;
        if(x > 960 - sp.getTexture()->getSize().x){
            x = 960 - sp.getTexture()->getSize().x;
        }
    }
    else{
        x -= vel;
        if(x < 0){
            x = 0;
        }
    }
}

void homer::dibujar(RenderWindow &w) {
    sp.setPosition(x, y);
    w.draw(sp);
}

void homer::setTexture(Texture &tex) {
    sp.setTexture(tex);
}



