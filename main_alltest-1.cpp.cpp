#include <cstdlib>
#include <ctime>
#include "parser_dimacs.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include "datatype.h"

using namespace std;
using namespace SAT_SOLVER;

int nConflicts = 0;
vector<int> pLiterals; //pure literals
vector<int> foLiterals;//forced literals
vector<int> frLiterals;//free literals
vector<int> dLiterals; //decision literals
vector<int> sat_nClause; // satisfied clause numbers
vector<int> frLiterals_Pos; // free literals position
vector<int>nUniClause; // the number of clause that is uni clause
vector<int>nUniClause_Lit; // the literal which is in the uni clause;
vector<int>chrono_test;//for non nonchrono_back
/*populate the occurence array*/
void pop_V_occur()
{
  for(int i = 0; i<V_Literals.size(); i++)
  V_occur.push_back(i+1);
}
/*sort the occurence array*/
void sorted_literals()
{
  int temp;
  for(int i = 0; i<V_occur.size()-1;i++)
  {
    for(int j=0; j<V_occur.size()-1-i;j++)
    if(V_Literals.at(j).size()<V_Literals.at(j+1).size())
     {
       temp = V_occur.at(j);
       V_occur.at(j)=  V_occur.at(j+1);
       V_occur.at(j+1) = temp;
     }
  }
}
/*check sign of the literal in the clause*/
int check_sign(int cnf_INT)
{
  //cout<<"Enter check_sign"<<endl;
	if(cnf_INT < 0) {
		return -1;
	} else {
		return 1;
	}
}
/*search for pure literals*/
int search_pLiterals()
{
	int neg, i, j;
	bool flag;
  //cout<<"Enter search_pliteral"<<endl;

	for(i = 0; i < V_Literals.size(); i++) {
		neg = check_sign(V_Literals.at(i).front());
		flag = true;
		for(j = 1; j < V_Literals.at(i).size(); j++) {
			if(check_sign(V_Literals.at(i).at(j)) != neg) {
				flag = false;
				break;
			}
		}
		if(flag) {
			pLiterals.push_back(neg * (i + 1));
		}
	}

	return 0;
}
/*search for forced literals*/
int search_foLiterals()
{
	int cLiteral, i;
  //cout<<"Enter search_foliteral"<<endl;

	for(i = 0; i < V_Clause.size(); i++) {
		if(V_Clause.at(i).count_done == 0 && V_Clause.at(i).Literal_unknown.size() == 1) {
			cLiteral = V_Clause.at(i).Literal_unknown.front();
			if(find(foLiterals.begin(), foLiterals.end(), cLiteral) == foLiterals.end()) {
				foLiterals.push_back(cLiteral);
			}
		}
	}

	return 0;
}
/*search for free literals*/
int search_frLiterals()
{
	int i;
  //cout<<"Enter searchfrlieral"<<endl;
	for(i = 1; i <= V_occur.size(); i++) {
		if(find(dLiterals.begin(), dLiterals.end(), i) == dLiterals.end() && find(dLiterals.begin(), dLiterals.end(), -i) == dLiterals.end()) {
			frLiterals.push_back(V_occur.at(i-1));//changed
    //  cout<<"value of i= "<<i<<endl;
		}
	}

	return 0;
}
/*set the literal*/
int set_Literal(int cLiteral)
{
	int neg, c_nClause, i;
  //cout<<"Enter set literal"<<endl;

	neg = check_sign(cLiteral);

	for(i = 0; i < V_Literals.at(neg * cLiteral - 1).size(); i++) {
		c_nClause = V_Literals.at(neg * cLiteral - 1).at(i);//pointing error for sorting
		if(check_sign(c_nClause) == neg) {
			V_Clause.at(neg * c_nClause - 1).count_done += 1;
			V_Clause.at(neg * c_nClause - 1).Literal_unknown.erase(
			remove(V_Clause.at(neg * c_nClause - 1).Literal_unknown.begin(),
			V_Clause.at(neg * c_nClause - 1).Literal_unknown.end(), cLiteral),
			V_Clause.at(neg * c_nClause - 1).Literal_unknown.end());

			if(find(sat_nClause.begin(), sat_nClause.end(), neg * c_nClause) == sat_nClause.end()) {
				sat_nClause.push_back(neg * c_nClause);
			}
		} else {
			V_Clause.at(-1 * neg * c_nClause - 1).Literal_unknown.erase(
			remove(V_Clause.at(-1 * neg * c_nClause - 1).Literal_unknown.begin(),
			V_Clause.at(-1 * neg * c_nClause - 1).Literal_unknown.end(), -cLiteral),
			V_Clause.at(-1 * neg * c_nClause - 1).Literal_unknown.end());
		}
	}

	return 0;
}
/*check for conflict*/
bool find_conflict()
{
	int i;
  //cout<<"Enter find conflict"<<endl;

	for(i = 0; i < foLiterals.size(); i++) {
		if(find(foLiterals.begin(), foLiterals.end(), -foLiterals.at(i)) != foLiterals.end()) {
			return true;
		}

	}

	return false;
}
/*satisfiability check*/
void satisfiablity()
{int i;
  //cout<<"Enter satisfiability"<<endl;
	if(sat_nClause.size() == V_Clause.size())
	{
		cout<<"RESULT: SAT" << endl;
        cout<<"ASSIGNMENT:"<<endl;
	for(i = 0; i < dLiterals.size(); i++) {
	if(dLiterals.at(i)<0)
        {//string var = "x";
         //var = var+ to_string(-1*dLiterals.at(i));		
	cout << " " <<(-1*dLiterals.at(i))<< "="<<0;}
        else{
         //string var = "x";
         //var = var+ to_string(dLiterals.at(i));
        cout << " " <<dLiterals.at(i)<< "="<<1;}
        }
	}
	else
		cerr<<"RESULT: UNSAT "<< endl;
	
}
/*simple chronological backtrack*/
int backtrack()
{
  //cout<<"Enter backtrack"<<endl;
	int cLiteral, c_nClause, neg, i;

	cLiteral = dLiterals.back();
	neg = check_sign(cLiteral);

	for(i = 0; i < V_Literals.at(neg * cLiteral - 1).size(); i++) {
		c_nClause = V_Literals.at(neg * cLiteral - 1).at(i);
		if(check_sign(c_nClause) == neg) {
			V_Clause.at(neg * c_nClause - 1).count_done -= 1;
			V_Clause.at(neg * c_nClause - 1).Literal_unknown.push_back(cLiteral);

			if(V_Clause.at(neg * c_nClause - 1).count_done == 0) {
				sat_nClause.erase(remove(sat_nClause.begin(), sat_nClause.end(), neg * c_nClause), sat_nClause.end());
			}
		} else {
			V_Clause.at(-1 * neg * c_nClause - 1).Literal_unknown.push_back(-cLiteral);
		}
	}

	if(dLiterals.size() - 1 != frLiterals_Pos.back()) {
		frLiterals.push_back(neg * cLiteral);
		dLiterals.pop_back();
		backtrack();
		return 0;
	} else {
		if(check_sign(dLiterals.back()) == 1) {
			dLiterals.back() = -dLiterals.back();
			return 0;
		} else {
			frLiterals.push_back(neg * cLiteral);
			dLiterals.pop_back();
			frLiterals_Pos.pop_back();

			if(frLiterals_Pos.empty()) {
				return 0;
			}

			backtrack();
			return 0;
		}
	}
}
/*tried this function for non chronological backtrack*/
void make_chrono()
{
	//cout<<"inside make_chrono function"<<endl;
  int j,temp,i,c_nClause;
  chrono_test.push_back(dLiterals.back());
  for(j=dLiterals.size()-2;j>=0;j--)
  {
    temp=dLiterals.at(j);
    int sign = check_sign(temp);
    for(i = 0; i < V_Literals.at(sign*temp - 1).size(); i++)
    {
  		c_nClause = V_Literals.at(sign*temp - 1).at(i);
  		int clause_sign = check_sign(c_nClause);
      if(find(V_Lookup.at(clause_sign*c_nClause-1).Literal_unknown.begin(),V_Lookup.at(clause_sign*c_nClause-1).Literal_unknown.end(),dLiterals.back())!=V_Lookup.at(clause_sign*c_nClause-1).Literal_unknown.end())
      {
        chrono_test.push_back(temp);
      }
    }

  }
}
/*tried this function for non chronological backtrack*/
int nonchrono_back()
{
	//cout<<"inside nonchrono_back function"<<endl;
  int cLiteral, c_nClause, neg, i,j;
  cLiteral=chrono_test.back();
  //cout<<"check sign first"<<endl;
  neg = check_sign(cLiteral);
  for(i = 0; i < V_Literals.at(neg * cLiteral - 1).size(); i++)
  {
    c_nClause = V_Literals.at(neg * cLiteral - 1).at(i);
    //cout<<"check sign second"<<endl;
    if(check_sign(c_nClause) == neg)
    {
      //cout<<"check sign second with first if"<<endl;
			V_Clause.at(neg * c_nClause - 1).count_done -= 1;
      for(j=0;j<=chrono_test.size()-1;j++)
      {
        //cout<<"check sign second with first if and first for"<<endl;
        cLiteral=chrono_test.at(j);
        //cout<<cLiteral<<endl;
        V_Clause.at(neg * c_nClause - 1).Literal_unknown.push_back(cLiteral);

      }
      //push_back multiple for loops running
      if(V_Clause.at(neg * c_nClause - 1).count_done == 0)
      {
        //cout<<"check sign second with first if and first for then second if"<<endl;
        sat_nClause.erase(remove(sat_nClause.begin(), sat_nClause.end(), neg * c_nClause), sat_nClause.end());

			}
    }
    else
    {
      //cout<<"check sign second with first else"<<endl;
      for(j=0;j<=chrono_test.size()-1;j++)
      {
        cLiteral=chrono_test.at(j);
        V_Clause.at(-1* neg * c_nClause - 1).Literal_unknown.push_back(-cLiteral);
      }
      //V_Clause.at(-1 * neg * c_nClause - 1).Literal_unknown.push_back(-cLiteral);//pudhback
    }
  }
  if(dLiterals.size() - 1 != frLiterals_Pos.back()) {
    for(j=0;j<=chrono_test.size()-1;j++)
    {
    frLiterals.push_back(neg * chrono_test.at(j));
		dLiterals.pop_back();
    }
		nonchrono_back();
		return 0;
	} else {
   // cout<<"check sign third"<<endl;
		if(check_sign(dLiterals.back()) == 1) {
			dLiterals.back() = -dLiterals.back();
			return 0;
		} else {
      for(j=0;j<=chrono_test.size()-1;j++)
      {
        frLiterals.push_back(neg * cLiteral);
			  dLiterals.pop_back();
		  	frLiterals_Pos.pop_back();
      }
      if(frLiterals_Pos.empty())
      {
				return 0;
			}


			nonchrono_back();
			return 0;
		}
	}

}
/*preprocessor for unit clauses*/
bool search_uniClause_Conflict( int key)
{
 // cout<<"Enter search uni clause conflict"<<endl;
	std::vector<int>::iterator it;

  it = find (nUniClause_Lit.begin(), nUniClause_Lit.end(), -key);
  if (it != nUniClause_Lit.end())
    return true;
  else
    return false;
}
/*preprocessor for unit clauses*/
int search_uniClause()
{

 // cout<<"Enter uniclause"<<endl;
int i; bool dec; int flag = 1;

for(int i = 0; i<V_Clause.size(); i++ )
{
  //cout<<"uniclause1"<<endl;
	if(V_Clause.at(i).Literal_unknown.size()==1) //found uniclause
	{

	 nUniClause.push_back(i);
	 dec = search_uniClause_Conflict(V_Clause.at(i).Literal_unknown.front());
	 if(!dec)
	 nUniClause_Lit.push_back(V_Clause.at(i).Literal_unknown.front());
	 else
	 {
	 	flag = 0;
	 	break;
	 }
	}
  else{
   // cout<<"in else"<<endl;
    //cout<<i<<endl;
  }

}
//cout<<"in flag"<<flag<<endl;
return flag;
}


