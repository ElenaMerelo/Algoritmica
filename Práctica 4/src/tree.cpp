
#include<iostream>
#include<list>
#include<vector>

using namespace std;

class tree
{
  private:
    
    struct node
    {

      node(int f, int l)
      {
        father=f;
        label=l;
      }

      int father;
      int label;

    };

    vector<list<node> > t;
    int n; //size

  public:
    
    tree(int n_)
    { 
      n=n_; 
      t.resize(n);
    }
    
    list<node>::iterator search_node(int level, int father, int label)
    {
      list<node>::iterator it=t[level].begin();
      do{ it++; } while( (*it).father!=father && (*it).label!=label );
      return it;
    }

    vector<int> generate_children(int level, int father, int label)
    {
      vector<int> taken, children;

      int current_level=level;

      while( current_level > 0 )
      {
        
      }

      return children;
    }

    void generate_level(int level)
    {
      if(level==0)
      {
        for(int i=0; i<n; i++) t[0].push_back( node(-1, i) );
      }
      else
      {
        list<node>::iterator it;
        for(it=t[level-1].begin(); it!=t[level-1].end(); it++)
        {
          vector<int> new_children=generate_children(level-1, (*it).father, (*it).label);
        }
      }
    }
};