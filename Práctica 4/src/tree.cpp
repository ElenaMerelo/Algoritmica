
#include<iostream>
#include<list>
#include<vector>

using namespace std;

class tree
{
  private:
    
    struct node
    {

      node(int f, int l)
      {
        father=f;
        label=l;
      }

      int father;
      int label;
      vector<int> children;
    };

    vector<list<node> > t;
    int n; //size

  public:
    
    tree(int n_){ n=n_; }
    
    void generate_level(int level)
    {
      if(level==0)
      {
        for(int i=0; i<n; i++) t[0].push_back(i);
      }
    }
};