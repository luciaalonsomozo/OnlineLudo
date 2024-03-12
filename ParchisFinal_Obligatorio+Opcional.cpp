/*PROYECTO DE FUNDAMENTOS DE PROGRAMACION I : PARCHIS
INCLUYE : VERSION OBLIGATORIA Y VERSION IA(Se activan con las constantes Debug y Opcional, en el caso de tener Opcional a true, para activar Opcional2 se deve elegir en la opciona de color que se le ofrece al jugador)
REALIZADO POR : ALBA BAUTISTA NUÑEZ Y LUCIA ALONSO MOZO
DOBLE GRADO DE INGENIERIA INFORMÁTICA Y MATEMATICAS
*/

#include <iostream>
#include<fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <limits> // Para pausa()
#ifdef _WIN32 // Se define autom?ticamente en Visual Studio
#include <windows.h>
#undef max // Para poder usar max() en pausa()
#endif
#include <time.h>
using namespace std;

//CONSTANTES
const string Archivo = "17.txt";
const bool Debug = true;
const bool Opcional = true;
const int NUM_FICHAS = 4, NUM_CASILLAS = 68, NUM_JUGADORES = 4;
const int SalidaAmarillo = 5, SalidaAzul = 22, SalidaRojo = 39, SalidaVerde = 56;
const int META = 108, ENTRADA_META = 101;
const int Seguro1 = 0, Seguro2 = 5, Seguro3 = 12;
const int ESTRATEGIA1_JUGADOR1 = 1;
const int ESTRATEGIA1_JUGADOR2 = 3;

//Datos declarados
typedef enum { Amarillo, Azul, Rojo, Verde, Ninguno, Gris } tColor;
typedef int tFichas[NUM_FICHAS];

//STRUCTS
struct tJugador {
    tFichas fichas;
    tColor color;
};
typedef tJugador tJugadores[NUM_JUGADORES];
struct tCasilla {
    tColor calle1;
    tColor calle2;
};
typedef tCasilla tCasillas[NUM_CASILLAS];
struct tJuego {
    tCasillas casillas;
    tJugadores jugadores;
    tColor jugadorTurno;
    int tirada;
    int premio = 0;
    int seises = 0;
    int ultimaFichaMovida;
    int jugadorElegido;
};
//PROTOTIPOS VERSION 3 OBLIGATORIA
bool esSeguro(int casilla); //Se le introduce el valor de la casilla en la que se encuentra el jugadorTurno. Esta función detecta si es un seguro, en ese caso, devuelve true.
int salidaJugador(tColor jug); //Se le introduce el tColor del jugador, y por tanto, detecta si es Amarillo, Rojo, Verde o Azul. Asigna la casilla de salida de cada jugador y devuelve esa casilla.
int zanataJugador(tColor jug);//Se le introduce el tColor del jugador, y por tanto, detecta si es Amarillo, Rojo, Verde o Azul. Asigna la casilla de zanata del correspondiente jugador, en concreto, cinco casilla menos que la salida del propio jugador..
string colorACadena(tColor color); //Se le introduce el tColor del jugador, y por tanto, detecta si es Amarillo, Rojo, Verde o Azul. Asigna a cada jugador su color.
void iniciar(tJuego& juego);  //La función iniciar genera un número aleatorio [0,3], para asignar el jugador que empieza la partida. Luego se inicializan los elementos del juego, de tal manera que todas las fichas de todos los jugadores est·n en casa y tanto la calle1 como la calle2 vacÌa.
void tiradaDepuracion(tJuego& juego, ifstream& archivo);   //La funcion tiradaDepuracion es creada por nosotras, no es obligatoria. Se le llama cuando el modo debug = True.  Su funciÛn principal es leer las tiradas que aparecen en el archivo parchis.txt, donde se encuentran las proximas tiradas que deben aparecer.
bool puente(const tJuego& juego, int casilla);  //La funcion puente sirve para localizar si existe un puente en una casilla. En el caso afirmativo, la funcion de vuelve true.
int cuantasEn(const tFichas jugador, int casilla); //La funcion cuantasEn sirve para localizar el numero de fichas del jugador que hay en una casilla. Para ello, hace un bucle for en el que recorre las 4 fichas y devuelve cuantas hay en esa casilla.
int primeraEn(const tFichas jugador, int casilla);  //La funcion primeraEn sirve para localizar la ficha con el índice mas bajo que se encuentra en una casilla.
int segundaEn(const tFichas jugador, int casilla);  //La funcion primeraEn sirve para localizar la ficha con el índice mas alto que se encuentra en una casilla.
void saleFicha(tJuego& juego);  //saleFicha tiene como funcion principal sacar una ficha. Coloca la ficha con el índice mas bajo que encuentre en la posicion -1 (en casa), en la calle 1.
bool todasEnMeta(const tFichas jugador);  //todasEnMeta devuelve el valor true cuando las cuatro fichas del mismo jugador se encuentren en casa, y solo asÌ, haya ganado la partida.
void aCasita(tJuego& juego, int casilla);  //La función aCasita sirve para mandar a la ficha correspondiente a su casa (-1).
bool procesa5(tJuego& juego, bool& pasaTurno);  //Recibe el juego, la variable pasaTurno (de salida) y devuelve true si se ha podido sacar ficha, y a su vez, pasaTurno se convierte en true si se ha podido sacar ficha. En caso contrario, ambas son false. 
bool puedeMover(const tJuego& juego, int ficha, int& casilla); //La funcion puedeMover evalua todos los posibles casos, y devuelve el valor true si el jugador puede mover ficha. Recibe el juego y la ficha que se va a mover asi como la casilla en la que se encuentra, la funcion calculara la nueva casilla solo si la ficha se puede mover.
void mover(tJuego& juego, int ficha, int casilla); //mover mueve la ficha a su correspondiente nueva casilla, teniendo en cuenta la tirada, y todos los casos que pueden ocurrir, como por ejemplo, que llegue a la meta, que coma... Recibe la nueva casilla calculada en puedeMover
bool procesa6(tJuego& juego, bool& pasaTurno); //procesa6 estudia todos los casos necesarios para cuando la tirada es 6, entre ellos abrir puente, tres seises consecutivos, o que la tirada pase a ser 7 si todas las fichas estan fuera de la casa en cuyo caso devolvera true , en caso contrario devolvera false.
void abrirPuente(tJuego& juego, int casilla, int casilla2);  //Esta funcion se encarga de abrir el puente cuando sea necesario, es llamada por procesa6. Recibe el juego, la casilla del puente asi como la posicion a la que se va a mover, se calculan dentro de la funcion.

