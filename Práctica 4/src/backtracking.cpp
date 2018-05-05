
// g++ -std=c++11 ./src/backtracking.cpp -o ./bin/backtracking

#include <iostream>
#include "tree.cpp"
#include <vector>

using namespace std;

#define SHOW 0

void make_conveniencie_matrix(vector<vector<int> > & v, int n)
{
  v.resize(n);
  for(int i=0; i<n; i++) v[i].resize(n);

  int k=0;
  for(int i=0; i<v.size(); i++)
  {
    for(int j=0; j<v.size(); j++) if( i!=j ) v[i][j]=i+j+k;
    k+=1;
  }

}

void show_convenience(vector<vector<int> > & v)
{
  for(int i=0; i<v.size(); i++)
  {
    for(int j=0; j<v.size(); j++)
      cout << v[i][j] << "\t";
    cout << endl;
  }
}


int main(int argc, const char **argv) 
{

  if( argc != 2 )
  {
    cerr << "Format: " << argv[0] << " <n>" << endl;
    exit(1);
  }

  int n=atoi(argv[1]);

  vector<vector<int> > convenience;
  make_conveniencie_matrix(convenience, n);

  #if SHOW
    show_convenience(convenience);
  #endif

  tree t(n);
  t.generate_level(1);
  t.generate_level(2);  
  t.show();

}
