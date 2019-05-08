#ifndef DATATYPE_H
#define DATATYPE_H

#include <vector>

using namespace std;

namespace SAT_SOLVER
{
  vector<vector<int> > V_Literals;
  //vector<vector<int> > V_Lookup;

  typedef struct Clause{
    int count_done;
    vector<int> Literal_unknown;
  }Clause;

  vector<Clause>V_Lookup;

  vector<Clause> V_Clause;
  int nVariables;
  vector<int>V_occur; 
  vector<int>V_count;
}

#endif
