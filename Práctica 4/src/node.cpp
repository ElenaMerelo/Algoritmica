#include <iostream>
#include <list>
#include <vector>

#ifndef __NODE__
#define __NODE__


using namespace std;

struct node
{

    int label;
    int level;

    list<node>::iterator father;
    vector< list<node>::iterator > children;

    node(){level=-1;};

    node(list<node>::iterator father_, int level_, int label_)
    {
        father=father_;
        level=level_;
        label=label_;
    }

};

#endif