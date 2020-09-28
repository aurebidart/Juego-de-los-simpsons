//
// Created by aureb on 29/8/2020.
//

#include "Corazon.h"


Corazon::Corazon(){

}
void Corazon::setTexture(Texture &tex){
    sp.setPosition(x, y);
    sp.setTexture(tex);
}

void Corazon::dibujar(RenderWindow &w){
    w.draw(sp);
}