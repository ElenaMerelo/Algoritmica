
#include<iostream>
#include<list>
#include<vector>
#include <stdlib.h>
#include "auxiliar.cpp"
#include "convenience.cpp"
#include "node.cpp"

using namespace std;

class tree
{
  private:
    int n;
    struct node root;
    list<list<node> > nodes;


  public:
  
  tree(){};
  
  tree(int n_){ nodes.resize(n_); n=n_; }

  void set_root(struct node r){ root=r; }

  struct node get_root(){ return root; }

  list<list<node> >::iterator get_it(int level)
  {
    list<list<node> >::iterator it; advance(it, level);
    return it;
  }

  void insert_node(struct node n, int label)

  {
    list<list<node> >::iterator it_=nodes.begin();
    (*it_).push_back( node(list<node>::iterator(), 0, label ) );    
  }

  void insert_node(list<node>::iterator it, int label)
  {
    list<list<node> >::iterator it_=get_it( (*it).level+1 );
    (*it_).push_back( node(it, (*it).level+1, label ) );
    (*it).children.push_back( (*it_).end() );
  }

  list<node>::iterator search_node(struct node n)
  {
    list<list<node> >::iterator it_=get_it(n.level);
    list<node>::iterator it=(*it_).begin();
    while( (*it).father != n.father && (*it).label != n.label ) it++;
    return it;
  }

  void delete_node_internally(struct node n){ delete_node_internally( search_node(n) ); }

  void delete_node_internally(list<node>::iterator it)
  {
    list<list<node> >::iterator it_=get_it((*it).level);
    (*it_).erase(it);
  }

  void delete_nodes(list<node>::iterator it)
  {
    if( !(*it).children.empty() )
    {
      for(int i=0; i<(*it).children.size(); i++)
      {
        delete_nodes( (*it).children[i] );
        delete_node_internally( (*it).children[i] );
      }
    }
  }

  void chop( struct node n )
  {
    list<node>::iterator it=search_node(n);
    delete_nodes(it);
  }

  void show_nodes_by_levels()
  {
    int i=0;
    for( list<list<node> >::iterator it=nodes.begin(); it!=nodes.end(); it++ )
    {
      cout << "Level " << i++ << " :" << endl;
      for( list<node>::iterator jt=(*it).begin(); jt!=(*it).end(); jt++ )
        cout << (*jt).label << " ";
      cout << endl;
    }
  }
};