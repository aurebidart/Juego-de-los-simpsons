
//Thanks www.pngwing.com for png´s


#include <ctime>
#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "homer.h"
#include "Comida.h"
#include "Corazon.h"


using namespace sf;

int main() {

    int atrapados = 0;
    int vidas = 3;
    int mas_bajo = 0;
    int cantidad = 0;
    int nivel = 0;

    srand(time(NULL)); //Funcion para generar numeros random

    // Create the main window
    RenderWindow window(VideoMode(1200, 540), "RUSH eating");


    // Load a sprite to display
    Texture tex_fondo;
    if (!tex_fondo.loadFromFile("sala.png"))
        return EXIT_FAILURE;

    Sprite sp_fondo(tex_fondo);



    // Load a sprite to display
    Texture tex_homer_buscando;
    if (!tex_homer_buscando.loadFromFile("homero buscando.png"))
        return EXIT_FAILURE;


    //Cargamos otra textura de homero
    Texture tex_homer_babeando;
    if (!tex_homer_babeando.loadFromFile("homero babeando.png"))
        return EXIT_FAILURE;


    //Cargamos el sprite de comida que hay que atrapar
    Texture tex_food;
    if (!tex_food.loadFromFile("hamburguesa.png"))
        return EXIT_FAILURE;


    //Cargamos el sprite de los corazones de la vida
    Texture tex_vida;
    if (!tex_vida.loadFromFile("Corazon.png"))
        return EXIT_FAILURE;

    homer homer(tex_homer_buscando);


    Corazon vida[3];

    for (int i = 0; i < 3; i++) {
        vida[i].x = tex_vida.getSize().x * i * 1.2;
        vida[i].y = 0;
        vida[i].setTexture(tex_vida);
    }

    // Create a graphical text to display
    Font font;
    if (!font.loadFromFile("arial.ttf"))
        return EXIT_FAILURE;

    Text score("Puntuacion: 0", font, 25);
    score.setPosition(tex_fondo.getSize().x+5, 50);

    // Load a music to play
    Music music;
    if (!music.openFromFile("intro.ogg"))
        return EXIT_FAILURE;

    music.setVolume(10);
    // Play the music
    music.play();



    //Seteo los fps para manejar los tiempos
    window.setFramerateLimit(60);



    // Start the game loop
    while (window.isOpen()) {

        if (vidas > 0) {
            nivel++;
            cantidad += 10;
            mas_bajo = 0;

            homer.vel++;


            Comida food[cantidad];

            for (int i = 0; i < cantidad; i++) {
                food[i].setTexture(tex_food);
                food[i].x = rand() % (tex_fondo.getSize().x - tex_food.getSize().x);
                food[i].y = i * -150;
                food[i].vel = 1 + (float) (nivel) / 5;
                food[i].sp.setPosition(food[i].x, food[i].y);
            }


            Text round("Nivel " + std::to_string(nivel), font, 25);
            round.setPosition(tex_fondo.getSize().x + 5, 0);


            while (vidas > 0 && mas_bajo != cantidad) {
                // Process events


                Event event;
                while (window.pollEvent(event)) {
                    // Close window: exit
                    if (event.type == Event::Closed) {
                        music.stop();
                        window.close();
                        vidas = 0;
                    }
                    if (event.type == Event::KeyPressed) {
                        switch (event.key.code) {
                            case Keyboard::Left:
                                homer.mover(0);
                                break;
                            case Keyboard::Right:
                                homer.mover(1);
                                break;
                        }
                    }
                }


                if (food[mas_bajo].x + tex_food.getSize().x > homer.x &&
                    food[mas_bajo].x < homer.x + tex_homer_buscando.getSize().x) {
                    homer.setTexture(tex_homer_babeando);
                    if (food[mas_bajo].y > tex_fondo.getSize().y - 100) {
                        atrapados++;
                        mas_bajo++;
                        score.setString("Puntuacion: " + std::to_string(atrapados));
                        score.setPosition(tex_fondo.getSize().x + 5, 50);
                    }
                } else {
                    homer.setTexture(tex_homer_buscando);
                    if (food[mas_bajo].y > tex_fondo.getSize().y - 98) {
                        vidas--;
                        mas_bajo++;
                    }
                }






                // Clear screen
                window.clear();
                // Draw the sprite
                window.draw(sp_fondo);
                // Draw the sprite
                homer.dibujar(window);

                for (int i = mas_bajo; i < cantidad; i++) {
                    food[i].dibujar(window);
                }

                for (int i = 0; i < vidas; i++) {
                    vida[i].dibujar(window);
                }

                window.draw(round);
                window.draw(score);



                // Update the window
                window.display();
            }
        }


        while (vidas == 0 && window.isOpen()) {

            Event event;
            while (window.pollEvent(event)) {
                // Close window: exit
                if (event.type == Event::Closed) {
                    music.stop();
                    window.close();
                }
            }

            Text text("GAME OVER", font, 60);
            text.setPosition(tex_fondo.getSize().x / 3, tex_fondo.getSize().y / 2);


            // Clear screen
            window.clear();
            // Draw the sprite
            window.draw(sp_fondo);
            //Ponemos Game Over
            window.draw(text);
            // Update the window
            window.display();


        }
    }

    return EXIT_SUCCESS;
}