// PROTOTIPOS TABLERO: PROPORCIONADOS POR EL PROFESOR
void mostrar(const tJuego& juego);
void setColor(tColor color);
void iniciaColores();
void pausa();
void cargar(tJuego& juego, ifstream& archivo);

// PROTOTIPOS VERSION 3 OPCIONAL
int eleccionColor(tJuego& juego, bool& opcional2); //Pide al usuario con que color quiere jugar o si quiere que jueguen las 4 maquinas
int puedeComer(tJuego& juego, int& casilla); //Recibe la posicion de las fichas, se encarga de recorrer todas las fichas a ver si alguna ficha de un jugador puede comer a otra de otro jugador, en ese caso, devuelve esa ficha, en caso contrario evuelve -1
int aMeta(tJuego& juego, int& casilla); //Identifica si alguna ficha llega a meta,recibe la posicion de las fichas y devuelve la ficha que se pueda mover a meta,  si ninguna puede devolvera -1
int aSeguro(tJuego& juego, int& casilla); //Recibe la posicion de las fichas y devuelve la ficha que se pueda mover a seguro en caso de que ninguna pueda devolvera -1.
int huir(tJuego& juego, int& casilla); //Identifica si hay una ficha que tenga a un jugador contrario cercano (hasta una distancia de 6 casillas), recibe la posicion de las fichas y devuelve la ficha que puede huir, en caso de que ninguna tenga que huir devolvera -1
int primeraPosible(tJuego& juego, int& casilla); //Si ninguna de las funciones anteriores devuelve una ficha, esta funcion se encarga de mover la primera que sea posible, recibe la posicion de las fichas, devolvera la ficha que puede mover primero, preferiblemente que no esté en seguro.
bool jugar(tJuego& juego, bool& fin, bool opcional2); //Jugar se encarga del desarrollo de todo el juego, interviene cuando procesa5 y procesa6 son false, la funcion devuelve true si se debe de pasarTurno, realiza el moviento de la ficha que elija el jugador y llama a jugar2 si la version opcional esta activada
void jugar2(tJuego& juego, bool opcional2); //Realiza las estrategias de la IA tanto cuando juega humano vs máquina cómo cuando juegan las 4 máquinas

