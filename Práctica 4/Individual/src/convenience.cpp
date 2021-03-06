#include<iostream>
#include<vector>
#include <stdlib.h>

#ifndef __CONVENIENCE__
#define __CONVENIENCE__

using namespace std;

struct conv
{
  int person;
  int convenience;
};

class ConvenienceMatrix
{
  private:
    vector<vector<conv> > c;
  public:
    ConvenienceMatrix(){};
    ConvenienceMatrix(int n)
    {
      c.resize(n);
      for(int i=0; i<n; i++) c[i].resize(n);

      int k=0;
      for(int i=0; i<c.size(); i++)
      {
        for(int j=0; j<c.size(); j++)
          {
            c[i][j].person=j;
            c[i][j].convenience=(i!=j) ? rand() % 100 : 0; //""""pseudo-random"""" 
          }
        k+=1;
      }
    }

    int size(){ return c.size(); }

    struct conv get(int i, int j){ return c[i][j]; }

    //Show convenience's values
    void show()
    {
      for(int i=0; i<c.size(); i++){
        for(int j=0; j<c.size()-1; j++)
          cout << get(i,j).person << ": " << get(i,j).convenience << "\t";
        cout << endl;
      }
    }

    int getConv(int i, int j)
    {
        return c[i][j].convenience;
    }

    //Calculate the cost of a given solution 'v'.
    int costs(vector<int> & v)
    {
      int cost=0, n=v.size();
      for(int i=1; i<n-1; i++) { cost += getConv(v[i],v[i-1]) + getConv(v[i],v[i+1]); }
      cost += getConv(v[0],v[n-1]) + getConv(v[0],v[1]);
      cost += getConv(v[n-1], v[n-2]) + getConv(v[n-1],v[0]);    
      return cost;
    }

};

#endif