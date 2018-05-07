
// g++ -std=c++11 ./src/backtracking.cpp -o ./bin/backtracking

#include <iostream>
#include <stdlib.h>
#include "tree.cpp"
#include <vector>

using namespace std;

void generate_level(tree & t, int level)
{
  vector<int> new_children;
  list<node>::iterator it;
  list<list<node> >::iterator it_=t.get_it(level);

  for( it=(*it_).begin(); it!=(*it_).end(); it++ )
  {
    new_children=generate_children(level-1, it);
  }
}

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

  t.show_nodes_by_levels();

}
