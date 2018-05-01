

/*-----------Solution with n-electricians---------*/

//g++ ./src/n-electrician.cpp -o ./bin/n-electrician; ./bin/n-electrician ./datosTSP/att48.tsp n > ./datosTSP/OPTIMOS.opt.tour

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

bool is_in(int j, vector<node>& v)
{
    for(int i=0; i<v.size(); i++)
    {
        if( v[i].label==j )
            return true;
    }
    return false;
}

bool cycle(vector<node>& v, struct node new_node){ return is_in(new_node.label, v); }

int nodeHeuristic(graph &g, struct node n, float min_bound, vector<node> taken){
    float min=LONG_MAX;
    int index_min=-1;
    float current_weight;
    
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

vector<vector<node> > greedy(graph &g, int begin, int n_electricians)
{
    vector<node> nodes=g.get_nodes();
    vector<vector<node> > solution(n_electricians);
    vector<node> taken;

    int index_nextNode;

    struct node current_node=nodes[begin];
    taken.push_back(current_node);
    bool first_time=true;
    
    for(int i=0; i<n_electricians; i++)
        solution[i].push_back(current_node);


    struct node nextNode;
    first_time=true;
    int j=0;
    while( taken.size() < nodes.size() )
    {
        if(j==n_electricians) j=0;
        current_node=solution[j].back();
        /*------------searching next node-----------*/
        first_time=true;
        do
        {
            index_nextNode=nodeHeuristic(g, current_node, (first_time)? 0:g.get_weight(current_node.label, nextNode.label), taken);
            if( index_nextNode==-1 ) return vector<vector<node> >();
            nextNode=nodes[index_nextNode];
            
            first_time=false;
        } while( cycle( solution[j], nextNode ) );
        /*------------searching next node-----------*/
        solution[j].push_back(nextNode);
        taken.push_back(nextNode);
        j++;
    }

    return solution;
}


float total_weight(graph &g, vector<node>& way)
{
    float weight=0;
    for(int i=0; i<way.size()-1; i++) weight+=g.get_weight(way[i].label, way[i+1].label);
    return weight;
}   

int main(int argc, char **argv)
{
    if( argc!=3 )
    {
        cerr << "Error: incorrect number of parameters. \n";
        cerr << "Format: " << argv[0] << " <graph_file> <number-electricians>\n";
        exit(1);
    }

    ifstream f(argv[1], ios::in);
    int n=atoi(argv[2]);
    
    if(!f)
    {
        cerr << "Error: couldnt open " << argv[1] << "\n";
        exit(1);
    }

    int dimension=get_dimension(f);
    graph G(dimension);
    fill_graph(G, f);

    vector<vector<node> > way, minimal_way=greedy(G, 0, n);

    /*------Uncomment to find the minimal way applyins the heuristic for each node in the graph-----*/
    // float min=LONG_MAX, weight;
    // for(int i=0; i<G.size(); i++)
    // {
    //     way=greedy(G, i);
    //     weight=total_weight(G, way);

    //     if( weight < min )
    //     {
    //         minimal_way=way;
    //         min=weight;
    //     }
    // }

    /*-------SHOW THE GRAPH (in TSP format)--------*/
    if( !minimal_way.empty() )
    {
        cout << "DIMENSION: " << dimension << endl;
        for(int i=0; i<minimal_way.size(); i++)
        {
            cout << "Camino del electricista " << i+1 << ":" << endl;
            for(int j=0; j<minimal_way[i].size(); j++)
                cout << minimal_way[i][j].label+1 << " " << minimal_way[i][j].coord.first << " " << minimal_way[i][j].coord.second << endl;
        }
    } 
    else
    {
        cout << "El problema no tiene solucion con el nodo inicial 0" << endl;
    }
}