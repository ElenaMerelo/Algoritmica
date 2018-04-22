#include <iostream>
#include <fstream>
#include <math.h>
#include <cstdlib>
#include "graph.cpp"
#include <algorithm>

using namespace std;

#define BUFFERSIZE 50

struct edge
{
    pair<int, int> coord;
    float weight;

    edge(int x, int y, float weight_)
    {
        coord.first=x;
        coord.second=y;
        weight=weight_;
    }
};

float euclidean_distance(pair<int, int> x1, pair<int, int> x2){ return sqrt((x1.first-x2.first)^2 + (x1.second-x2.second)^2); }

int get_dimension(ifstream &f)
{
    char buffer[BUFFERSIZE], aux;

    f.seekg(0, ios::beg); //At the beginning of the file.

    f.getline(buffer, BUFFERSIZE, ' '); //To delete the word 'DIMENSION: '.
    f.getline(buffer, BUFFERSIZE);

    int dimension=atoi(buffer);
    

    return dimension;
}

void fill_graph(graph &g, ifstream &f)
{
    char buffer[BUFFERSIZE], aux;
    char number[5];
    int x, y;

    for(int i=0; i<3; i++)
    {

        f.getline(buffer, BUFFERSIZE);

        for(int j=0; j<3; j++)
            number[j]=buffer[j+4];        
        x=atoi(number);
        for(int j=0; j<3; j++)
            number[j]=buffer[j+8];        
        y=atoi(number);

        g.add_node( pair<int, int>(x,y) );
    }

    vector<node> nodes=g.get_nodes();
    for(int i=0; i<nodes.size(); i++){ //FIXME: Matrix is simetric.
        for(int j=0; j<nodes.size(); j++){
            if(i==j)
                g.set_weight(i,j,0.0);
            else
                g.set_weight(i,j, euclidean_distance(nodes[i].coord, nodes[j].coord) );
        }
    }
}


bool compare_by_weight(const edge& a, const edge& b){ return a.weight < b.weight; }


bool cycle(vector<edge>& v)
{
    for(int i=0; i<v.size(); i++)
    {
        for(int j=0; j<v.size(); j++)
        {
            if( v[i].coord.first == v[j].coord.first && i!=j)
                return true;
            if( v[i].coord.second == v[j].coord.second && i!=j)
                return true;

        }
    }
    return false;
}

vector<edge> kruskal(graph &g)
{
    //First we create a vector with each edge of the grap.
    vector<edge> edges; 
    for(int i=0; i<g.size(); i++)
        for(int j=i; j<g.size(); j++)
            edges.push_back(edge(i,j,g.get_weight(i,j)));

    sort(edges.begin(), edges.end(), compare_by_weight);
}

int main(int argc, char **argv)
{
    if( argc!=2 )
    {
        cerr << "Error: incorrect number of parameters. \n";
        cerr << "Format: " << argv[0] << " <graph_file>\n";
        exit(1);
    }

    ifstream f(argv[1], ios::in);

    if(!f)
    {
        cerr << "Error: couldnt open " << argv[1] << "\n";
        exit(1);
    }

    int dimension=get_dimension(f);

    graph G(dimension);

    fill_graph(G, f);
}