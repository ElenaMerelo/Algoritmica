#include <iostream>
#include "ArbolBinario.cpp"

using namespace std;


int main(int argc, const char * argv[]) {
  ArbolBinario<int> tree;

  tree.asigna_raiz(1);

  tree.insertar_derecha()
  tree.insertar_izquierda(3);

  return 0;
}