/*core dpll algo*/

int dpll_algo()
{
  int conflicts=0;
  //sorted_literals();
  pop_V_occur();
 sorted_literals();
  //cout<<"Enter DPLL algo"<<endl;
  int check = search_uniClause();
  if(check == 0)
  {
  	satisfiablity();
  	cerr<<"contradcitory uniclause"<<endl;
  	return 0;
  }
  /*else
  {
  	set_Literal(nUniClause_Lit.front());
  }*/


  search_pLiterals();
	while(!pLiterals.empty()) {
		dLiterals.push_back(pLiterals.front());
		pLiterals.erase(pLiterals.begin());
		set_Literal(dLiterals.back());
	}

	search_foLiterals();
  while(!foLiterals.empty()) {
		if(find_conflict()) {
			nConflicts += 1;
			satisfiablity();
			cerr << "RESULT : UNSAT" << endl;
			return 0;
		}
		dLiterals.push_back(foLiterals.front());
		foLiterals.erase(foLiterals.begin());
		set_Literal(dLiterals.back());
		search_foLiterals();
	}
  search_frLiterals();
  while(frLiterals.empty()!=true)
  {
    dLiterals.push_back(frLiterals.front());
    frLiterals.erase(frLiterals.begin());//check with .begin
    frLiterals_Pos.push_back(dLiterals.size()-1);
    set_Literal(dLiterals.back()); // check with .back
    search_foLiterals();
    while(foLiterals.empty()!=true)
    {
      if(find_conflict())//made new method find_conflict()
      {
    //    cout<<"find conflict"<<endl;
        nConflicts=nConflicts+1;
        
				//make_chrono();/* new add*/
        backtrack();//made new method for backtracking cronologically
        //nonchrono_back(); /* new add*/
        if(frLiterals_Pos.empty()==true)
        {
          cerr<<"RESULT: UNSAT"<<endl;
          return 0;
        }
        set_Literal(dLiterals.back());//check with .back
        foLiterals.clear();
        search_foLiterals();
        continue;
      }
      dLiterals.push_back(foLiterals.front());
      frLiterals.erase(remove(frLiterals.begin(), frLiterals.end(), check_sign(foLiterals.front()) * foLiterals.front()), frLiterals.end());
			foLiterals.erase(foLiterals.begin());
			set_Literal(dLiterals.back());
			search_foLiterals();
		}
	}
	// replacing with fully satisfiable function
	satisfiablity();


	return 0;
}

int main(int argc, char *argv[])
{
  string fname;
  clock_t time;


  fname=argv[1];

  time=clock();
  int parsingError=parser(fname);
  time=clock()-time;
  if(parsingError==-1)
  {
    cerr<<"Parsing Error"<<endl;
    return 0;
  }

  //cout<<"Parsing successful"<<endl;
  time=clock();
  dpll_algo();
  time=clock()-time;

  cout<<"Time for computation - "<< (float)(time)/CLOCKS_PER_SEC<<"secs"<<endl;//trying without static

  return 0;
}
