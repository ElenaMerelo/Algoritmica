#include <iostream>
#include <vector>
#include <list>
#include "tree.cpp"
#include "convenience.cpp"


#ifndef __BACKTRACKING__
#define __BACKTRACKING__

vector<int> generate_children(tree & t, int level, list<node>::iterator father)
{
  vector<int> taken, children;
  int level_=level;
  list<node>::iterator current_father=father;
  if( level==0 )
    taken.push_back( (*father).label );
  else
  {
    while( level_>=0 )
    {
      taken.push_back( (*current_father).label );
      current_father= (*current_father).father;
      level_--;
    }
  }

  return supplementary(taken, t.levels());
}

void generate_level(tree & t, int level)
{
  vector<int> new_children;
  list<node>::iterator it;
  list<list<node> >::iterator it_=t.get_it(level-1);
  list<list<node> >::iterator it__=t.get_it(level); //level to insert nodes

  for( it=(*it_).begin(); it!=(*it_).end(); it++)
  { 
    new_children=generate_children(t, level-1, it);
    for(int i=0; i<new_children.size(); i++){ (*it__).push_back( node(it, level, new_children[i]) ); }
  }
}

void show_ways(tree & t)
{
    int level, k=0;
    list<node>::iterator current_father;
    vector<int> aux;
    list<list<node> >::iterator it_=t.get_it(t.levels()-1);
    for(list<node>::iterator it=(*it_).begin(); it!=(*it_).end(); it++)
    {
        cout << "Camino x: ";
        level=t.levels()-1;
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
        cout << "Coste del camino:(" << k++ << ") " << "2" << endl;
        }
}

#endif