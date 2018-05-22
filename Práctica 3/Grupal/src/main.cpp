
// g++ ./src/main.cpp -o ./bin/main

#include "graph.cpp"

void show_vector(vector<int> r){
    for(int i=0; i<r.size(); i++)
        //Internamente representamos las ciudades desde 0 hasta N-1.
        cout << r[i]+1 << " "; 
    cout << endl; 
}


int main(int argc, char **argv){
    
    if(argc != 2){
        cout << "Número incorrecto de argumentos";
        exit(-1);
    }


    graph G(1);

    clock_t tbefore;
    clock_t tafter;


    double l;
    vector<int> solution;

    for(int i=1; i<16; i++){

    G = graph(i);
    tbefore=clock();    
    solution=G.min_path1(0, l);
    tafter=clock();

    printf("%d %5.9f\n", i, ((double)(tafter-tbefore))/CLOCKS_PER_SEC );

    }
}