bool esSeguro(int casilla) {
    int resto;
    bool seguro = false;

    resto = casilla % 17;
    if ((resto == Seguro1) || (resto == Seguro2) || (resto == Seguro3))
        seguro = true;
    return seguro;
}
int salidaJugador(tColor jug) {
    int salidajugador = 0;

    switch (jug) {
    case Amarillo:
        salidajugador = SalidaAmarillo;
        break;

    case Azul:
        salidajugador = SalidaAzul;
        break;

    case Rojo:
        salidajugador = SalidaRojo;
        break;
    case Verde:
        salidajugador = SalidaVerde;
        break;
    }

    return salidajugador;
}
int zanataJugador(tColor jug) {
    return salidaJugador(jug) - 5;
}
string colorACadena(tColor color) {
    string jugadorTurnoColor;
    switch (color) {
    case Amarillo:
        jugadorTurnoColor = "Amarillo";
        break;
    case Azul:
        jugadorTurnoColor = "Azul";
        break;
    case Rojo:
        jugadorTurnoColor = "Rojo";
        break;
    case Verde:
        jugadorTurnoColor = "Verde";
        break;
    }
    return jugadorTurnoColor;
}
void iniciar(tJuego& juego) {
    srand((unsigned int)time(NULL));

    //generador de numero aleatorio para ver el jugador que empieza.
    int turnoJugador = rand() % 4;

    //Inicializar los elementos del juego
    for (int i = 0; i <= NUM_CASILLAS; i++) {
        juego.casillas[i].calle1 = Ninguno;
        juego.casillas[i].calle2 = Ninguno;
    }


    for (int jug = 0; jug <= NUM_JUGADORES; jug++) {
        for (int i = 0; i <= NUM_FICHAS; i++) {
            juego.jugadores[jug].fichas[i] = -1;
        }
    }
    juego.seises = 0;
    juego.jugadorTurno = tColor(turnoJugador);

}
void tiradaDepuracion(tJuego& juego, ifstream& archivo) {
    archivo >> juego.tirada;
}
bool puente(const tJuego& juego, int casilla) {
    bool bloqueo = false;

    //si en la misma casilla de la calle1 y en la calle2 se encuentra el mismo jugador, se produce el bloqueo.
    if (((casilla < NUM_CASILLAS) && (casilla > -1)) && (juego.casillas[casilla].calle1 == juego.casillas[casilla].calle2) && (juego.casillas[casilla].calle1 != Ninguno))
        bloqueo = true;

    return bloqueo;
}
int cuantasEn(const tFichas jugador, int casilla) {
    int numfichas = 0;

    for (int i = 0; i < NUM_FICHAS; i++) {
        if (jugador[i] == casilla)
            numfichas++;
    }
    return numfichas;
}
int primeraEn(const tFichas jugador, int casilla) {

    int primera;
    bool encontrado = false;
    int i = 0;

    while ((!encontrado) && (i < NUM_FICHAS)) {
        if (jugador[i] == casilla) {
            primera = i;
            encontrado = true;
        }
        else {
            i++;
            primera = -1;
        }

    }
    return primera;
}
int segundaEn(const tFichas jugador, int casilla) {

    int segunda;
    bool encontrado = false;
    int i = 3;

    while ((!encontrado) && (i >= 0)) {
        if (jugador[i] == casilla) {
            segunda = i;
            encontrado = true;
        }
        else {
            i--;
            segunda = -1;
        }
    }
    return segunda;
}
void saleFicha(tJuego& juego) {
    //Localiza la casilla de salida que tiene el jugador
    int casillaSalida = salidaJugador(juego.jugadorTurno);
    int primeraficha = primeraEn(juego.jugadores[juego.jugadorTurno].fichas, -1);

    //Haya o no haya alguien en la calle1 , se saca ficha. (El jugador que estuviese en la calle1 pasa a la calle2)
    if (juego.casillas[casillaSalida].calle1 != Ninguno) {
        juego.casillas[casillaSalida].calle2 = juego.casillas[casillaSalida].calle1;
    }
    juego.casillas[casillaSalida].calle1 = juego.jugadorTurno;
    juego.jugadores[juego.jugadorTurno].fichas[primeraficha] = casillaSalida;
}
bool todasEnMeta(const tFichas jugador) {

    bool meta = false;

    if (cuantasEn(jugador, META) == NUM_FICHAS) {
        meta = true;


    }

    return meta;
}
void aCasita(tJuego& juego, int casilla) {

    tColor jugadorcasillacalle1, jugadorcasillacalle2;
    int ficha;
    jugadorcasillacalle1 = juego.casillas[casilla].calle1;
    jugadorcasillacalle2 = juego.casillas[casilla].calle2;

    if (jugadorcasillacalle2 != Ninguno) {
        ficha = segundaEn(juego.jugadores[jugadorcasillacalle2].fichas, casilla);
        juego.jugadores[juego.casillas[casilla].calle2].fichas[ficha] = -1;
        juego.casillas[casilla].calle2 = Ninguno;

    }
    else {
        ficha = segundaEn(juego.jugadores[jugadorcasillacalle1].fichas, casilla);
        juego.jugadores[juego.casillas[casilla].calle1].fichas[ficha] = -1;
        juego.casillas[casilla].calle1 = Ninguno;
    }
}
bool procesa5(tJuego& juego, bool& pasaTurno) {

    int casillaSalida = salidaJugador(juego.jugadorTurno);
    bool fichaEnCasa = false;
    bool sacaFicha = true;

    if (cuantasEn(juego.jugadores[juego.jugadorTurno].fichas, -1) == 0) {
        sacaFicha = false;
    }

    else { //Debemos ir haciendo casos

        //No hay nadie en la casilla de salida y no hay nadie en la calle2.
        if (juego.casillas[casillaSalida].calle2 == Ninguno) {
            saleFicha(juego);
            pasaTurno = true;
        }

        //Ambas calles ocupadas

        else {
            //En la calle2 un jugador ajeno.
            if ((juego.casillas[casillaSalida].calle2 != juego.jugadorTurno)) {
                aCasita(juego, casillaSalida);
                saleFicha(juego);
                pasaTurno = false;
                juego.premio = 20;
            }
            else {
                pasaTurno = true;
                sacaFicha = false;
            }

        }
    }
    return sacaFicha;
}
bool puedeMover(const tJuego& juego, int ficha, int& casilla) {

    bool mover = true;
    int zanatajug = zanataJugador(juego.jugadorTurno);
    int contador = 0;
    casilla = juego.jugadores[juego.jugadorTurno].fichas[ficha];

    if ((cuantasEn(juego.jugadores[juego.jugadorTurno].fichas, -1) == NUM_FICHAS) && (juego.tirada != 5) || (casilla == META) || (casilla == -1))
        mover = false;

    while ((mover) && (contador <= juego.tirada)) {
        contador++;
        casilla++;

        if (casilla == NUM_CASILLAS) {
            casilla = 0;
        }
        if (casilla == (zanataJugador(juego.jugadorTurno) + 1)) {
            casilla = ENTRADA_META;
        }
        else if ((casilla == META) && (juego.tirada != contador)) {
            casilla = casilla - contador;
            mover = false;
        }
        else if (esSeguro(casilla) && juego.casillas[casilla].calle2 != Ninguno && casilla < NUM_CASILLAS && juego.tirada == contador) {
            mover = false;

        }

        else if (puente(juego, casilla)) {
            if (juego.casillas[casilla].calle1 != juego.jugadorTurno) {
                mover = false;
            }
            else if ((juego.casillas[casilla].calle1 == juego.jugadorTurno) && (juego.tirada == contador)) {
                mover = false;
            }
        }
        if (juego.tirada == contador) {
            contador++;
        }
    }
    return mover;
}
void mover(tJuego& juego, int ficha, int casilla) {
    tColor colorjug;

    //Casilla de donde parte.
    int casillaanterior = juego.jugadores[juego.jugadorTurno].fichas[ficha];

    if ((juego.casillas[casillaanterior].calle2 != Ninguno)) {

        if (juego.casillas[casillaanterior].calle1 == juego.jugadorTurno) {
            juego.casillas[casillaanterior].calle1 = juego.casillas[casillaanterior].calle2;
        }
        juego.casillas[casillaanterior].calle2 = Ninguno;
    }
    else
        juego.casillas[casillaanterior].calle1 = Ninguno;

    //Nueva casilla.
    juego.jugadores[juego.jugadorTurno].fichas[ficha] = casilla;

    if ((juego.casillas[casilla].calle1 != Ninguno) && (casilla < NUM_CASILLAS)) {
        if (juego.casillas[casilla].calle1 == juego.jugadorTurno) {
            juego.casillas[casilla].calle2 = juego.jugadorTurno;
        }
        else {
            if (esSeguro(casilla)) {
                juego.casillas[casilla].calle2 = juego.jugadorTurno;
            }
            else {
                colorjug = juego.casillas[casilla].calle1;
                aCasita(juego, casilla);
                juego.premio = 20;
                juego.casillas[casilla].calle1 = juego.jugadorTurno;
                cout << "Ha comido una ficha del jugador " << colorACadena(colorjug) << " ... " << endl;

            }
        }

    }
    else {
        juego.casillas[casilla].calle1 = juego.jugadorTurno;
    }
    if (casilla == META) {
        cout << "La " << ficha + 1 << " ha llegado a la meta!" << endl;
        juego.premio = 10;
    }
}
bool procesa6(tJuego& juego, bool& pasaTurno) {

    bool seis = false;
    int contadorPuentes = 0;
    int puentesPuedenMover = 0;
    int nopueden = 0;
    int primeraficha1, segundaficha1, primeraficha2, segundaficha2, posicion, posicion1, posicion2;

    juego.seises++;

    //Si el jugador no tiene fichas en casa, la tirada pasar· a ser 7.
    if (cuantasEn(juego.jugadores[juego.jugadorTurno].fichas, -1) == 0) {
        juego.tirada = 7;
        cout << "Como no tiene fichas en casa, cuenta 7! " << endl;
        seis = true;
    }



    //Si salen tres seis consecutivos.
    if (juego.seises == 3) {
        pasaTurno = true;
        seis = true;
        //Solo ocurre algo si se encuentra en calle1 o calle2
        if ((juego.jugadores[juego.jugadorTurno].fichas[juego.ultimaFichaMovida] < NUM_CASILLAS) && (juego.ultimaFichaMovida != -1)) {
            cout << "Tercer seis consecutivo... La ultima ficha movida se va a casa!" << endl;
            aCasita(juego, juego.jugadores[juego.jugadorTurno].fichas[juego.ultimaFichaMovida]);
        }
        else if (juego.jugadores[juego.jugadorTurno].fichas[juego.ultimaFichaMovida] > NUM_CASILLAS) {
            cout << "Tercer seis consecutivo... La ultima ficha movida se salva por estar de camino a meta!" << endl;
        }
        else {
            cout << "Tercer seis consecutivo... Ninguna ficha se va a casa porque ninguna ha movido!" << endl;
        }
    }

    else {
        pasaTurno = false;
        seis = true;
        int i = 0;
        int j = 0;

        //Localizamos el numero de puentes que tiene el jugador.
        while ((i < NUM_FICHAS) && (puentesPuedenMover == 0)) {

            if (puente(juego, juego.jugadores[juego.jugadorTurno].fichas[i])) {
                primeraficha1 = primeraEn(juego.jugadores[juego.jugadorTurno].fichas, juego.jugadores[juego.jugadorTurno].fichas[i]);
                segundaficha1 = segundaEn(juego.jugadores[juego.jugadorTurno].fichas, juego.jugadores[juego.jugadorTurno].fichas[i]);
                contadorPuentes = 1;

                if (puedeMover(juego, segundaficha1, posicion1)) {
                    puentesPuedenMover = 1;
                }
            }
            i++;
        }

        if (contadorPuentes == 1) {
            while ((j < NUM_FICHAS)) {
                if ((puente(juego, juego.jugadores[juego.jugadorTurno].fichas[j])) && ((j != primeraficha1) && (j != segundaficha1))) {
                    primeraficha2 = primeraEn(juego.jugadores[juego.jugadorTurno].fichas, juego.jugadores[juego.jugadorTurno].fichas[j]);
                    segundaficha2 = segundaEn(juego.jugadores[juego.jugadorTurno].fichas, juego.jugadores[juego.jugadorTurno].fichas[j]);
                    contadorPuentes = 2;
                    if (puedeMover(juego, segundaficha2, posicion2)) {
                        puentesPuedenMover = 2;
                    }
                }
                j++;
            }
        }

        //Localizamos el numero de puentes que puede mover.
        if ((contadorPuentes == 2) && (puentesPuedenMover == 0)) {
            cout << "No se puede abrir ninguno de los dos puentes! Se ignora la tirada..." << endl;
        }

        else if (puentesPuedenMover == 1) {
            abrirPuente(juego, juego.jugadores[juego.jugadorTurno].fichas[segundaficha1], posicion1);
             //juego.ultimaFichaMovida = segundaficha1;
        }
        else {
            seis = false;
        }
    }



    return seis;
}
void abrirPuente(tJuego& juego, int casilla, int casilla2) {
    int f;

    f = segundaEn(juego.jugadores[juego.jugadorTurno].fichas, casilla);
    cout << "Se abre el puente de la casilla " << casilla << endl;
    mover(juego, f, casilla2);
    f = juego.ultimaFichaMovida;
}

