#include <iostream>
#include <fstream>
#include <cstdlib>
#include "graph.cpp"

using namespace std;

#define BUFFERSIZE 50

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

        //TODO: Hay que rellenar el grafo con los nodos.
    }

    //TODO: Calcular distancias entre todos los nodos.
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