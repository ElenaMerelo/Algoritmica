#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include <cstdlib>
#include "graph.cpp"
#include <algorithm>
#include <string>

using namespace std;

#define BUFFERSIZE 50

struct edge
{
    struct node p1;
    struct node p2;
    float weight;

    edge(){};

    edge(struct node p1_, struct node p2_, float weight_)
    {
        p1=p1_;
        p2=p2_;
        weight=weight_;
    }
};

float euclidean_distance(pair<int, int> x1, pair<int, int> x2){ return sqrt( pow(x1.first-x2.first,2) + pow(x1.second-x2.second,2) ); }

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
    string line, aux;
    int row[3], i;
    vector< vector<int> > all_integers;
    while ( getline( f, line ) ) {
        stringstream is( line );
        i=0;
        while(i<3)
        {
            getline(is, aux, ' ');
            row[i]=atoi(aux.c_str());
            i++;
        }

        g.add_node( pair<int, int>(row[1],row[2]) );
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


bool cycle(vector<edge>& v, struct edge new_edge)
{
    bool inH=false, inI=false;
    for(int i=0; i<v.size(); i++)
    {
        if( v[i].p1.coord==new_edge.p1.coord )
            inH=true;
        if( v[i].p1.coord==new_edge.p1.coord )
            inI=true;
    }

    if( (!inH && !inI) || (!inH && inI) || (inH && !inI) )
        return false;
    else
        return true;

}

vector<edge> generate_all_edges(graph &g)
{
    vector<node> nodes=g.get_nodes();
    vector<edge> edges;

    for(int k=0; k<g.size()-1; k++)
        for(int i=0; i<g.size(); i++)
            for(int j=i+k+1; j<g.size(); j++)
                edges.push_back(edge(nodes[i],nodes[j],g.get_weight(i,j)));

    return edges;

}

vector<edge> kruskal(graph &g)
{
    //First we create a vector with each edge of the grap.
    vector<edge> edges=generate_all_edges(g);

    sort(edges.begin(), edges.end(), compare_by_weight); //Sort the vector of edges.

    vector<edge> solution;

    while(edges.size())
    {
        if( !cycle(solution, edges[0]) )
            solution.push_back(edges[0]);

        edges.erase(edges.begin());
    }

    return solution;
}


int nodeHeuristic(graph &g, int i, float value_default=0){
    float min=value_default;
    int index_min=-1;
    for(int j=i+1; j<g.size(); j++)
    {
        if( g.get_weight(i, j) < min )
        {
            min=g.get_weight(i,j);
            index_min=j;
        }
    }
    return index_min;
}

vector<edge> kruskal_heuristic(graph &g)
{
    vector<edge> solution;    
    vector<node> nodes=g.get_nodes();

    struct node init=nodes[0]; //Heuristic starts with the first node on the file.

    int nextNode, aux;
    float weight=0;
    struct edge nextEdge;
    while( nodes.size() )
    {
        cout << "HOLA" << endl;
        weight=0;
        do
        {
            nextNode=nodeHeuristic(g, init.label-1, weight);
            if( nextNode == -1) return vector<edge>(); //NP if the heuristic can not find a node

            nextEdge=edge(init, nodes[nextNode], g.get_weight(init.label-1,nextNode));

            weight=nextEdge.weight;

        } while( cycle(solution, nextEdge) );

        
        solution.push_back(nextEdge);
        
        aux=init.label;
        init=nodes[nextNode];
        nodes.erase(nodes.begin()+aux);
    }

    return solution;
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

    vector<edge> way=kruskal_heuristic(G);

    if( !way.empty() )
    {
        cout << "DIMENSION: " << dimension << endl;
        for(int i=0; i<way.size(); i++)
        {
            cout << way[i].p1.label << " " << way[i].p1.coord.first << " " << way[i].p1.coord.second << endl;
            cout << way[i].p2.label << " " << way[i].p2.coord.first << " " << way[i].p2.coord.second << endl;    
        }
    } 
    else
    {
        cout << "El problema no tiene solucion con el nodo inicial 0" << endl;
    }
    

}