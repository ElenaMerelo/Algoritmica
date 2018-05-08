
// g++ -std=c++11 ./src/main.cpp -o ./bin/main

#include <iostream>
#include <stdlib.h>
#include <vector>
#include "convenience.cpp"
#include "auxiliar.cpp"

using namespace std;

#define SHOW_EMPIRIC 1

static int count = 0; //numero de soluciones calculadas

void to_s(const vector<int>& v) {
  cout << "combination no " << (++count) << ": [ ";
  for (int i = 0; i < v.size(); ++i) { cout << v[i] << " "; }
  cout << "] " << endl;
}


vector<int> solution; //mejor solucuion
int cost; //coste de la solucion
int aux_cost; //coste auxiliar para calcular el maximo

void backtracking(ConvenienceMatrix & c, vector<int> v)
{
  vector<int> available=supplementary(v, c.size()); //definida en 'auxliar.cpp'
  for(int i=0; i<available.size(); i++)
  {
    v.push_back(available[i] ); //aniadimos el siguiente numero que no este ya en la solucion
    if( v.size() == c.size() ) ++count;//to_s(v); //descomentar para ver todas las posibilidades
    aux_cost=c.costs(v);
    if(aux_cost > cost  ) //guardamos la solucion con el mayor coste
    {
      cost=aux_cost;
      solution=v;
    }

    else backtracking(c, v); //recursividad
    
    v.pop_back(); //eliminamos el elemento aniadido antes para calcular
                  //la siguiente posibilidad
  }
}



int main(int argc, const char **argv) 
{

  if( argc != 2 )
  {
    cout << "Format: " << argv[0] << " <n>" << "\n";
    exit(1);
  }

  clock_t tantes;    // Valor del reloj antes de la ejecución
  clock_t tdespues; // Valor del reloj después de la ejecución

  int n=atoi(argv[1]);
  ConvenienceMatrix c(n);

  // for(int i=0; i<n; i++){ solution.push_back(i); }
  // cost=c.costs(solution);
  
  tantes = clock();
  backtracking(c, vector<int>());
  tdespues = clock();

  #if SHOW_EMPIRIC  
    cout << n << " " << ((double)(tdespues-tantes))/CLOCKS_PER_SEC << endl;
  #else
    cout << "Mejor solucion: ";
    to_s(solution);
    cout << "Coste de mejor solución: " << cost << endl;
    cout << "Soluciones totales calculadas " << count << endl;
  #endif
}
