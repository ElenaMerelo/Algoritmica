
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

  // t.convenience.show();
  cout << t.cost_of_the_trivial_solution();

  // int level=0;
  // list<node>::iterator it=t.t[0].begin();
  // while(level!=n)
  // {
  //   cout << (*it).label << " ";
  //   it=(*(*it).children.begin());
  //   level++;
  // }

}
