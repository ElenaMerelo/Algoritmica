/**
 * @author Antonio Gamiz Delgado
 * @author Elena Merelo Molina
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <queue>
#include <limits.h>

#include "auxiliar.cpp"

using namespace std;

//Variable global para ver la diferencia entre a lo 'bruto' y 'ramificacion y poda'.
long long int nodes_calculated=0;
long long int bounds=0;
long long int max_queue_size=0;

/**
 * @brief Representacion de la matriz de pesos/adyacencia.
 *        - Los valores de la distancia entre dos ciudades se ha limitado
 *          a 100 ( rand() % 100 ).
 *        - Se considera que la matriz es simetrica.  
 */
struct weight {
    vector< vector<int> > weights;

    weight(int n) {
        weights.resize(n);
        srand(time(NULL));
        for( int i=0; i<n; i++ ) {
            for( int j=n-1-i; j<n; j++ ) {
                weights[i].push_back( rand() % 100 );
            }
        }
    }

    int get(int i, int j) {
        if( i > j) return weights[i][j];
        else if( i < j ) return weights[j][i];
        else return 0;
    }

    int size() { return weights.size(); }
};

/**
 * @brief Representacion de un nodo del arbol.
 * Formado por un int 'cs', que es el coste posible
 * (segun possible_cost) y un vector<int> 'path', que
 * contiene el camino con coste posible 'cs'.
 */
struct node{
    int cs;
    vector<int> path;

    node(){};

    node(int cs_, vector<int> path_){
        cs=cs_;
        path=path_;
     }
};

/**
 * @brief Calcula el coste de 'v' segun 'w'
 * @param w matriz de pesos a partir de la cual se calculara el coste
 * @oaram v vector de enteros que contiene la solucion de la que se quiere saber el coste
 * 
 * [Aclaracion] No cerramos el camino porque las soluciones finales (las de tamanio n) ya 
 * tienen el camino cerrado.
 */
int solution_cost(struct weight & w, vector<int> & v){
    int cost=0;
    for( int i=0; i<v.size()-1; i++ )
        cost+=w.get( v[i], v[i+1] );
    return cost;
}

/**
 * @brief Devuelve el valor del minimo elemento de la fila 'index' en la matriz de pesos
 * 'w' evitando las columnas en 'to_avoid'
 * @param index Indice de la fila donde se va a calcular el minimo
 * @param to_avoid vector<int> que contiene los indices de las columnas a evitar en el calculo
 * @param w Matriz de pesos a partir de la cual se calculara el coste
 */
int min_value_row(int index, vector<int> to_avoid, struct weight & w) {
    int min=INT_MAX;
    for( int j=0; j<w.size(); j++ ){
        if( index!=j && !is_in(to_avoid, j)){ //is_in definida en "auxiliar.cpp"
            if( w.get(index, j) < min )
                min=w.get(index, j);
        }
    }
    return min;
}


/**
 * @brief Considerando que v tenga k elementos => v = { v[0], ... , v[k-1] },
 * calculamos el minimo de las filas cuyos indices no pertenecen a v, es decir,
 * del complementario de v, mas el ultimo elemento de v.  Evitando todas las columnas
 * cuyo indice este en { v[1], ... , v[k-1] }
 * @param w Matriz de pesos
 * @param v solucion a la que se le quiere calcular el coste posible 
 */
int possible_cost(struct weight & w, vector<int> & v){

    int cost=solution_cost(w, v);

    vector<int> rows_to_calculate_min = supplementary(v, w.size());
    rows_to_calculate_min.push_back(v.back());

    vector<int> rows_to_avoid = v;
    rows_to_avoid.erase( rows_to_avoid.begin() );

    for( int i=0; i<rows_to_calculate_min.size(); i++ )
        cost+=min_value_row(rows_to_calculate_min[i], rows_to_avoid, w);

    return cost;
}

