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


bool cycle(vector<edge>& v)
{
    vector<node> nodes;
    for(int i=0; i<v.size(); i++)
    {
        nodes.push_back(v[i].p1);
        nodes.push_back(v[i].p2);       
    }

    for(int i=0; i<nodes.size(); i++)
        for(int j=0; j<nodes.size(); j++)
           //TODO: Equal operator for struct node.
            if( nodes[i].coord.first == nodes[j].coord.first && nodes[i].coord.second == nodes[j].coord.second && i!=j) return true;
    
    return false;
}

vector<edge> kruskal(graph &g)
{
    //First we create a vector with each edge of the grap.
    vector<edge> edges;
    vector<node> nodes=g.get_nodes();

    for(int i=0; i<g.size(); i++)
        for(int j=i; j<g.size(); j++)
            edges.push_back(edge(nodes[i],nodes[j],g.get_weight(i,j)));

    sort(edges.begin(), edges.end(), compare_by_weight); //Sort the vector of edges.

    // for(int i=0; i<edges.size(); i++) cout << edges[i].weight << endl;

    vector<edge> solution;
    struct edge candidate;

    while(edges.size())
    {
        candidate=edges[0];

        solution.push_back(candidate);

        if( cycle(solution) )
            solution.pop_back();

        edges.erase(edges.begin());
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

    vector<edge> way=kruskal(G);


    cout << "DIMENSION: " << dimension << endl;
    for(int i=0; i<way.size(); i++){
        cout << way[i].p1.label << " " << way[i].p1.coord.first << " " << way[i].p1.coord.second << endl;
        cout << way[i].p2.label << " " << way[i].p2.coord.first << " " << way[i].p2.coord.second << endl;    
    }
    

}