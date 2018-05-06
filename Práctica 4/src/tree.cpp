
#include<iostream>
#include<list>
#include<vector>
#include <stdlib.h>
#include "convenience.cpp"

using namespace std;

struct node
{

  node(list<node>::iterator f, int l)
  {
    father=f;
    label=l;
  }

  int label;
  list<node>::iterator father;
  vector< list<node>::iterator > children;
};

class tree
{
  private:

    int n; //size
    ConvenienceMatrix convenience;
    list<list<node> > t;

  public:
    
    tree(int n_)
    { 
      t.resize(n_);
      n=n_;
      generate_level(0);
      convenience = ConvenienceMatrix(n);
    }
    
    list<node>::iterator search_node(int level, list<node>::iterator father, int label)
    {
      list<list<node> >::iterator it_=t.begin(); advance(it_, level);
      list<node>::iterator it=(*it_).begin(); 
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
      if ( level==0 )
        taken.push_back( (*father).label );
      else
      {
        while(level_>=0)
        {
          taken.push_back((*current_father).label);
          current_father=(*current_father).father;
          level_--;
        }
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
        for(int i=0; i<n; i++) { (*t.begin()).push_back( node( (*t.begin()).begin() , i) ); } 
      }
      else
      {
        vector<int> new_children;
        list<node>::iterator it;
        list<list<node> >::iterator it_=t.begin(), it__=t.begin();
        advance(it_, level-1); advance(it__, level);

        for( it=(*it_).begin() ; it!=(*it_).end(); it++)
        {
          new_children=generate_children(level-1, it);
          for(int i=0; i<new_children.size(); i++){
            (*it__).push_back( node( it, new_children[i]) );
            (*it).children.push_back( search_node(level, it, new_children[i]) ); cout << "a";
          }
        }
      }
    }

    void generate_all_levels()
    {
      for(int i=1; i<n; i++) generate_level(i);
    }

    void delete_nodes(list<node>::iterator it, int level)
    {
      if( !(*it).children.empty() )
      {
        for( int i=0; i<(*it).children.size(); i++ ) delete_nodes((*it).children[i], level+1);
        list<list<node> >::iterator it_=t.begin(); advance(it_, level);
        (*it_).erase(it);        
      }
      else
      {
        vector< list<node>::iterator >::iterator i=(*(*it).father).children.begin();
        while( (*i)!=it ) i++; (*(*it).father).children.erase(i);

        list<list<node> >::iterator it_=t.begin(); advance(it_, level);
        (*it_).erase(it);
      }
    }


    //Show all the calculated possibilities.
    void show_ways()
    {
      int level, k=0;
      list<node>::iterator current_father;
      vector<int> aux;
      for(list<node>::iterator it=t.back().begin(); it!=t.back().end(); it++)
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
        cout << "Coste del camino:(" << k++ << ") " << convenience.costs(aux) << endl;
      }
    }

    //considering the [0,1,...,n-1] as trivial solution
    int cost_of_the_trivial_solution()
    {
      vector<int> trivial_solution;
      for(int i=0; i<n; i++) trivial_solution.push_back(i);
      return convenience.costs(trivial_solution);
    }

    int backtracking()
    {
      int current_cost=cost_of_the_trivial_solution();
      return current_cost;
    }

};