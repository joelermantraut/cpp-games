#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>

#define enter 13
#define pausa 112
#define escape 27
#define espacio 32
#define lxar 1
#define lxab 42

using namespace std;

int x,y,xp,yp,cont,random,jump,line,puntos,color;
bool ala;
char c;

void gotoxy(int xx, int yy) { // Funcion que coloca el cursor en posicion determinado
    HANDLE hcon;
    hcon = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD dwPos;
    dwPos.X = xx;
    dwPos.Y = yy;
    SetConsoleCursorPosition(hcon, dwPos);
}

void init() {
    x = 25;
    y = 20;
    xp = 6;
    line = true;
    puntos = 0;
    ala = true;
    color = rand()%3;
    switch (color) {
        case 0: color = 3;break;
        case 1: color = 4;break;
        case 2: color = 6;break;
    }
}

void continuar() {
    while (getch() != enter);
}

void casco(bool mode) { // Final del tubo superior y principio del tubo inferior
    if (mode == true) {
        gotoxy(xp-2,yp+random);
        printf("---            ---");
        gotoxy(xp-2,yp+random+1);
        printf("|                |");
        gotoxy(xp-2,yp+random+2);
        printf("|                |");
        gotoxy(xp-2,yp+random+3);
        printf("------------------");
    } else { // Diferencia de 10 posiciones
        gotoxy(xp-2,yp+random+18);
        printf("------------------");
        gotoxy(xp-2,yp+random+19);
        printf("|                |");
        gotoxy(xp-2,yp+random+20);
        printf("|                |");
        gotoxy(xp-2,yp+random+21);
        printf("---            ---");
    }
}

void obst() {
    xp--;
    yp = 0;
    if (xp == 5) {
        random = rand()%22+1;
        xp = 60;
    }
    for (int i=0;i<random;i++) { // Tubo superior
        gotoxy(xp,yp+i);
        printf("|            |");
    }
    casco(true); // Casco del tubo superior
    casco(false); // Casco del tubo inferior
    for (int i=0;i<(25-random);i++) { // Tubo inferior
        gotoxy(xp,yp+i+random+17);
        printf("|            |");
    }
}

void over() {
    Sleep(1000);
    system("cls");
    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), color);
    for (int i=0;i<44;i++) {
        gotoxy(0,i);
        for (int i=0;i<80;i++) {
            cout << "/";
        }
        cout << endl;
    }
    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 5);
    gotoxy(35,20);
    printf("GAME OVER");
    gotoxy(25,25);
    printf("Pulse ENTER para reintentar");
    gotoxy(0,0);
    continuar();
    init();
}

void limites() {
    if (x+6 == xp && y+1 >= 0 && y+1 <= random) {over();} // Impacto con tubo superior
    else if (x+5 == xp-2 && y >= random && y <= random+3) {over();} // Impacto con casco superior
    else if (x+6 == xp-2 && y >= random-1 && y <= random+3) {over();} // Impacto punta casco superior
    else if (x+5 == xp-2 && y+2 >= 0 && y+2 <= random) {over();} // ""
    else if (y == yp+random+3 || y+2 == yp+random+13) {
        for (int i=xp-2;i<xp+16;i++) {
            for (int i2=x+1;i2<x+5;i2++) {
                if (i == i2) {over();}
            }
        }
    } // ""
    else if (x+6 == xp-2 && y+1 > random+13 && y+1 < random+17) {over();} // Impacto con casco inferior
    else if (x+5 == xp-2 && y > random+13 && y < random+16) {over();} // ""
    else if (x+5 == xp-2 && y+2 > random+13 && y+2 < random+16) {over();} // ""
    else if (x+6 == xp && y+1 >= random+17 && y+1 <= lxab) {over();} // Impacto con tubo inferior
    else if (x == xp+17) {puntos++;} // Aumentar puntos al superar el obstaculo
}

void linea() {
    gotoxy(0,41);
    printf("_______________________________________________________________________________");
    gotoxy(0,42);
    if (line == true) {
        line = false;
        printf("| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |");
    } else {
        line = true;
        printf(" | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | ");
    }
}

void cuerpo() {  // FUNCION QUE DIBUJA EL PAJARO
    if (jump == 0) {
        y++;
        if (y+2 == lxab) {over();}
    } else if (jump == 1) {jump = 0;}
    system("cls");
    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), color);
    gotoxy(x+1,y);
    printf("----");
    gotoxy(x,y+1);
    if (ala == true) {printf("(   *)");}
    else {printf("( ° *)");}
    gotoxy(x+1,y+2);
    printf("----");
    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 2);
    linea();
    obst();
    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 5);
    gotoxy(3,20);
    cout << puntos << endl;
    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), 7);
    limites();
}

void salto() { // Funcion que realiza el salto
    jump = 2;
    for (int i=0;i<5;i++) {
        y--;
        if (y == lxar) {y=2;break;}
        cuerpo();
    }
    jump = 1;
}

int main() {
    init();
    cuerpo();
    gotoxy(x,y-10);
    printf("Pulse ENTER para iniciar");
    gotoxy(x,y-9);
    printf("Pulse P para pausar");
    gotoxy(x,y-8);
    printf("Pulse ESPACIO para saltar");
    while (getch() != enter);
    while (true) {
        if (kbhit()) {
            c = getch();
            switch (c) {
                case pausa:
                    if (xp > 15) {
                        gotoxy(0, 0);
                    } else {
                        gotoxy(55, 0);
                    }
                    printf("Pulse 'p' para continuar");
                    while (getch() != pausa);break;
                case espacio: salto();break;
                case escape:
                    system("cls");
                    gotoxy(35,21);
                    printf("Pulse ENTER para finalizar");
                    continuar();
                    exit(0);
            }
        }
        cuerpo();
    }
}
