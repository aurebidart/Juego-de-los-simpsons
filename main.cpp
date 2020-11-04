
//Thanks www.pngwing.com for png´s


//Incluimos todas las librerias necesarias (usamos la lista que hicimos con el profe)
#include <queue>
#include <stack>
#include <ctime>
#include <iostream>
#include <fstream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include "LinkedList.h"
#include "homer.h"
#include "Comida.h"
#include "Corazon.h"
#include "Puntuaciones.h"
#include "basura.h"

using namespace sf;

//Funcion de cargar las puntuaciones maximas
//USA LECTURA DE ARCHIVOS
void cargar_maximas(Puntuaciones maximas[10]) {

    //Cargamos y abrimos el archivo
    ifstream ifs;
    ifs.open("maximas.txt");
    //Lo leemos y guardamos las puntuaciones en la memoria
    for (int i = 0; i < 10 && !ifs.eof(); i++) {
        ifs >> maximas[i].puntuacion;
    }
    //Cerramos el archivo
    ifs.close();
}

//Función de guardar la puntuacion maxima
//USA ESCRITURA DE ARCHIVOS
void guardar_puntuacion(int puntos, Puntuaciones maximas[10]) {
    //Recorremos las puntuaciones maximas y buscamos si superamos alguna
    for (int i = 9; i >= 0; i--) {
        if (puntos > maximas[i].puntuacion) {
            //En caso de que sea asi, reescribimos las puntuaciones que quedaron por debajo para que queden de mayor a menor
            for (int j = 0; j < i; j++) {
                maximas[j].puntuacion = maximas[j + 1].puntuacion;
            }
            //Reemplazamos el valor superado por el nuevo
            maximas[i].puntuacion = puntos;
            break;
        }
    }

    //Abrimos el archivo donde vamos a guardar las puntuaciones
    ofstream ofs;
    ofs.open("maximas.txt");

    //Las sobreescribimos una a una
    for (int i = 0; i < 10; i++) {
        ofs << maximas[i].puntuacion << " ";
    }
    //Cerramos el archivo
    ofs.close();
}


