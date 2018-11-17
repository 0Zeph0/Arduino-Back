#include "LedControlMS.h" 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

LedControl lc=LedControl(12,11,10,1); // Los numeros se refieren a que pin de ARDUINO tienes en cada uno de los terminales
byte Maze1[] = 

{B01111111,
B00000000,
B11101111,
B11000000,
B11101111,
B11101111,
B11101111,
B11100000};

byte Maze2[] =

{B00100000,
B00000000,
B00010000,
B10000000,
B00001000,
B00000000,
B01000000,
B00000000};

int maze[8][8]  =  {
        {1, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 1, 1, 1, 1, 1}
    };

int maze2[8][8] = {
        {1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 0, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1}
};


void setup() {
  // El numero que colocamos como argumento de la funcion se refiere a la direccion del decodificador
  lc.shutdown(0,false);
  lc.setIntensity(0,1);// La valores estan entre 1 y 15 
  lc.clearDisplay(0);

}
void Representar(byte *Datos,int retardo) //Funcion para la representacion de bytes de datos para una matriz de 8x8  
{
  for (int i = 0; i < 8; i++)  
  {
    lc.setRow(0,i,Datos[i]);
  }
  delay(retardo);
}

bool solveMazeUtil(int maze[8][8], int x, int y, int sol[8][8])
{
    // if (x,y is goal) return true
    if(x == 8-1 && y == 8-1)
    {
        sol[x][y] = 1;
        lc.setLed(0,x,y,true);
        delay(500);
        return true;
    }
    
    // Check if maze[x][y] is valid
    if(isSafe(maze, x, y) == true)
    {
        // mark x,y as part of solution path
        sol[x][y] = 1;
        lc.setLed(0,x,y,true);
        delay(500);

        /* Move forward in x direction */
        if (solveMazeUtil(maze, x+1, y, sol) == true)
            //lc.setLed(0,x,y,true);
            //delay(1000);
            return true;

        /* If moving in x direction doesn't give solution then
           Move down in y direction  */
        if (solveMazeUtil(maze, x, y+1, sol) == true)
            //lc.setLed(0,x,y,true);
            //delay(1000);
            return true;

        /* If none of the above movements work then BACKTRACK:
            unmark x,y as part of solution path */
        sol[x][y] = 0;
        lc.setLed(0,x,y,true);
        delay(500);
        return false;
    }
    return false;
}

bool solveMaze(int maze[8][8])
{
    int sol[8][8] = {
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0}
    };

    if(solveMazeUtil(maze, 0, 0, sol) == false)
    {
        printf("Solution doesn't exist");
        return false;
    }

    //printSolution(sol);
    return true;
}

bool isSafe(int maze[8][8], int x, int y)
{
    // if (x,y outside maze) return false
    if(x >= 0 && x < 8 && y >= 0 && y < 8 && maze[x][y] == 1)
        return true;

    return false;
}

//-------------------funciones de obstaculizar en el led-------------------//

void obstaculizar(int matriz[8][8]) {
  srand(time(NULL));
  bool salida = false;

  while (!salida) {
    int fila = rand() % 8;
    int columna = rand() % 8;
    lc.setLed(0,fila,columna,true);
    if (matriz[fila][columna] != 0) {
      matriz[fila][columna] = 0;
      lc.setLed(0,fila,columna,true);
      salida = true;
    }
  }
}
/*
void imprimir(int matriz[8][8]) {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      cout << matriz[i][j] << ", ";
    }
    cout << endl;
  }
}*/


void loop() {

  int matriz[8][8];

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      matriz[i][j] = 1;
    }
  }
  


  
  //Representar(Maze2,5000);
  //lc.setLed(0,5,5,true);
  //obstaculizar(matriz);
 
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      obstaculizar(matriz);
      solveMaze(matriz);
      lc.clearDisplay(0);
    }
  }
  
  //solveMaze(matriz);
  //lc.clearDisplay(0);
  //lc.setLed(0,0,0,true);
  

}
