
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
  t.show_convenience();
  cout << t.cost_of_the_trivial_solution() << endl;
  vector<int> a;
  a.push_back(3); a.push_back(5);a.push_back(1); a.push_back(4); a.push_back(0); a.push_back(2);
  cout << t.costs(a) << endl;
  
}
