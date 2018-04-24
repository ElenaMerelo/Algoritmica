
//g++ ./src/electrician.cpp -o ./bin/electrician; ./bin/electrician ./datosTSP/att48.tsp > ./datosTSP/OPTIMOS.opt.tour

#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include <cstdlib>
#include "graph.cpp"
#include <algorithm>
#include <string>
#include <limits.h>

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

bool cycle(vector<node>& v, struct node new_node){ return is_in(new_node.label, v); }

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


bool is_in(int j, vector<node>& v)
{
    for(int i=0; i<v.size(); i++)
    {
        if( v[i].label==j )
            return true;
    }
    return false;
}

int nodeHeuristic(graph &g, struct node n, float weight, vector<node> taken){
    float min=LONG_MAX;
    int index_min=-1;
    float current_weight, min_bound=0;
    
    for(int i=0; i<g.size(); i++)
    {
        current_weight=g.get_weight(i, n.label);
        if( current_weight > 0 && current_weight < min && current_weight > min_bound && !is_in(i, taken) )
        {
            min=current_weight;
            index_min=i;
        }        
    }

    return index_min;
}

// vector<node> kruskal_heuristic(graph &g)
// {
//     vector<edge> solution;    
//     vector<node> nodes=g.get_nodes();

//     struct node init=nodes[2]; //Heuristic starts with the first node on the file.

//     int nextNode, aux;
//     float weight=0;
//     struct edge nextEdge;

//     nodes.erase(nodes.begin());

//     while( nodes.size() )
//     {
//         weight=0;
//         do
//         {
//             nextNode=nodeHeuristic(g, init.label, weight, solution, nodes);
//             if( nextNode == -1) return vector<node>(); //NP if the heuristic can not find a node

//             nextEdge=edge(init, nodes[nextNode], g.get_weight(init.label,nextNode));

//             weight=nextEdge.weight;
//         } while( cycle(solution, nextEdge) );

        
//         solution.push_back(nextEdge);

//         init=nodes[nextNode];
//         nodes.erase(nodes.begin()+nextNode);

//     }


//     vector<node> solution_;
//     solution_.push_back(solution[0].p1);
//     for(int i=0; i<solution.size(); i++) solution_.push_back(solution[i].p2);
//     return solution_;
// }

vector<node> kruskal_heuristic2(graph &g)
{
    vector<node> nodes=g.get_nodes();
    vector<node> solution;
    
    struct node current_node=nodes[0];
    bool first_time=true;
    solution.push_back(current_node);


    struct node nextNode;
    do
    {
        nextNode=nodes[nodeHeuristic(g, current_node, (first_time)? 0:g.get_weight(current_node.label, nextNode.label))];
        
        first_time=false;
    }

    return vector<node>();
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

    vector<node> way=kruskal_heuristic2(G);

    float min=LONG_MAX;



    if( !way.empty() )
    {
        cout << "DIMENSION: " << dimension << endl;
        for(int i=0; i<way.size(); i++)
            cout << way[i].label << " " << way[i].coord.first << " " << way[i].coord.second << endl;
    } 
    else
    {
        cout << "El problema no tiene solucion con el nodo inicial 0" << endl;
    }
    

}