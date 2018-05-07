
// g++ -std=c++11 ./src/backtracking.cpp -o ./bin/backtracking

#include <iostream>
#include <stdlib.h>
#include "tree.cpp"
#include <vector>
#include "backtracking.cpp"

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
  t.set_root( node( list<node>::iterator(), -1, -1 ) );

  for(int i=0; i<n; i++)
    t.insert_node( t.get_root(), i );

  generate_level(t, 1);
  generate_level(t, 2);
  
  t.show_nodes_by_levels();
  
  list<list<node> >::iterator it=t.get_it(0);
  t.chop( (*(*it).begin()) );

  t.show_nodes_by_levels();
  // show_ways(t);

}
