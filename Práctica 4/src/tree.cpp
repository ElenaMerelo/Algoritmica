
#include<iostream>
#include<list>
#include<vector>

using namespace std;

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
    int n; //size

  public:
    
    tree(int n_)
    { 
      n=n_; 
      t.resize(n);
      generate_level(0);
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

      list<node>::iterator current_father=father;

      if(level==0)
      {
        taken.push_back((*father).label);
      }

      return supplementary(taken);
    }

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
        list<node>::iterator it;
        vector<int> new_children;
        for(it=t[level-1].begin(); it!=t[level-1].end(); it++)
        {
          new_children=generate_children(level-1, (*it).father);
          for(int i=0; i<new_children.size(); i++) t[level].push_back( node( it, new_children[i]) );
        }
      }
    }

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
};