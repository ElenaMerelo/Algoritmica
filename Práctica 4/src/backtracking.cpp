
// g++ -std=c++11 ./src/backtracking.cpp -o ./bin/backtracking

#include <iostream>
#include <stdlib.h>
#include "tree.cpp"
#include <vector>

using namespace std;

int main(int argc, const char **argv) 
{

  if( argc != 2 )
  {
    cout << "Format: " << argv[0] << " <n>" << "\n";
    exit(1);
  }

  int n=atoi(argv[1]);

  tree t(n);
  t.generate_all_levels();

  cout << "ANTES DE BORRAR" << endl;
  // t.show_ways();


  list<list<node> >::iterator it=t.t.begin(); it++;
  list<node>::iterator it_=(*it).begin();
  cout << (*it_).label <<  " " << (*(*it_).father).label << endl;
  t.delete_nodes(it_, 1);



  cout << "DESPUES DE BORRAR" << endl;
  // t.show_ways();

}
