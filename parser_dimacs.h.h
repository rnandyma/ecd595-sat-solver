#ifndef PARSER_DIMACS_H
#define PARSER_DIMACS_H

#include <iostream>
#include <fstream>
#include <string>

#include "datatype.h"

using namespace std;
using namespace SAT_SOLVER;

int get_integer(vector<char> V_ch)
{
  int temp=0, end=0, mul=1, minus=1, i;
  if(V_ch.front()=='-')
  {
    minus=-1;
    end=1;
  }
  for(i = V_ch.size()-1;i>=end;i--)
  {
    temp=temp+mul*(V_ch.at(i)-'0');
    mul=mul*10;
  }
  return temp*minus;
}

int add_clause(vector<int> Clause_Literals)
{
  Clause temp={0, Clause_Literals};
  V_Clause.push_back(temp);
  V_Lookup.push_back(temp);
  return 0;
}

int add_clause_num(int Clause_num, int Curr_Literal)
{
  int minus=1;
  if(Curr_Literal<0)
  {
    minus=-1;
  }
  V_Literals.at(minus*Curr_Literal - 1).push_back(minus*Clause_num);

  return 0;
}

int parser(string file_name)
{
  char ch;
  vector<char> V_ch;
  int nClause=1, C_Literals,i;//removed nVariables
  vector<int> Clause_Literals;

  ifstream input_file(file_name.c_str());

  if(input_file.is_open())
  {
    while(!input_file.eof())
    {
      input_file.get(ch);

      if(isdigit(ch) || ch=='-')
      {
        if(ch!='0')
        {
          while(ch !=' ')
          {
            V_ch.push_back(ch);
            input_file.get(ch);
          }
          C_Literals=get_integer(V_ch);
          V_ch.clear();
          add_clause_num(nClause, C_Literals);
          Clause_Literals.push_back(C_Literals);
        }
        else if(ch=='0')
        {
          add_clause(Clause_Literals);
          nClause = nClause + 1;
          Clause_Literals.clear();
          continue;//not sure if needed?
        }
        //cout<<" "<<C_Literals<<" "<<nClause<<endl;
      }
      else if(ch == 'c')
      {
        while(ch!='\n')
        {
          input_file.get(ch);;
        }
      }
      else if(ch == 'p')
      {
        while(!isdigit(ch))
        {
          input_file.get(ch);
        }
        while(ch!=' ')
        {
          V_ch.push_back(ch);
          input_file.get(ch);
        }
        nVariables=get_integer(V_ch);
        V_ch.clear();
        V_Literals.resize(nVariables);

        while(ch!='\n')
        {
          input_file.get(ch);
        }
      }
    }
  }
  else
  {
    cout<<"Cannot Open File."<< endl;
    return -1;
  }

  return 0;
}

#endif