int main() {

    //Inicializacion de las variables que vamos a ir usando a lo largo del programa
    //(no estan las que se van a usar en momentos especificos)

    int atrapados = 0;              //Comida atrapada o puntos
    int vidas = 0;                  //Contador de vidas
    int nivel = 0;                  //Nivel de juego
    bool moviendo_derecha = false;  //Variable booleana usada para darle la direccion en la que mira homero
    int comida = 0;                 //Variable que vamos a randomizar para definir la textura de la comida
    int basura = 0;                 //Lo mismo que la anterior pero para la basura
    int cantidad = 0;               //Cantidad de comida en cada nivel
    int basura_cant = 0;            //Cantidad de basura en cada nivel
    int mouse[3];                   //Vector que usamos para leer las acciones del mouse. 0) eje x; 1) Eje y; 2) Boton izquierdo
    char tecla = ' ';               //Variable para leer la letra que presionamos
    String momento = "menu";        //Momento de juego, usado para darle un orden al juego (se inicia en menu, pasa a jugar y termina en game over)
    Puntuaciones maximas[10];       //Vector donde vamos a guardar las puntuaciones maximas

    srand(time(NULL)); //Funcion para generar numeros random

    // Crea la ventana principal
    RenderWindow window(VideoMode(1200, 540), "RUSH eating");


    // cargamos TODAS las texturas y vamos inicializando los objetos
    Texture tex_fondo;
    if (!tex_fondo.loadFromFile("sala.png"))
        return EXIT_FAILURE;

    Sprite sp_fondo(tex_fondo);


    //texturas de los botones
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

    //Texturas de homero
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

    //Texturas de basura
    Texture tex_trash[3];
    if (!tex_trash[0].loadFromFile("Lata roja.png"))
        return EXIT_FAILURE;
    if (!tex_trash[1].loadFromFile("Lata verde.png"))
        return EXIT_FAILURE;
    if (!tex_trash[2].loadFromFile("Manzana comida.png"))
        return EXIT_FAILURE;


    //Los corazones de la vida
    Texture tex_vida;
    if (!tex_vida.loadFromFile("Corazon.png"))
        return EXIT_FAILURE;


    //Inicializamos a homero
    homer homer(tex_homer_buscando[0]);

    //Creamos la pila de corazones
    stack<Corazon> vida;

    // Crea la fuente del texto que se va a mostrar
    Font font;
    if (!font.loadFromFile("arial.ttf"))
        return EXIT_FAILURE;

    Text score("Puntuacion: 0", font, 25);
    score.setPosition(tex_fondo.getSize().x + 5, 50);


    //Cargamos los sonidos
    SoundBuffer buf_intro;
    if (!buf_intro.loadFromFile("intro.ogg"))
        return EXIT_FAILURE;
    SoundBuffer buf_doh;
    if (!buf_doh.loadFromFile("doh.ogg"))
        return EXIT_FAILURE;
    SoundBuffer buf_ingame;
    if (!buf_ingame.loadFromFile("ingame.ogg"))
        return EXIT_FAILURE;
    SoundBuffer buf_woohoo;
    if (!buf_woohoo.loadFromFile("woohoo.ogg"))
        return EXIT_FAILURE;

    //Los ponemos en sus "Sprites"
    Sound intro;
    intro.setBuffer(buf_intro);
    Sound doh;
    doh.setBuffer(buf_doh);
    Sound ingame;
    ingame.setBuffer(buf_ingame);
    Sound woohoo;
    woohoo.setBuffer(buf_woohoo);

    intro.setVolume(4);
    ingame.setVolume(3);

    //Seteo los fps para manejar los tiempos
    window.setFramerateLimit(60);
    Event event;


    // Start the game loop
    while (window.isOpen()) {

        if (momento == "menu") {


            // Play the music
            intro.play();

            //Carga las puntuaciones maximas y las pone en el objeto tipo Text
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

            //Las ubica y tambien a los botones
            max_scores.setPosition(tex_fondo.getSize().x + 5, 20);
            sp_new_game.setPosition(230, 180);
            sp_exit.setPosition(600, 180);

            //Mientras no hayamos iniciado el juego...
            while (vidas == 0) {

                //Leemos las acciones del usuario
                while (window.pollEvent(event)) {
                    switch (event.type) {
                        //si cierra la ventana paramos todo_ salimos de los bucles y cerramos la ventana
                        case Event::Closed:
                            intro.stop();
                            vidas = -1;
                            window.close();
                            momento = " ";
                            break;

                        //Leemos la posicion del mouse
                        case Event::MouseMoved:
                            mouse[0] = event.mouseMove.x;
                            mouse[1] = event.mouseMove.y;
                            break;

                        //Leemos si apreta algun botón
                        case Event::MouseButtonPressed:
                            mouse[2] = 1;
                            break;
                        //Leemos cuando lo suelta
                        case Event::MouseButtonReleased:
                            mouse[2] = 0;
                            break;
                    }
                }

                //Si el mouse esta sobre el boton de "New game"
                if (mouse[0] > sp_new_game.getPosition().x &&
                    mouse[0] < sp_new_game.getPosition().x + tex_new_game[0].getSize().x &&
                    mouse[1] > sp_new_game.getPosition().y &&
                    mouse[1] < sp_new_game.getPosition().y + tex_new_game[0].getSize().y) {
                    //Le ponemos los bordes negros
                    sp_new_game.setTexture(tex_new_game[1]);

                    //Si presiona el mouse arranca el juego
                    if (mouse[2] == 1) {
                        vidas = 3;
                        homer.y = 540.0 - (tex_homer_buscando[0].getSize().y);
                        momento = "jugar";
                        intro.stop();
                        ingame.play();
                    }

                }
                //Si no esta sobre el boton de "New Game"
                else {
                    //Los bordes son amarillos
                    sp_new_game.setTexture(tex_new_game[0]);
                }

                //Si el mouse esta sobre el boton de "Exit"
                if (mouse[0] > sp_exit.getPosition().x &&
                    mouse[0] < sp_exit.getPosition().x + tex_exit[0].getSize().x &&
                    mouse[1] > sp_exit.getPosition().y &&
                    mouse[1] < sp_exit.getPosition().y + tex_exit[0].getSize().y) {
                    //Le ponemos los bordes negros
                    sp_exit.setTexture(tex_exit[1]);

                    //Si aprieta el mouse cerramos todo
                    if (mouse[2] == 1) {
                        intro.stop();
                        vidas = -1;
                        window.close();
                        momento = " ";
                    }
                }
                    //Si no esta sobre el boton de "Exit"
                else {
                    //Los bordes son amarillos
                    sp_exit.setTexture(tex_exit[0]);
                }

                //Miramos donde esta el mouse y alli va a mirar homero (detallito)
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
                // Draw the sprites
                window.draw(sp_exit);
                window.draw(max_scores);
                homer.dibujar(window);
                // Update the window
                window.display();
            }
        }

        //Si estamos jugando
        if (momento == "jugar") {

            //Si recien esta arrancando a jugar
            if (nivel == 0) {

                //Cargamos la pila de corazones (son 3 por partida)
                for (int i = 0; i < 3; i++) {
                    Corazon aux;
                    aux.x = tex_vida.getSize().x * i * 1.2;
                    aux.y = 0;
                    aux.setTexture(tex_vida);
                    vida.push(aux);
                }
            }
            //Si solo paso el nivel y no es que recien esta arrancado, festeja
            else{
                woohoo.play();
            }

            //Esto es lo que pasa cuando pasa de nivel y tambien cuando arrnaca (pasa del nivel 0 al 1)
            nivel++;

            //Caen mas cosas
            cantidad += 10;
            basura_cant += 9;

            //Homero va a ser mas rapido
            homer.vel++;

            //Creamos una nueva lista y cola de cosas ya que la del nivel anterior se destruyó
            LinkedList<Comida> food;
            queue<Basura> trash;
            score.setString("Puntuacion: " + std::to_string(atrapados));

            //Carga la lista de comida
            //Usa un objeto auxiliar que nos permite insertar los datos mas facil
            for (int i = 0; i < cantidad; i++) {
                Comida aux;
                comida = rand() % 5;
                aux.setTexture(tex_food[comida]);
                aux.x = rand() % (tex_fondo.getSize().x - tex_food[comida].getSize().x);
                aux.y = i * -200;
                aux.vel += nivel * 0.2 ;
                aux.sp.setPosition(aux.x, aux.y);
                food.insert(i, aux);
            }

            //Carga la cola de basura
            //Otra vez usamos el objeto auxiliar
            for (int i = 0; i < basura_cant; i++) {
                Basura aux;
                basura = rand() % 3;
                aux.setTexture(tex_trash[basura]);

                //Este bucle es para que la basura no apareza sobre la comida (pero solo funciona en los niveles faciles, obvio)
                do {
                    aux.x = rand() % (tex_fondo.getSize().x - tex_trash[basura].getSize().x);
                } while (aux.x + tex_trash[basura].getSize().x > food.get(i).x && aux.x < food.get(i).x + tex_food[0].getSize().x);
                aux.y = i * -200 + rand() % 150 - 75;
                aux.sp.setPosition(aux.x, aux.y);
                trash.push(aux);
            }


            //Carga y posicionamiento del texto de que muestra el nivel en que estamos
            Text round("Nivel " + std::to_string(nivel), font, 25);
            round.setPosition(tex_fondo.getSize().x + 5, 0);


            //Bucle de nivel, mientras tengas vidas y todavia haya comida por atrapar, seguis jugando
            while (vidas > 0 && !food.empty()) {
                // Process events
                while (window.pollEvent(event)) {
                    // Close window: exit
                    if (event.type == Event::Closed) {
                        ingame.stop();
                        vidas = -1;
                        window.close();
                        momento = " ";
                    }
                    //Lee que tecla se presiono
                    else if (Keyboard::isKeyPressed(Keyboard::A)) {
                        tecla = 'a';
                    } else if (Keyboard::isKeyPressed(Keyboard::D)) {
                        tecla = 'd';
                    } else {
                        tecla = ' ';
                    }
                }

                //Segun el movimiento, actualizamos la direccion en la que mira homero
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


                //Si homero esta debajo de la comida
                if (food.get(0).x + tex_food[0].getSize().x > homer.x &&
                    food.get(0).x < homer.x + tex_homer_buscando[0].getSize().x) {

                    //Ponemos a homero a babear
                    homer.setTexture(tex_homer_babeando[moviendo_derecha]);

                    //Si la comida llega hasta la boca de homero la atrapa y suma puntos. Tambien se borra la comida de la lista
                    if (food.get(0).y > tex_fondo.getSize().y - 100) {
                        atrapados++;
                        food.erase(0);
                        score.setString("Puntuacion: " + std::to_string(atrapados));
                    }
                }
                //Si homero no esta debajo de la comida
                else {

                    //La va a estar buscando
                    homer.setTexture(tex_homer_buscando[moviendo_derecha]);

                    //Y si se cae pierde vidas y se queja. Tambien se borra la comida de la lista y un corazon de la pila
                    if (food.get(0).y > tex_fondo.getSize().y - 98) {
                        doh.play();
                        vidas--;
                        vida.pop();
                        food.erase(0);
                        if (vidas == 0) {
                            momento = "game over";
                        }
                    }
                }

                //Si homero esta abajo de la basura
                if (trash.front().x + tex_trash[0].getSize().x > homer.x &&
                    trash.front().x < homer.x + tex_homer_buscando[0].getSize().x) {

                    //Y la basura llega hasta homero, pierde vida, se queja y se borra tanto la vida como la basura
                    if (trash.front().y > tex_fondo.getSize().y - 75) {
                        doh.play();
                        vidas--;
                        vida.pop();
                        trash.pop();
                        if (vidas == 0) {
                            momento = "game over";
                        }
                    }
                }
                //Si homero no esta debajo de la comida
                else {
                    //Y la comida cae, esta se borra sin mas
                    if (trash.front().y > tex_fondo.getSize().y - 74) {
                        trash.pop();
                    }
                }





                // Clear screen
                window.clear();
                // Draw the sprite
                window.draw(sp_fondo);
                // Draw the sprite
                homer.dibujar(window);

                //Dibuja toda la comida
                for (int i = 0; i < food.size(); i++) {
                    Comida aux = food.get(i);
                    aux.dibujar(window);
                    food.replace(i, aux);
                }


                //Dibuja la basura y se usa una cola auxiliar porque para leerla hay que borrarla
                queue<Basura> cola_aux;
                while(trash.size() >= 1) {
                    trash.front().dibujar(window);
                    cola_aux.push(trash.front());
                    trash.pop();
                }
                trash = cola_aux;

                //Dibuja los corazones y lo mismo que la basura con la pila
                stack<Corazon> pila_aux;
                for (int i = 0; i < vidas; i++) {
                    vida.top().dibujar(window);
                    pila_aux.push(vida.top());
                    vida.pop();
                }
                vida = pila_aux;

                window.draw(round);
                window.draw(score);



                // Update the window
                window.display();
            }
        }


        //Pantalla de GAME OVER
        if (momento == "game over") {

            //Guardamos la puntuacion
            guardar_puntuacion(atrapados, maximas);

            //Cargamos la nuevas puntuaciones maximas y las muestra
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

            //Pone el texto de "GAME OVER" y ubica los botones de "Exit" y "Play Again"
            Text text("GAME OVER", font, 60);
            text.setPosition(tex_fondo.getSize().x / 3, tex_fondo.getSize().y / 2);
            sp_replay.setPosition(230, 180);
            sp_exit.setPosition(600, 180);

            //Reiniciamos las variables del mouse para que no haya bugs
            for (int i = 0; i < 3; i++) {
                mouse[i] = 0;
            }

            //Una vez hecho lo anterior entramos a un bucle
            while (momento == "game over") {
                while (window.pollEvent(event)) {
                    switch (event.type) {

                        //Si cierra la ventana paramos la musica, salimos del bucle y cerramos
                        case Event::Closed:
                            intro.stop();
                            ingame.stop();
                            window.close();
                            momento = " ";
                            break;

                        //Lee los movimientos del mouse
                        case Event::MouseMoved:
                            mouse[0] = event.mouseMove.x;
                            mouse[1] = event.mouseMove.y;
                            break;

                        //Se fija si es apretado y/o soltado
                        case Event::MouseButtonPressed:
                            mouse[2] = 1;
                            break;
                        case Event::MouseButtonReleased:
                            mouse[2] = 0;
                            break;
                    }
                }

                //Si el mouse esta sobre el boton de "Replay"
                if (mouse[0] > sp_replay.getPosition().x &&
                    mouse[0] < sp_replay.getPosition().x + tex_replay[0].getSize().x &&
                    mouse[1] > sp_replay.getPosition().y &&
                    mouse[1] < sp_replay.getPosition().y + tex_replay[0].getSize().y) {

                    //Le ponemos el borde negro
                    sp_replay.setTexture(tex_replay[1]);

                    //Si presiona el mouse arranca el juego otra vez y reinicia todas las variables de juego
                    if (mouse[2] == 1) {
                        vidas = 3;
                        nivel = 0;
                        atrapados = 0;
                        homer.vel = 7;
                        homer.y = 540.0 - (tex_homer_buscando[0].getSize().y);
                        momento = "jugar";
                    }
                }
                //Si no esta sobre ese boton
                else {
                    //Los bordes son amarillos
                    sp_replay.setTexture(tex_replay[0]);
                }

                //Si el mouse esta sobre el boton "Exit"
                if (mouse[0] > sp_exit.getPosition().x &&
                    mouse[0] < sp_exit.getPosition().x + tex_exit[0].getSize().x &&
                    mouse[1] > sp_exit.getPosition().y &&
                    mouse[1] < sp_exit.getPosition().y + tex_exit[0].getSize().y) {
                    //Le pone bordes negros
                    sp_exit.setTexture(tex_exit[1]);

                    //Si se presiona el boton, salimos
                    if (mouse[2] == 1) {
                        intro.stop();
                        ingame.stop();
                        window.close();
                        momento = " ";
                    }
                }
                //Si no esta sobre este boton, los bordes amarillos
                else {
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
                window.draw(max_scores);
                // Update the window
                window.display();

            }
        }
    }

    //FIN DEL PROGRAMA
    return EXIT_SUCCESS;
}