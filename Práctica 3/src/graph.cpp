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

        int begin(int i){ return 0; }

        int end(int i){ return weights.size(); }
};

class heap
{
    private:
        vector<bool> inner_vertexs; 
        set<pair<int, int> > distance_heap;

    public: 
        heap(int n){inner_vertexs.resize(n, false); }

        bool in_heap(int v){ return inner_vertexs[v]; }

        void insert(int d, int v){ distance_heap.insert(pair<int, int>(d, v)); inner_vertexs[v]=true; }

        void update(int d, int v)
        {
            set<pair<int, int> >::iterator it;
            
            for(it=distance_heap.begin(); it!=distance_heap.end() && it->second!=v; ++it)
            
            if(it==distance_heap.end()){ cout << "Error: vertex doesnt exist" << endl; exit(0); }

            distance_heap.erase(it);
            distance_heap.insert( pair<int, int>(d, v) );
        }

        bool empty() const{ return distance_heap.empty(); }

        pair<int, int> erase_min()
        {
            pair<int, int> min = *(distance_heap.begin());
            distance_heap.erase( distance_heap.begin() );
            return min;
        }
};
