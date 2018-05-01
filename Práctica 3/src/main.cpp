
#include "graph.cpp"


int main(int argc, char **argv){
    if(argc != 2){
        cout << "Número incorrecto de argumentos";
        exit(-1);
    }

    graph m(argv[1]);
    double l= 0;
    int beginning=0;

    //cout << "Introduzca ciudad por la que quiere empezar(0- dim-1): ";
    //cin >> beginning;
    // vector<int> path= m.min_path1(beginning, l);

    // cout << "Recorrido con peso mínimo " << l << ": ";
    // for(unsigned int i= 0; i< path.size(); i++)
    //     cout << path[i] << " ";
    // cout << endl;

    // double l_min=0;
    // vector<int> optimal_path= m.optimal_path(l_min);
    // cout << "\nEl camino más corto con peso " << l_min << " es: ";
    // for(int i=0; i<optimal_path.size(); i++)
    //     cout << optimal_path[i] << " ";
    // cout << endl;

    double ll=0;
    vector<int> p=m.min_path3(0, ll);
    for(int i=0; i<p.size(); i++)
        cout << p[i] << " ";
    cout << endl;
}