/**
 * @brief Guarda en 'children' todos los vectores "hijos" correspondientes a 'v',
 * es decir, cada vector de 'children' estara formado por una copia de v, mas un elemento
 * de su complementario.
 * @param children Vector de vectores donde se guardaran los hijos.
 * @param v Vector a partil del cual calcular los hijos
 * @param n Tamanio, necesario para poder calcular el complementario sobre n
 */
void generate_children(vector<vector<int> > & children, vector<int> & v, int n){
    children.clear();
    vector<int> rest=supplementary(v, n);
    children.resize(rest.size());
    for(int i=0; i<children.size(); i++){
        children[i]=v;
        children[i].push_back(rest[i]);
    }
}

/**
 * @brief Redefinicion del operador () para ordenar los nodos de la cola de menor a mayor valor de
 * la cota superior
 */
class comparison{ 
    public:
        bool operator() ( const struct node & a, const struct node & b ) { return a.cs < b.cs; } 
};

/**
 * @brief Algoritmo de ramificacion y poda.
 * @param root Indice de la ciudad en la que empezar (da igual cual se ponga, la mejor solucion sera equivalente)
 * @param w Matriz de pesos
 */
vector<int> branch_and_bound(int root, struct weight & w){
    
    //Lista de nodos donde guardaremos los nodos pendientes de desarrollar/podar
    priority_queue<node, vector<node>, comparison> lnv;
    //Aqui iremos guardando la mejor solucion que vayamos encontrando
    vector<int> solution; 
    //Nodo sobre el que estaremos trabajando
    struct node current_option;
    //Vector de vectores donde almacenaremos los hijos correspondientes a 'current_option'
    vector<vector<int> > children;
    
    //Inicializamos 'current_option' con 'root' y lo aniadimos a 'lnv' para poder empezar con
    //el algoritmo
    current_option.cs=0;
    current_option.path.push_back(root);
    lnv.push( current_option );

    //Coste de la mejor solucion hasta el momento, lo inicializamos a infinito para poder
    //calcular el minimo
    int min_cost = INT_MAX; 

    int aux;
    while( !lnv.empty() ){
        current_option=lnv.top();

        if( max_queue_size < lnv.size() ) max_queue_size=lnv.size(); 
        
        lnv.pop();

        //CI:=coste hasta el momento de x
        if( solution_cost(w, current_option.path) < min_cost ){

            generate_children(children, current_option.path, w.size());
            
            for(int i=0; i<children.size(); i++){
                nodes_calculated++;
                if( children[i].size() != w.size()){
                    //Esto siempre se cumplira hasta que se encuentre la primera solucion, es decir, desarrollamos
                    //los nodos con menor coste hasta encontrar una solucion, para asegurarnos que el menos encontramos una.
                    //Despues de eso, aplicamos ramificacion/poda
                    if(solution_cost(w, children[i]) < min_cost) 
                        lnv.push( node(possible_cost(w, children[i]), children[i]) );
                    else    //Si el coste de ese hijo es mayor que el minimo, entonces podamos
                        bounds++;
                }   
                else{
                    //Si hemos encontrado una solucion mejor, la guardamos y actualizamos 'min_cost'
                    if( solution_cost(w, children[i]) < min_cost ){
                        solution=children[i];
                        aux=solution_cost(w, children[i]);
                        min_cost = ( min_cost < aux ) ? min_cost:aux;
                    }
                }
            }
        } else bounds++;
    }
    return solution;

}

long long int factorial(int n)
{
  return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}

int main( int argc, char ** argv ) {
    
    if( argc != 2 ) {
        cout << "Format: " << argv[0] << " <n>." << endl;
        exit(1);
    }

    int n=atoi(argv[1]);
    struct weight w(n);

    clock_t tbefore;
    clock_t tafter;

    tbefore=clock();
    branch_and_bound(0, w);
    tafter=clock();

    printf("n=%d => n!: %lld \t\t Nodos: %lld \t\t Podas: %lld \t\t Max. Size %lld \t\t Tiempo (s): %5.9f\n",
            n, factorial(n), nodes_calculated, bounds, max_queue_size, ((double)(tafter-tbefore))/CLOCKS_PER_SEC);

}