
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
      generate_level(0);
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

      taken.push_back(label); //add current child

      int current_level=level-1; //because we have already added the node associated to level 'level'
      int current_father=father;
      int current_label=label;      

      list<node>::iterator it;


      while( current_level >= 0 )
      {
        it=search_node(current_level, current_father, current_label);
        current_level--;
        current_father=(*it).father;
        current_label=(*it).label;
        taken.push_back(current_label);
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

    void show()
    {
      for(int i=0; i<t.size(); i++)
      {
        cout << "Level " << i << ":\n";
        for( list<node>::iterator it=t[i].begin(); it!=t[i].end(); it++ )
          cout << (*it).label << " ";
        cout << "\n";
      }
    }
};