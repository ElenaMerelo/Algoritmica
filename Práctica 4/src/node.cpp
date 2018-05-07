#include <iostream>
#include <list>
#include <vector>

using namespace std;

struct node
{

    int label;

    list<node>::iterator father;
    vector< list<node>::iterator > children;

    node(list<node>::iterator f, int l)
    {
        father=f;
        label=l;
    }

};
