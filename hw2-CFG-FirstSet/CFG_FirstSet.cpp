#include <stdio.h> 
#include <vector>
#include <iostream>
#include <string>
#include <map>
#include <set>
#include <ctype.h>
using namespace std;
class Production {
	public:
		string LHS;
		vector<string> RHS;
		Production(string L = "", vector<string> R = vector<string>()) {
			LHS = L;
			RHS = R;
		}
};
class Grammar {
	public:
		static const string 		lambda;
		vector<Production> 			rules;
		map<string, set<string> > 	first_set;
		map<string, bool> 			derives_lambda;
		
		map<string, bool> mark_lambda();
		void fill_first_set();
		bool isNonterminal(string token);
		set<string> compute_first(vector<string> rhs);
};
const string Grammar::lambda("l");
set<string> Grammar::compute_first(vector<string> rhs) {
	set<string> result;
	size_t i;
	
	if(rhs.size() == 0 || rhs[0] == Grammar::lambda) {
		result.insert(Grammar::lambda);
	} else {
		result = first_set[rhs[0]];
		for(i = 1; i < rhs.size() && 
			first_set[rhs[i-1]].find(Grammar::lambda) != first_set[rhs[i-1]].end(); i++) {
			set<string> f = first_set[rhs[i]];
			f.erase(Grammar::lambda);
			result.insert(f.begin(), f.end());	
		}
		if(i == rhs.size() 
			&& first_set[rhs[i-1]].find(Grammar::lambda) != first_set[rhs[i-1]].end()) {
			result.insert(Grammar::lambda);
		} else {
			result.erase(Grammar::lambda);
		}
	}

	return result;
} 
bool Grammar::isNonterminal(string token) {
	if(token == Grammar::lambda)
		return false;
	for(size_t i = 0; i < token.length(); i++) {
		if(isupper(token[i]))
			return false;
	}
	return true;
}
map<string, bool> Grammar::mark_lambda() {
	bool 				rhs_derives_lambda;
	bool 				changes;
	Production 			p;
	
	derives_lambda.clear();
	
	/* initially, nothing is marked. */
	for(size_t i = 0; i < rules.size(); i++) {
		derives_lambda[rules[i].LHS] = false;
	}
	do {
		changes = false;
		for(size_t i = 0; i < rules.size(); i++) {
			p = rules[i];
			if(!derives_lambda[p.LHS]) {
				if(p.RHS.size() == 0 || p.RHS[0] == Grammar::lambda) {
					changes = derives_lambda[p.LHS] = true;
					continue;
				}
				/* does each part of RHS derive lambda ? */
				rhs_derives_lambda = derives_lambda[string(p.RHS[0])];
				for(size_t j = 1; j < p.RHS.size(); j++) {
					rhs_derives_lambda &= derives_lambda[p.RHS[j]];
				}
				if(rhs_derives_lambda) {
					changes = true;
					derives_lambda[p.LHS] = true;
				}
			}
		}
	} while(changes);
	return derives_lambda;
}
void Grammar::fill_first_set() {
	
	string 		A;
	string 		a;
	Production 	p;
	bool 		changes;
		
	mark_lambda();
	first_set.clear();
	
	for(size_t i = 0; i < rules.size(); i++) {
		A = rules[i].LHS;
		if(derives_lambda[A])
			first_set[A].insert(Grammar::lambda);
	}
	
	for(size_t i = 0; i < rules.size(); i++) {
		for(size_t j = 0; j < rules[i].RHS.size(); j++) {
			a = rules[i].RHS[j];
			if(isNonterminal(a)) {
				first_set[a].insert(a);
				if(j == 0) { // A -> aXX
					first_set[rules[i].LHS].insert(a);
				}
			}
		}
	}
	
	do {
		changes = false;
		for(size_t i = 0; i < rules.size(); i++) {
			p = rules[i];
			set<string> rfirst = compute_first(p.RHS);
			size_t oldsize = first_set[p.LHS].size();
			first_set[p.LHS].insert(rfirst.begin(), rfirst.end());
			size_t newsize = first_set[p.LHS].size();
			if(oldsize != newsize)
				changes = true;
		}
	} while(changes);
} 
void parsingProduction(string r, Grammar &g) {
	string div("->");
	size_t found = r.find(div);
	if(found != std::string::npos) {
		string rhs = r.substr(found + div.length());
		vector<string> tokens;
		for(size_t i = 0; i < rhs.size(); i++)
			tokens.push_back(rhs.substr(i, 1));
		Production p(r.substr(0, found), tokens);
		g.rules.push_back(p);
	}
} 
int main() {
	freopen("input.txt", "r+t", stdin);
	freopen("output.txt", "w+t", stdout); 
	char in[1024];
	while(gets(in)) {
		Grammar g;
		parsingProduction(in, g);
		while(gets(in) && in[0] != '\0') {
			parsingProduction(in, g);
		}
		
		g.fill_first_set();
		
		for(map<string, set<string> >::iterator it = g.first_set.begin();
			it != g.first_set.end(); it++) {
			cout << it->first << ":";
			bool comma = false;
			for(set<string>::iterator jt = it->second.begin();
				jt != it->second.end(); jt++) {
				if(comma == true)	cout << ",";
				comma = true;
				cout << *jt;
			}
			cout << endl;
		}
	}
	return 0;
}
/*
A->BaAb
A->cC
B->d
B->l
C->eA
C->l

A->aAd
A->B
B->bBd
B->C
C->cCd
C->l

E->TX
X->+E
X->#
T->(E)
T->intY
Y->*T
Y->#

E->P(E)
E->vT
P->f
P->l
T->+E
T->l

S->aSe
S->B
B->bBe
B->C
C->cCe
C->d

S->ABc
A->a
A->l
B->b
B->l

*/
