#include <iostream>
#include <vector>

#ifndef __AUXILIAR__
#define __AUXILIAR__

using namespace std;

bool is_in(vector<int> & v, int x)
{
    for(int i=0; i<v.size(); i++) if(v[i]==x) return true;
    return false;
}

//supplementary of v respect to [0...n-1]
vector<int> supplementary(vector<int> & v, int n)
{
    vector<int> result;
    for(int i=0; i<n; i++) if( !is_in(v, i) ) result.push_back(i);
    return result;
}

#endif