#include <iostream>
#include <fstream>
#include <math.h>
#include <cstdlib>
#include "graph.cpp"

using namespace std;

#define BUFFERSIZE 50

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