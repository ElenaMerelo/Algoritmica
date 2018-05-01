
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

    graph G(argv[1]);

    double l;
    vector<int> solution;
    
    solution=G.min_path1(0, l);
    cout << "Algoritmo 1: (ciudad 1) " << "Peso: " << l << endl;
    show_vector(solution);

    solution=G.min_path2(l);
    cout << "Algoritmo 2: " << "Peso: " << l << endl;
    show_vector(solution);

    solution=G.min_path3(0, l);
    cout << "Algoritmo 3: (triangulo desde ciudad 1) " << "Peso: " << l << endl;
    show_vector(solution);


}