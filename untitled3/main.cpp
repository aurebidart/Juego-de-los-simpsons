
//Thanks www.pngwing.com for png´s


#include <ctime>
#include <iostream>
#include <fstream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include "homer.h"
#include "Comida.h"
#include "Corazon.h"
#include "Puntuaciones.h"

using namespace sf;


void cargar_maximas(Puntuaciones maximas[10]) {
    ifstream ifs;
    ifs.open("maximas.txt");
    for (int i = 0; i < 10 && !ifs.eof(); i++) {
        ifs >> maximas[i].puntuacion;
    }
    ifs.close();
}

void guardar_puntuacion(int puntos, Puntuaciones maximas[10]) {
    for (int i = 9; i >= 0; i--) {
        if (puntos > maximas[i].puntuacion) {
            for (int j = 0; j < i; j++) {
                maximas[j].puntuacion = maximas[j + 1].puntuacion;
            }
            maximas[i].puntuacion = puntos;
            break;
        }
    }
    ofstream ofs;
    ofs.open("maximas.txt");
    for (int i = 0; i < 10; i++) {
        ofs << maximas[i].puntuacion << " ";
    }
    ofs.close();
}


int main() {

    int atrapados = 0;
    int vidas = 0;
    int mas_bajo = 0;
    int cantidad = 0;
    int nivel = 0;
    bool moviendo_derecha = false;
    int comida = 0;
    int mouse[3];
    char tecla = ' ';
    String momento = "menu";
    Puntuaciones maximas[10];

    srand(time(NULL)); //Funcion para generar numeros random

    // Create the main window
    RenderWindow window(VideoMode(1200, 540), "RUSH eating");


    // Load a sprite to display
    Texture tex_fondo;
    if (!tex_fondo.loadFromFile("sala.png"))
        return EXIT_FAILURE;

    Sprite sp_fondo(tex_fondo);


    Texture tex_replay[2];
    if (!tex_replay[0].loadFromFile("boton replay.png"))
        return EXIT_FAILURE;
    if (!tex_replay[1].loadFromFile("boton replay seleccionado.png"))
        return EXIT_FAILURE;
    Texture tex_new_game[2];
    if (!tex_new_game[0].loadFromFile("boton nuevo juego.png"))
        return EXIT_FAILURE;
    if (!tex_new_game[1].loadFromFile("boton nuevo juego seleccionado.png"))
        return EXIT_FAILURE;
    Texture tex_exit[2];
    if (!tex_exit[0].loadFromFile("boton exit.png"))
        return EXIT_FAILURE;
    if (!tex_exit[1].loadFromFile("boton exit seleccionado.png"))
        return EXIT_FAILURE;


    Sprite sp_replay(tex_replay[0]);
    Sprite sp_new_game(tex_new_game[0]);
    Sprite sp_exit(tex_exit[0]);

    // Load a sprite to display
    Texture tex_homer_buscando[2];
    if (!tex_homer_buscando[0].loadFromFile("homero buscando izquierda.png"))
        return EXIT_FAILURE;
    if (!tex_homer_buscando[1].loadFromFile("homero buscando derecha.png"))
        return EXIT_FAILURE;


    //Cargamos otra textura de homero
    Texture tex_homer_babeando[2];
    if (!tex_homer_babeando[0].loadFromFile("homero babeando izquierda.png"))
        return EXIT_FAILURE;
    if (!tex_homer_babeando[1].loadFromFile("homero babeando derecha.png"))
        return EXIT_FAILURE;


    //Cargamos el sprite de comida que hay que atrapar
    Texture tex_food[5];
    if (!tex_food[0].loadFromFile("dona.png"))
        return EXIT_FAILURE;
    if (!tex_food[1].loadFromFile("hamburguesa.png"))
        return EXIT_FAILURE;
    if (!tex_food[2].loadFromFile("pizza.png"))
        return EXIT_FAILURE;
    if (!tex_food[3].loadFromFile("pancho.png"))
        return EXIT_FAILURE;
    if (!tex_food[4].loadFromFile("sandwitch.png"))
        return EXIT_FAILURE;

    Texture bonus;
    if (!bonus.loadFromFile("cafe.png"))
        return EXIT_FAILURE;

    //Cargamos el sprite de los corazones de la vida
    Texture tex_vida;
    if (!tex_vida.loadFromFile("Corazon.png"))
        return EXIT_FAILURE;


    homer homer(tex_homer_buscando[0]);


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
    score.setPosition(tex_fondo.getSize().x + 5, 50);


    // Load a music to play
    Music intro;
    if (!intro.openFromFile("intro.ogg"))
        return EXIT_FAILURE;
    Music doh;
    if (!doh.openFromFile("doh.ogg"))
        return EXIT_FAILURE;

    intro.setVolume(5);


    //Seteo los fps para manejar los tiempos
    window.setFramerateLimit(60);
    Event event;


    // Start the game loop
    while (window.isOpen()) {

        if (momento == "menu") {


            // Play the music
            intro.play();
            cargar_maximas(maximas);
            Text max_scores("MAXIMAS\nPUNTUACIONES\n"
                            + std::to_string(maximas[9].puntuacion) + "\n"
                            + std::to_string(maximas[8].puntuacion) + "\n"
                            + std::to_string(maximas[7].puntuacion) + "\n"
                            + std::to_string(maximas[6].puntuacion) + "\n"
                            + std::to_string(maximas[5].puntuacion) + "\n"
                            + std::to_string(maximas[4].puntuacion) + "\n"
                            + std::to_string(maximas[3].puntuacion) + "\n"
                            + std::to_string(maximas[2].puntuacion) + "\n"
                            + std::to_string(maximas[1].puntuacion) + "\n"
                            + std::to_string(maximas[0].puntuacion) + "\n", font, 30);


            max_scores.setPosition(tex_fondo.getSize().x + 5, 20);
            sp_new_game.setPosition(230, 180);
            sp_exit.setPosition(600, 180);

            while (vidas == 0) {
                while (window.pollEvent(event)) {
                    // Close window: exit
                    switch (event.type) {

                        case Event::Closed:
                            intro.stop();
                            window.close();
                            momento = " ";
                            break;
                        case Event::MouseMoved:
                            mouse[0] = event.mouseMove.x;
                            mouse[1] = event.mouseMove.y;
                            break;
                        case Event::MouseButtonPressed:
                            mouse[2] = 1;
                            break;
                        case Event::MouseButtonReleased:
                            mouse[2] = 0;
                            break;
                    }
                }

                if (mouse[0] > sp_new_game.getPosition().x &&
                    mouse[0] < sp_new_game.getPosition().x + tex_new_game[0].getSize().x &&
                    mouse[1] > sp_new_game.getPosition().y &&
                    mouse[1] < sp_new_game.getPosition().y + tex_new_game[0].getSize().y) {
                    sp_new_game.setTexture(tex_new_game[1]);
                    if (mouse[2] == 1) {
                        vidas = 3;
                        homer.y = 540.0 - (tex_homer_buscando[0].getSize().y);
                        momento = "jugar";
                        intro.stop();
                    }
                } else {
                    sp_new_game.setTexture(tex_new_game[0]);
                }
                if (mouse[0] > sp_exit.getPosition().x &&
                    mouse[0] < sp_exit.getPosition().x + tex_exit[0].getSize().x &&
                    mouse[1] > sp_exit.getPosition().y &&
                    mouse[1] < sp_exit.getPosition().y + tex_exit[0].getSize().y) {
                    sp_exit.setTexture(tex_exit[1]);
                    if (mouse[2] == 1) {
                        intro.stop();
                        momento = " ";
                    }
                } else {
                    sp_exit.setTexture(tex_exit[0]);
                }

                if (mouse[0] < homer.x + tex_homer_buscando[0].getSize().x / 2) {
                    homer.setTexture(tex_homer_buscando[0]);
                } else {
                    homer.setTexture(tex_homer_buscando[1]);
                }


                // Clear screen
                window.clear();
                // Draw the sprite
                window.draw(sp_fondo);
                // Draw the sprite
                window.draw(sp_new_game);
                // Draw the sprite
                window.draw(sp_exit);
                window.draw(max_scores);
                homer.dibujar(window);
                // Update the window
                window.display();
            }
        }

        if (momento == "jugar") {
            nivel++;
            cantidad += 10;
            mas_bajo = 0;
            homer.vel++;
            Comida food[cantidad];

            for (int i = 0; i < cantidad; i++) {
                comida = rand() % 5;
                food[i].setTexture(tex_food[comida]);
                food[i].x = rand() % (tex_fondo.getSize().x - tex_food[comida].getSize().x);
                food[i].y = i * -150;
                food[i].vel *= 1.3;
                food[i].sp.setPosition(food[i].x, food[i].y);
            }


            Text round("Nivel " + std::to_string(nivel), font, 25);
            round.setPosition(tex_fondo.getSize().x + 5, 0);


            while (vidas > 0 && mas_bajo != cantidad) {
                // Process events
                while (window.pollEvent(event)) {
                    // Close window: exit
                    if (event.type == Event::Closed) {
                        momento = " ";
                    } else if (Keyboard::isKeyPressed(Keyboard::A)) {
                        tecla = 'a';
                    } else if (Keyboard::isKeyPressed(Keyboard::D)) {
                        tecla = 'd';
                    } else {
                        tecla = ' ';
                    }
                }

                switch (tecla) {
                    case 'a':
                        moviendo_derecha = false;
                        homer.mover(moviendo_derecha);
                        break;
                    case 'd':
                        moviendo_derecha = true;
                        homer.mover(moviendo_derecha);
                        break;
                }

                if (food[mas_bajo].x + tex_food[0].getSize().x > homer.x &&
                    food[mas_bajo].x < homer.x + tex_homer_buscando[0].getSize().x) {
                    homer.setTexture(tex_homer_babeando[moviendo_derecha]);
                    if (food[mas_bajo].y > tex_fondo.getSize().y - 100) {
                        atrapados++;
                        mas_bajo++;
                        score.setString("Puntuacion: " + std::to_string(atrapados));
                        score.setPosition(tex_fondo.getSize().x + 5, 50);
                    }
                } else {
                    homer.setTexture(tex_homer_buscando[moviendo_derecha]);
                    if (food[mas_bajo].y > tex_fondo.getSize().y - 98) {
                        doh.play();
                        vidas--;
                        mas_bajo++;
                        if (vidas == 0) {
                            momento = "game over";
                        }
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


        if (momento == "game over") {

            guardar_puntuacion(atrapados, maximas);

            Text text("GAME OVER", font, 60);
            text.setPosition(tex_fondo.getSize().x / 3, tex_fondo.getSize().y / 2);
            sp_replay.setPosition(230, 180);
            sp_exit.setPosition(600, 180);

            for (int i = 0; i < 3; i++) {
                mouse[i] = 0;
            }
            while (momento == "game over") {
                while (window.pollEvent(event)) {
                    // Close window: exit
                    switch (event.type) {

                        case Event::Closed:
                            intro.stop();
                            window.close();
                            momento = " ";
                            break;
                        case Event::MouseMoved:
                            mouse[0] = event.mouseMove.x;
                            mouse[1] = event.mouseMove.y;
                            break;
                        case Event::MouseButtonPressed:
                            mouse[2] = 1;
                            break;
                        case Event::MouseButtonReleased:
                            mouse[2] = 0;
                            break;
                    }
                }

                if (mouse[0] > sp_replay.getPosition().x &&
                    mouse[0] < sp_replay.getPosition().x + tex_replay[0].getSize().x &&
                    mouse[1] > sp_replay.getPosition().y &&
                    mouse[1] < sp_replay.getPosition().y + tex_replay[0].getSize().y) {
                    sp_replay.setTexture(tex_replay[1]);
                    if (mouse[2] == 1) {
                        vidas = 3;
                        homer.y = 540.0 - (tex_homer_buscando[0].getSize().y);
                        momento = "jugar";
                    }
                } else {
                    sp_replay.setTexture(tex_replay[0]);
                }
                if (mouse[0] > sp_exit.getPosition().x &&
                    mouse[0] < sp_exit.getPosition().x + tex_exit[0].getSize().x &&
                    mouse[1] > sp_exit.getPosition().y &&
                    mouse[1] < sp_exit.getPosition().y + tex_exit[0].getSize().y) {
                    sp_exit.setTexture(tex_exit[1]);
                    if (mouse[2] == 1) {
                        momento = " ";
                    }
                } else {
                    sp_exit.setTexture(tex_exit[0]);
                }


                // Clear screen
                window.clear();
                // Draw the sprite
                window.draw(sp_fondo);
                //Ponemos Game Over
                window.draw(text);
                window.draw(sp_replay);
                window.draw(sp_exit);
                // Update the window
                window.display();

            }
        }

        if (momento == " ") {
            window.close();
        }
    }

    return EXIT_SUCCESS;
}