//Tablero
void mostrar(const tJuego& juego) {
    int casilla, ficha;
    tColor jug;

    cout << "\x1b[2J\x1b[H"; // Se situa en la esquina superior izquierda
    setColor(Gris);
    cout << endl;

    // Filas con la numeraci?n de las casillas...
    for (int i = 0; i < NUM_CASILLAS; i++)
        cout << i / 10;
    cout << endl;
    for (int i = 0; i < NUM_CASILLAS; i++)
        cout << i % 10;
    cout << endl;

    // Borde superior...
    for (int i = 0; i < NUM_CASILLAS; i++)
        cout << '>';
    cout << endl;

    // Primera fila de posiciones de fichas...
    for (int i = 0; i < NUM_CASILLAS; i++) {
        setColor(juego.casillas[i].calle2);
        if (juego.casillas[i].calle2 != Ninguno)
            cout << primeraEn(juego.jugadores[juego.casillas[i].calle2].fichas, i) + 1;
        else
            cout << ' ';
        setColor(Gris);
    }
    cout << endl;

    // "Mediana"   
    for (int i = 0; i < NUM_CASILLAS; i++)
        if (esSeguro(i))
            cout << 'o';
        else
            cout << '-';
    cout << endl;


    // Segunda fila de posiciones de fichas...
    for (int i = 0; i < NUM_CASILLAS; i++) {
        setColor(juego.casillas[i].calle1);
        if (juego.casillas[i].calle1 != Ninguno)
            cout << segundaEn(juego.jugadores[juego.casillas[i].calle1].fichas, i) + 1;
        else
            cout << ' ';
        setColor(Gris);
    }
    cout << endl;

    jug = Amarillo;
    // Borde inferior...
    for (int i = 0; i < NUM_CASILLAS; i++)
        if (i == zanataJugador(jug)) {
            setColor(jug);
            cout << "V";
            setColor(Gris);
        }
        else if (i == salidaJugador(jug)) {
            setColor(jug);
            cout << "^";
            setColor(Gris);
            if (jug != Verde) jug = tColor(int(jug) + 1);
        }
        else
            cout << '>';
    cout << endl;

    // Metas y casas...
    for (int i = 0; i < NUM_FICHAS; i++) {
        casilla = 0;
        jug = Amarillo;
        setColor(jug);
        while (casilla < NUM_CASILLAS) {
            if (casilla == zanataJugador(jug)) {
                ficha = primeraEn(juego.jugadores[jug].fichas, 101 + i);
                if (ficha != -1) {
                    cout << ficha + 1;
                    if (cuantasEn(juego.jugadores[jug].fichas, 101 + i) > 1) {
                        ficha = segundaEn(juego.jugadores[jug].fichas, 101 + i);
                        if (ficha != -1) {
                            cout << ficha + 1;
                        }
                        else
                            cout << "V";
                    }
                    else
                        cout << "V";
                }
                else
                    cout << "VV";
                casilla++;
            }
            else if (casilla == salidaJugador(jug)) {
                if (juego.jugadores[jug].fichas[i] == -1) // En casa
                    cout << i + 1;
                else
                    cout << "^";
                if (jug != Verde) jug = tColor(int(jug) + 1);
                setColor(jug);
            }
            else
                cout << ' ';
            casilla++;
        }
        cout << endl;
    }

    // Resto de metas...
    for (int i = 105; i <= 107; i++) {
        casilla = 0;
        jug = Amarillo;
        setColor(jug);
        while (casilla < NUM_CASILLAS) {
            if (casilla == zanataJugador(jug)) {
                ficha = primeraEn(juego.jugadores[jug].fichas, i);
                if (ficha != -1) {
                    cout << ficha + 1;
                    if (cuantasEn(juego.jugadores[jug].fichas, i) > 1) {
                        ficha = segundaEn(juego.jugadores[jug].fichas, i);
                        if (ficha != -1) {
                            cout << ficha + 1;
                        }
                        else
                            cout << "V";
                    }
                    else
                        cout << "V";
                }
                else
                    cout << "VV";
                casilla++;
                if (jug != Verde) jug = tColor(int(jug) + 1);
                setColor(jug);
            }
            else
                cout << ' ';


            casilla++;
        }
        cout << endl;
    }

    casilla = 0;
    jug = Amarillo;
    setColor(jug);
    while (casilla < NUM_CASILLAS) {
        cout << ((juego.jugadores[jug].fichas[0] == 108) ? '1' : '.');
        cout << ((juego.jugadores[jug].fichas[1] == 108) ? '2' : '.');
        if (jug != Verde) jug = tColor(int(jug) + 1);
        setColor(jug);
        cout << "               ";
        casilla += 17;
    }
    cout << endl;
    casilla = 0;
    jug = Amarillo;
    setColor(jug);
    while (casilla < NUM_CASILLAS) {
        cout << ((juego.jugadores[jug].fichas[2] == 108) ? '3' : '.');
        cout << ((juego.jugadores[jug].fichas[3] == 108) ? '4' : '.');
        if (jug != Verde) jug = tColor(int(jug) + 1);
        setColor(jug);
        cout << "               ";
        casilla += 17;
    }
    cout << endl << endl;
    setColor(Gris);
}
void setColor(tColor color) {
    switch (color) {
    case Azul:
        cout << "\x1b[34;107m";
        break;
    case Verde:
        cout << "\x1b[32;107m";
        break;
    case Rojo:
        cout << "\x1b[31;107m";
        break;
    case Amarillo:
        cout << "\x1b[33;107m";
        break;
    case Gris:
    case Ninguno:
        cout << "\x1b[90;107m";
        break;
    }
}
void iniciaColores() {
#ifdef _WIN32
    for (DWORD stream : {STD_OUTPUT_HANDLE, STD_ERROR_HANDLE}) {
        DWORD mode;
        HANDLE handle = GetStdHandle(stream);

        if (GetConsoleMode(handle, &mode)) {
            mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(handle, mode);
        }
    }
#endif
}
void pausa() {
    cout << "Pulsa Intro para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
void cargar(tJuego& juego, ifstream& archivo) {
    int jugador, casilla, tirada;

    for (int i = 0; i < NUM_JUGADORES; i++) {
        for (int f = 0; f < NUM_FICHAS; f++) {
            archivo >> casilla;
            juego.jugadores[i].fichas[f] = casilla;
            if ((casilla >= 0) && (casilla < NUM_CASILLAS))
                if (juego.casillas[casilla].calle1 == Ninguno)
                    juego.casillas[casilla].calle1 = tColor(i);
                else
                    juego.casillas[casilla].calle2 = tColor(i);
        }
    }
    archivo >> jugador;
    juego.jugadorTurno = tColor(jugador);
}

//PARTE OPCIONAL
int eleccionColor(tJuego& juego, bool& opcional2) {
    int opcion, eleccion, opcion1;

    cout << "Elija opción o color del jugador: " << endl;
    cout << endl;
    setColor(Amarillo);
    cout << "[0] = Amarillo || ";
    setColor(Azul);
    cout << " [1] = Azul || ";
    setColor(Rojo);
    cout << " [2] = Rojo || ";
    setColor(Verde);
    cout << " [3] = Verde || ";
    setColor(Gris);
    cout << " [4] = Todos Maquinas " << endl;
    cout << endl;
    cout << "Opcion: ";
    cin >> opcion;
    cout << endl;

    while (opcion < 0 || opcion > 4) {
        cout << "Opcion incorrecta! Numero de jugador [0..4]: ";
        cin >> opcion;
    }

    if (opcion == 4) {
        cout << "Todos los jugadores son maquinas. " << endl;
        opcional2 = true;
        eleccion = -1;
    }
    else {
        eleccion = tColor(opcion);
        setColor(tColor(eleccion));
        cout << "Eres el jugador " << colorACadena(tColor(eleccion)) << endl;
        opcional2 = false;
    }
    system("pause");

    return eleccion;
}
int puedeComer(tJuego& juego, int& casilla) {
    int fichapuedeComer;
    bool encontrado = false;
    bool encontrada = false;
    int i = 0;
    int cont = 0;
    int seguros = 0;

    while ((!encontrado) && (i < NUM_FICHAS)) {
        casilla = juego.jugadores[juego.jugadorTurno].fichas[i];
        if (!esSeguro(casilla) && !puente(juego, casilla)) {
            if (puedeMover(juego, i, casilla) && (casilla < NUM_CASILLAS) && (!esSeguro(casilla)) && ((juego.casillas[casilla].calle1 != juego.jugadorTurno) && (juego.casillas[casilla].calle1 != Ninguno) || ((juego.casillas[casilla].calle2 != juego.jugadorTurno) && (juego.casillas[casilla].calle2 != Ninguno)))) {
                fichapuedeComer = i;
                encontrado = true;
            }
            else {
                fichapuedeComer = -1;
                seguros++;
                i++;
            }
        }
        else {
            fichapuedeComer = -1;
            seguros++;
            i++;
        }
    }
    i = 0;
    if (seguros == 4) {
        while (!encontrada && i < NUM_FICHAS) {
            if (puedeMover(juego, i, casilla) && (casilla < NUM_CASILLAS) && (!esSeguro(casilla)) && ((juego.casillas[casilla].calle1 != juego.jugadorTurno) && (juego.casillas[casilla].calle1 != Ninguno) || ((juego.casillas[casilla].calle2 != juego.jugadorTurno) && (juego.casillas[casilla].calle2 != Ninguno)))) {
                fichapuedeComer = i;
                encontrada = true;
            }
            else {
                i++;
                fichapuedeComer = -1;
            }
        }
    }

    if (fichapuedeComer != -1) {
        cout << "Se mueve la ficha " << fichapuedeComer + 1 << " porque puede comer..." << endl;
    }
    return fichapuedeComer;
}
int aMeta(tJuego& juego, int& casilla) {

    bool encontrado = false;
    int fichaMeta;
    int i = 0;

    while ((!encontrado) && (i < NUM_FICHAS)) {
        if (puedeMover(juego, i, casilla) && (casilla == META)) {
            fichaMeta = i;
            encontrado = true;
        }
        else {
            fichaMeta = -1;
            i++;
        }
    }
    if (fichaMeta != -1) {
        cout << "Se mueve la ficha " << fichaMeta + 1 << " porque va a meta..." << endl;
    }
    return fichaMeta;
}
int aSeguro(tJuego& juego, int& casilla) {
    int fichaSeguro;
    bool encontrado = false;
    int i = 0;
    while ((!encontrado) && (i < NUM_FICHAS)) {

        if (puedeMover(juego, i, casilla) && (esSeguro(casilla) && (casilla < NUM_CASILLAS))) {
            fichaSeguro = i;
            encontrado = true;
        }
        else {
            fichaSeguro = -1;
            i++;
        }
    }
    if (fichaSeguro != -1) {
        cout << "Se mueve la ficha " << fichaSeguro + 1 << " porque va a seguro..." << endl;
    }
    return fichaSeguro;
}
int huir(tJuego& juego, int& casilla) {

    bool encontrado = false;
    int fichaHuye;
    int MAX = 6;
    int i = 0;
    int g = 0;
    int casilla2;

    while (g < MAX && (!encontrado)) {
        g++;
        i = 0;
        while (i < NUM_FICHAS && !encontrado) {

            casilla = juego.jugadores[juego.jugadorTurno].fichas[i];
            casilla2 = casilla - g;

            if (casilla2 < 0) {
                casilla2 = casilla2 + 68;
            }

            if ((!esSeguro(casilla)) && (casilla < NUM_CASILLAS) && (juego.casillas[casilla2].calle1 != Ninguno) && (juego.casillas[(casilla2) % 68].calle1 != juego.jugadorTurno) && puedeMover(juego, i, casilla)) {
                fichaHuye = i;
                encontrado = true;
            }
            else {
                fichaHuye = -1;
                i++;
            }
        }
    }
    if (fichaHuye != -1) {
        cout << "Se mueve la ficha " << fichaHuye + 1 << " ..." << endl;
    }

    return fichaHuye;
}
int primeraPosible(tJuego& juego, int& casilla) {

    int primeraFicha;
    bool encontrado = false;
    int i = 0;
    int j = 0;
    int seguros = 0;

    while ((!encontrado) && (i < NUM_FICHAS)) {
        casilla = juego.jugadores[juego.jugadorTurno].fichas[i];
        if (!esSeguro(casilla) && puedeMover(juego, i, casilla)) {
            primeraFicha = i;
            encontrado = true;
        }
        else {
            i++;
            primeraFicha;
            seguros++;
        }
    }

    if (seguros == 4) {
        while ((!encontrado) && (j < NUM_FICHAS)) {
            if (puedeMover(juego, j, casilla)) {
                primeraFicha = j;
                encontrado = true;
            }
            else {
                j++;
                primeraFicha = -1;
            }
        }
    }

    if (primeraFicha != -1) {
        cout << "Se mueve la ficha " << primeraFicha + 1 << " ..." << endl;
    }

    return primeraFicha;
}
bool jugar(tJuego& juego, bool& fin, bool opcional2) {

    //La funcion jugar sirve para mostrar y preguntar al jugador que ficha quiere mover y a que casilla se va a mover.Devuelve true si se pasa turno, adem∑s de el premio, seises y ultimafichamovida 

    int fichasmover = 0;
    int ficha, ficha1, contador, posicion;
    bool puede, siguientejug;
    bool salirdeljuego = false;

    //Vemos cuantas fichas se pueden mover

    for (int i = 0; i < NUM_FICHAS; i++) {
        if (puedeMover(juego, i, posicion)) {
            fichasmover++;
            ficha1 = i;
        }
    }

    if (fichasmover > 0) {

        //Si solo se puede mover una ficha, se mueve(por defecto), no enseÏa el resto
        if (fichasmover == 1) {
            if (puedeMover(juego, ficha1, posicion)) {
                cout << "Se mueve la ficha " << ficha1 + 1 << " de la casilla " << juego.jugadores[juego.jugadorTurno].fichas[ficha1] << " a la casilla " << posicion << endl;
                mover(juego, ficha1, posicion);

                juego.ultimaFichaMovida = ficha1;
            }
        }
        else {
            ficha = -2;
            puede = false;
            contador = 1;

            if (!Opcional || ((juego.jugadorTurno == juego.jugadorElegido) && !opcional2)) {
                while (((ficha < 0) || (ficha > 3)) && (!salirdeljuego)) {
                    cout << "Por favor, elige la ficha que quieres mover... " << endl;
                    for (int f = 0; f < NUM_FICHAS; f++) {
                        if (puedeMover(juego, f, posicion)) {
                            cout << f + 1 << ": ";
                            cout << "De la casilla " << juego.jugadores[juego.jugadorTurno].fichas[f] << " a la casilla " << posicion << endl;
                            puede = true;
                        }
                    }
                    if (puede) {
                        cout << "Ficha (0 para salir): ";
                        cin >> ficha;
                        cin.get();
                        ficha--;
                        if (ficha == -1) {
                            salirdeljuego = true;
                            fin = true;
                        }
                        else if (puedeMover(juego, ficha, posicion)) {
                            mover(juego, ficha, posicion);
                            juego.ultimaFichaMovida = ficha;
                        }
                        else {
                            ficha = -2;
                        }
                    }
                }
            }
            else {
                jugar2(juego, opcional2);
            }
        }
    }
    else {
        if (cuantasEn(juego.jugadores[juego.jugadorTurno].fichas, -1) == NUM_FICHAS) {
            cout << "El jugador tiene todas las fichas en casa... No puede mover" << endl;
        }
        else
            cout << "No se puede mover ninguna ficha: siguiente jugada..." << endl;
    }

    //Si hay un premio o la tirada ha sido 6/7 pasaturno es false y por tanto la funcion devolvera false 
    //Si se acaba de jugar un premio y los seises son mayores que 0 pasaturno sera false
    if (((juego.premio > 0) || ((juego.tirada == 6) || (juego.tirada == 7))) || ((juego.tirada > 7) && (juego.seises > 0))) {
        siguientejug = false;
    }
    //En caso contrario, pasaturno sera true
    else {
        siguientejug = true;
    }

    if (todasEnMeta(juego.jugadores[juego.jugadorTurno].fichas)) {
        fin = true;
        siguientejug = false;
    }


    return siguientejug;
}
void jugar2(tJuego& juego, bool opcional2) {

    bool estrategia1;
    int ficha, posicion;

    if (opcional2 && (juego.jugadorTurno == ESTRATEGIA1_JUGADOR1) || (juego.jugadorTurno == ESTRATEGIA1_JUGADOR2))
        estrategia1 = true;
    else
        estrategia1 = false;

    if (opcional2 && estrategia1) {
        ficha = aMeta(juego, posicion);
        if (ficha == -1) {
            ficha = aSeguro(juego, posicion);
            if (ficha == -1) {
                ficha = puedeComer(juego, posicion);
                if (ficha == -1) {
                    ficha = primeraPosible(juego, posicion);
                }
            }
        }
    }
    else {
        ficha = puedeComer(juego, posicion);
        if (ficha == -1) {
            ficha = aMeta(juego, posicion);
            if (ficha == -1) {
                ficha = aSeguro(juego, posicion);
                if (ficha == -1) {
                    ficha = huir(juego, posicion);
                    if (ficha == -1) {
                        ficha = primeraPosible(juego, posicion);
                    }
                }
            }
        }
    }

    if (ficha != -1) {
        mover(juego, ficha, posicion);
        juego.ultimaFichaMovida = ficha;
    }
}

int main() {

    bool fin = false;
    bool pasaTurno, jugada, depuracion;
    bool opcional2 = false;
    system("chcp 1252");
    system("CLS");
    system("color F0");

    tJuego juego;
    tJugadores jugadores;
    tFichas jugador;
    tColor jug;
    int ficha = -1;

    int casilla = -1;

    ifstream archivo;

    if (Opcional) {
        juego.jugadorElegido = eleccionColor(juego, opcional2);
    }

    iniciaColores();

    iniciar(juego);

    if (Debug) {
        archivo.open(Archivo);
        if (archivo.is_open()) {
            cargar(juego, archivo);
        }
    }
    setColor(juego.jugadorTurno);
    mostrar(juego);

    while (!fin) {

        setColor(juego.jugadorTurno);

        cout << "Turno para el jugador " << colorACadena(juego.jugadorTurno) << endl;
        if (juego.premio > 0) {
            cout << "Ha de contar " << juego.premio << "!" << endl;
            juego.tirada = juego.premio;
            juego.premio = 0;
        }
        else {
            if (Debug) {
                tiradaDepuracion(juego, archivo);

                if ((juego.tirada != -1) && (archivo.is_open())) {
                    depuracion = false;
                }
                else {
                    archivo.close();
                    depuracion = true;
                }

                if (depuracion) {
                    cout << "Dame tirada: ";
                    cin >> juego.tirada;
                    cin.get();
                }
            }
            else {
                juego.tirada = rand() % 6 + 1;
            }

            cout << "Sale un " << juego.tirada << endl;
        }

        if (juego.tirada == 0)
            fin = true;

        else {
            jugada = false;
            pasaTurno = true;

            if (juego.tirada < 6) {
                juego.seises = 0;
            }

            if (juego.tirada == 5) {
                jugada = procesa5(juego, pasaTurno);

            }
            else if (juego.tirada == 6) {
                jugada = procesa6(juego, pasaTurno);
            }
            if (!jugada) {
                pasaTurno = jugar(juego, fin, opcional2);
            }
            system("pause");
            mostrar(juego);

            if (pasaTurno) {
                juego.jugadorTurno = tColor((int(juego.jugadorTurno) + 1) % NUM_JUGADORES);
                juego.seises = 0;
                juego.ultimaFichaMovida = -1;
            }
        }
    }

    if (fin) {
        if (todasEnMeta(juego.jugadores[juego.jugadorTurno].fichas)) {
            setColor(juego.jugadorTurno);
            cout << "ENHORABUENA, HAS GANADO LA PARTIDA" << endl;
            system("pause");
        }

    }

    return 0;
}