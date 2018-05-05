
#include<iostream>
#include<list>
#include<vector>
#include <stdlib.h>

using namespace std;

struct conv
{
  int person;
  int convenience;
};

void make_conveniencie_matrix(vector<vector<conv> > & v, int n)
{
  v.resize(n);
  for(int i=0; i<n; i++) v[i].resize(n-1);

  int k=0;
  for(int i=0; i<v.size(); i++)
  {
    for(int j=0; j<v.size()-1; j++)
      {
        v[i][j].person=( j>=i ) ? j+1:j;
        v[i][j].convenience=rand() % 100; //""""pseudo-random"""" 
      }
    k+=1;
  }
}

class tree
{
  private:
    
    struct node
    {

      node(list<node>::iterator f, int l)
      {
        father=f;
        label=l;
      }

      list<node>::iterator father;
      int label;

    };

    vector<list<node> > t;
    vector<vector<conv> > convenience;
    int n; //size

  public:
    
    tree(int n_)
    { 
      n=n_; 
      t.resize(n);
      generate_level(0);
      make_conveniencie_matrix(convenience, n);
    }
    
    list<node>::iterator search_node(int level, list<node>::iterator father, int label)
    {
      list<node>::iterator it=t[level].begin();
      while( (*it).father!=father && (*it).label!=label ){ it++; }
      return it;
    }

    bool is_in(vector<int> & v, int x)
    {
      for(int i=0; i<v.size(); i++) if(v[i]==x) return true;
      return false;
    }

    vector<int> generate_children(int level, list<node>::iterator father)
    {
      vector<int> taken, children;
      int level_=level;
      list<node>::iterator current_father=father;

      while(level_>=0)
      {
        taken.push_back((*current_father).label);
        current_father=(*current_father).father;
        level_--;
      }

      return supplementary(taken);
    }

    //supplementary of v respect to [0...n-1]
    vector<int> supplementary(vector<int> & v)
    {
      vector<int> result;
      for(int i=0; i<n; i++) if( !is_in(v, i) ) result.push_back(i);
      return result;
    }

    void generate_level(int level)
    {
      if(level==0)
      {
        //element 'father' in t[0] nodes is never used
        for(int i=0; i<n; i++) t[0].push_back( node( t[0].begin() , i) );
        vector<int> aux;
        list<node>::iterator it=t[0].begin();
        for(int i=0; i<n; i++){
          aux.clear(); aux.push_back(i);
          aux=supplementary(aux);
          for(int j=0; j<aux.size(); j++) t[1].push_back( node(it, aux[j]) );
          it++;
        }
      }
      else
      {
        vector<int> new_children;
        list<node>::iterator it;

        for(it=t[level-1].begin(); it!=t[level-1].end(); it++)
        {
          new_children=generate_children(level-1, it);
          for(int i=0; i<new_children.size(); i++) t[level].push_back( node( it, new_children[i]) );
        }
      }
    }

    //Show the tree by 'parent-levels'.
    void show()
    {
      int k=0;
      for(int i=0; i<t.size(); i++)
      {
        k=0;
        cout << "\nLevel " << i << ":\n";
        for( list<node>::iterator it=t[i].begin(); it!=t[i].end(); it++ )
        {
          if( k==0 ){ cout << "\n/***/" << (*(*it).father).label << "/***/ "; }
          cout << (*it).label << " ";
          k = ( k==n-i-1 ) ? 0:k+1;
        }
        cout << "\n";
      }
    }

    //Show all the calculated possibilities.
    void show_ways()
    {
      int level, k=0;
      list<node>::iterator current_father;
      vector<int> aux;
      for(list<node>::iterator it=t[n-1].begin(); it!=t[n-1].end(); it++)
      {
        cout << "Camino x: ";
        level=n-1;
        current_father=it;
        aux.clear();
        while( level>=0 )
        {
          cout << (*current_father).label << " ";
          aux.push_back((*current_father).label);
          current_father=(*current_father).father;
          level--;
        }
        cout << endl;
        cout << "Coste del camino:(" << k++ << ") " << costs(aux) << endl;
      }
    }

    //Show convenience's values
    void show_convenience()
    {
      for(int i=0; i<n; i++){
        for(int j=0; j<n-1; j++)
          cout << convenience[i][j].person << ": " <<  convenience[i][j].convenience << "\t";
        cout << endl;
      }
    }

    int getConv(int i, int j)
    {
      int k=0;
      while( convenience[i][k].person!=j ) { k++; }
      return convenience[i][k].convenience;
    }

    //Calculate the cost of a given solution 'v'.
    int costs(vector<int> & v)
    {
      int cost=0;
      for(int i=1; i<n-1; i++) cost += getConv(i,i-1) + getConv(i,i+1);
      cost += getConv(0,n-1) + getConv(0,1);
      cost += getConv(n-1, n-2) + getConv(n-1,0);      
    }

    //considering the [0,1,...,n-1] as trivial solution
    int cost_of_the_trivial_solution()
    {
      vector<int> trivial_solution;
      for(int i=0; i<n; i++) trivial_solution.push_back(i);
      return costs(trivial_solution);
    }
};