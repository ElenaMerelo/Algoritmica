
// g++ -std=c++11 ./src/backtracking.cpp -o ./bin/backtracking

#include <iostream>
#include "tree.cpp"
#include <vector>

using namespace std;

int main(int argc, const char **argv) 
{

  if( argc != 2 )
  {
    cerr << "Format: " << argv[0] << " <n>" << endl;
    exit(1);
  }

  int n=atoi(argv[1]);

  tree t(n);
  t.generate_level(2);
  t.generate_level(3);
  t.generate_level(4);
  t.generate_level(5);
  t.show_ways();
  
}
