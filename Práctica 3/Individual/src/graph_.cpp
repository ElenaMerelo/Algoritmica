/*Graph Representation*/

#include <iostream>
#include <set>
#include <list>
#include <algorithm>
#include <queue>

using namespace std;

/*-----------BASIC-MATRIX-GRAPH-----------*/

struct node
{
    int label;
    pair<int, int> coord;

    node(){label=0;}

    node(int label_, pair<int, int> coord_)
    {
        label=label_;
        coord=coord_;
    }

    int get_x(){ return coord.first; }
    int get_y(){ return coord.second; }
};

class graph
{
    private:
        vector<vector<float> > weights;
        vector<node> nodes;
    public:
        //Init matrix with 0's.
        graph(int n){ weights.resize(n); for(int i=0; i<n; i++) weights[i].resize(n, 0); }

        void set_weight(int origin, int destination, int d){ weights[origin][destination]=d; }

        float get_weight(int origin, int destination) const { return weights[origin][destination]; }
        
        vector<node> get_nodes(){ return nodes; }

        void add_node(pair<int, int> coord){ nodes.push_back( node(nodes.size(), coord)); }

        int size(){ return weights.size(); }
};

