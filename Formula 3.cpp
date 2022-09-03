#include <stdio.h> // printf, getchar...
#include <windows.h> // Todo lo de la funcion gotoxy
#include <conio.h> // hbhit, getch...
#include <iostream> // cout
#include <time.h> // srand, rand

using namespace std; // Necesario para cout, cin

#define enter 13
#define der 77
#define izq 75
#define pausa 112
#define carril1 12
#define carril2 38
#define carril3 64
#define xlimit 80
#define ylimit 43

int xa; // Coordenada en x del auto
int ya; // Coordenada en y del auto
int xo1; // Coordenada en x del obstaculo 1
int yo1; // Coordenada en y del obstaculo 1
int xo2; // Coordenada en x del obstaculo 2
int yo2; // Coordenada en y del obstaculo 2
int tecla=0; // Variable que almacena el valor de la ultima tecla pulsada
int nivel=1; // Determina el nivel de dificultad
int puntos=0; // Inicializa el puntaje
int prevCarril=0;

void gotoxy(int xx, int yy) { // Funcion que coloca el cursor en posicion determinado
	HANDLE hcon;
	hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD dwPos;
	dwPos.X = xx;
	dwPos.Y = yy;
	SetConsoleCursorPosition(hcon, dwPos);
}

void llenar_pantalla() {
    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 4); // ESTA FUNCION CAMBIA EL COLOR
    system("cls");
    for (int i=2;i<ylimit;i++) {
        gotoxy(0,i);
        for (int i=0;i<xlimit;i++) {
            cout << "/";
        }
        cout << endl;
    } // Llena la pantalla con este simbolo "/"
}

void pause() { // Pausa el juego
	while (getch() != enter); // Mientras lo que presione el usuario sea diferente de ENTER
}

void init() { // Establece la configuracion de inicio
    xa = carril2;
    ya = 38;
    xo1 = carril2;
    yo1 = 5;
    xo2 = carril3;
    yo2 = 5;
    prevCarril = 1;
    nivel = 1;
    puntos = 0;
    system("cls");
    llenar_pantalla();
    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
	gotoxy(25,23);
	printf("Pulse ENTER para continuar");
    pause();
}

void continuar() {
    llenar_pantalla();
    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
    gotoxy(35,18);
    printf("GAME OVER");
    gotoxy(25,23);
    printf("Pulse ENTER para reintentar");   
    gotoxy(29,28);
    printf("Puntos alcanzados: %d", puntos);
    pause();
}

void carretera() { // Dibuja la carretera
    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 2);
	for (int i=1;i<ylimit+1;i++) {
        gotoxy(0, i);
		printf("|                         |                         |                         |");
    }
}

int elegir_carril() { // Seleccione el carril donde se posiciona
    srand(time(NULL)); // Inicializa la semilla de la funcion rand
    int x = rand()%3; // Un numero al azar
    if (x == prevCarril) { // Si el carril es el mismo de antes
        if (x == 0) {x=1;}
        else if (x == 1) {x=2;}
        else {x=0;}
        // Modifica el valor al siguiente
    }
    prevCarril = x; // Guarda el ultimo carril
    if (x == 0) {x = carril1;}
    else if (x == 1) {x = carril2;}
    else {x = carril3;}
    return x;
}

void puntaje() {
    gotoxy(39, 0);
    printf("%d", puntos);
}

void dibujar(int x, int y) { // Dibuja el auto en la posicion indicada
    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 9);
    gotoxy(x+1, y);
    printf("|");
    gotoxy(x, y+1);
    printf("-*-");
    gotoxy(x+1, y+2);
    printf("|");
    gotoxy(x, y+3);
    printf("-*-");
}

void ganador() {
    llenar_pantalla();
    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
    gotoxy(25, 18);
    printf("Ganador - Juego terminado");
    gotoxy(25, 23);
    printf("Pulse ENTER para continuar");
    gotoxy(29,28);
    printf("Puntos alcanzados: %d", puntos);
    pause();
    init();
}

void obstaculo() { // Dibuja los obstaculos
    if (yo1+3 == ylimit) { // Si llego al final de la pantalla
        yo1 = 5; // Lo devuelve al principio
        yo2 = 5;
        xo1 = elegir_carril(); // Y elige el carril
        xo2 = elegir_carril();
        if (xo1 == xo2) {
            if (xo2 == carril1) {xo2 = carril2;}
            else if (xo2 == carril2) {xo2 = carril3;}
            else {xo2 = carril1;}
        }
        puntos++;
        if (puntos == 10) {nivel=2;}
        else if (puntos == 20) {ganador();}
    }
    else {yo1++;yo2++;} // Si no, avanza
    dibujar(xo1, yo1);
    if (nivel == 2) {dibujar(xo2,yo2);}
}

void derecha() { // Posiciona el auto un carril a la derecha
	if (xa == carril1) {xa = carril2;}
	else if (xa == carril2) {xa = carril3;}
}

void izquierda() { // Posiciona el auto un carril a la izquierda
	if (xa == carril2) {xa = carril1;}
	else if (xa == carril3) {xa = carril2;}
}

void explosion(int x, int y) {
    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 6);
    gotoxy(x+4, y+3);
    printf("*");
    Sleep(100);
    gotoxy(x, y+3);
    printf("*********");
    Sleep(100);
    gotoxy(x+1, y+2);
    printf("*******");
    Sleep(100);
    gotoxy(x+1, y+4);
    printf("*******");
    Sleep(100);
    gotoxy(x+2, y+1);
    printf("*****");
    gotoxy(x+2, y+5);
    printf("*****");
    gotoxy(x+3, y);
    printf("***");
    gotoxy(x+3, y+6);
    printf("***");
    Sleep(1500);
    // Borra pantalla e imprime una explosion
    continuar(); // Envia a la pantalla para continuar
}

void finished() {
    explosion(xa-3, ya-3);
    Sleep(500);
    init();
}

void limites() { // Verifica si los autos chocaron
    if (((xa+1 == xo1+1) && (ya+3 > yo1) && (ya-1 < yo1+3))) {
        finished();
    } else if (nivel == 2) {
        if ((xa+1 == xo2+1) && (ya+3 > yo2) && (ya < yo2+3)) {
            finished();
        }
    }
}

main() {
    init();
	while (true) { // Se ejecuta por siempre
		if (kbhit()) { // Si se esta pulsando una tecla
			tecla = getch(); // Guarda el valor de la tecla pulsada
			if (tecla == der) {derecha();}
			else if (tecla == izq) {izquierda();}
			else if (tecla == pausa) {pause();}
		}
		dibujar(xa, ya);
		carretera();
		obstaculo();
		limites();
		puntaje();
		system("cls"); // Borra TODA la pantalla
	}
}
