/**
   @file Ordenaci�n por selecci�n
*/

   
#include <iostream>
using namespace std;
#include <ctime>
#include <cstdlib>
#include <climits>
#include <cassert>
#include <chrono>





/* ************************************************************ */ 
/*  M�todo de ordenaci�n por selecci�n  */

/**
   @brief Ordena un vector por el m�todo de selecci�n.

   @param T: vector de elementos. Debe tener num_elem elementos.
             Es MODIFICADO.
   @param num_elem: n�mero de elementos. num_elem > 0.

   Cambia el orden de los elementos de T de forma que los dispone
   en sentido creciente de menor a mayor.
   Aplica el algoritmo de selecci�n.
*/
inline static 
void seleccion(int T[], int num_elem);



/**
   @brief Ordena parte de un vector por el m�todo de selecci�n.

   @param T: vector de elementos. Tiene un n�mero de elementos 
                   mayor o igual a final. Es MODIFICADO.
   @param inicial: Posici�n que marca el incio de la parte del
                   vector a ordenar.
   @param final: Posici�n detr�s de la �ltima de la parte del
                   vector a ordenar. 
		   inicial < final.

   Cambia el orden de los elementos de T entre las posiciones
   inicial y final - 1de forma que los dispone en sentido creciente
   de menor a mayor.
   Aplica el algoritmo de selecci�n.
*/
static void seleccion_lims(int T[], int inicial, int final);



/**
   Implementaci�n de las funciones
**/

void seleccion(int T[], int num_elem)
{
  seleccion_lims(T, 0, num_elem);
}

static void seleccion_lims(int T[], int inicial, int final)
{
  int i, j, indice_menor;
  int menor, aux;
  for (i = inicial; i < final - 1; i++) {
    indice_menor = i;
    menor = T[i];
    for (j = i; j < final; j++)
      if (T[j] < menor) {
	indice_menor = j;
	menor = T[j];
      }
    aux = T[i];
    T[i] = T[indice_menor];
    T[indice_menor] = aux;
  };
}



int main(int argc, char * argv[])
{

  int n = atoi(argv[1]);

  int * T = new int[n];
  assert(T);

  srandom(time(0));

  for (int i = 0; i < n; i++)
    {
      T[i] = random();
    };


  const int TAM_GRANDE = 2000;
  const int NUM_VECES = 100;

  chrono::high_resolution_clock::time_point t_antes, t_despues, t_antes_vacio, t_despues_vacio;
  chrono::duration<double> t_transcurrido, t_transcurrido_vacio;

  if (n > TAM_GRANDE)
    {
    t_antes = chrono::high_resolution_clock::now();
      
    seleccion(T, n);
      
    t_despues = chrono::high_resolution_clock::now();

    t_transcurrido = chrono::duration_cast<chrono::duration<double> >(t_despues - t_antes);

    cout << n << " " << t_transcurrido.count() << endl;

    } else {

    int * U = new int[n];
    assert(U);

    for (int i = 0; i < n; i++)
      U[i] = T[i];
    
    t_antes_vacio = chrono::high_resolution_clock::now();
    for (int veces = 0; veces < NUM_VECES; veces++)
  	{
      for (int i = 0; i < n; i++)
        U[i] = T[i];
    }
    t_despues_vacio = chrono::high_resolution_clock::now();

    t_transcurrido_vacio = chrono::duration_cast<chrono::duration<double> >(t_despues_vacio - t_antes_vacio);

    t_antes = chrono::high_resolution_clock::now();

    for (int veces = 0; veces < NUM_VECES; veces++)
    {
      for (int i = 0; i < n; i++)
        U[i] = T[i];
        seleccion(U, n);
    }
      t_despues = chrono::high_resolution_clock::now();
      t_transcurrido = chrono::duration_cast<chrono::duration<double> >(t_despues - t_antes);
      cout << n << " " << t_transcurrido.count() + t_transcurrido_vacio.count() << endl;
      delete [] U;
    }

  delete [] T;

  return 0;
};
