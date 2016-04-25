//#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <set>
#include <map>
#include <bitset>
#include <vector>
#include <iostream>
#include <sstream>
using namespace std;
#define MAXD 32
#define MAXN 512
struct Inst;
bitset<MAXN> IN[MAXD], OUT[MAXD];
bitset<MAXN> GEN[MAXD], KILL[MAXD];
map<string, int> BBname, BBvar, BBop;
map<int, string> invBBname, invBBvar, invBBop;
map<int, vector<int>> BBgraph, invBBgraph;
map<int, vector<Inst>> BBinst;
vector<Inst> instList;
struct Inst {
	int expId, varId;
	int opcode;
	vector<int> opargs;
	void print() {
		printf("\t\t[%d] %s = %s", expId, invBBvar[varId].c_str(), invBBop[opcode].c_str());
		for (auto e : opargs)
			printf(" %s", invBBvar[e].c_str());
		puts("");
	}
};
static void listBB() {
	puts("=== Basic Block Information ===");
	for (auto &e : BBinst) {
		int bid = e.first;
		printf("\t--- %s ---\n", invBBname[bid].c_str());
		vector<Inst> &instSet = e.second;
		for (auto &I : instSet)
			I.print();
	}
	puts("===\n");
}
static void printBBinfo(int bid) {
	printf("\t--- %s ---\n", invBBname[bid].c_str());
	printf("\t\tGEN :\n");
	for (int i = 0; i < instList.size(); i++) {
		if (GEN[bid][i])
			putchar('\t'), instList[i].print();
	}
	printf("\t\tKILL:\n");
	for (int i = 0; i < instList.size(); i++) {
		if (KILL[bid][i])
			putchar('\t'), instList[i].print();
	}
	printf("\t\tIN  :\n");
	for (int i = 0; i < instList.size(); i++) {
		if (IN[bid][i])
			putchar('\t'), instList[i].print();
	}
	printf("\t\tOUT :\n");
	for (int i = 0; i < instList.size(); i++) {
		if (OUT[bid][i])
			putchar('\t'), instList[i].print();
	}
}
static string trim(string s) {  
    if (s.empty())
        return s;  
    s.erase(0, s.find_first_not_of(" "));  
    s.erase(s.find_last_not_of(" ") + 1);
    return s;  
}
static int BBlookup(string s) {
	int sz = BBname.size();
	if (BBname.count(s))
		return BBname[s];
	invBBname[sz] = s;
	return BBname[s] = sz;
}
static int Oplookup(string s) {
	int sz = BBop.size();
	if (BBop.count(s))
		return BBop[s];
	invBBop[sz] = s;
	return BBop[s] = sz;
}
static int Varlookup(string s) {
	int sz = BBvar.size();
	if (BBvar.count(s))
		return BBvar[s];
	invBBvar[sz] = s;
	return BBvar[s] = sz;
}
static void clearGraph() {
	BBname.clear();
	BBgraph.clear();
}
static void clearInst() {
	BBop.clear();
	BBvar.clear();
	BBinst.clear();
}
static void initTable() {
	for (int i = 0; i < instList.size(); i++) {
		IN[i] = bitset<MAXN>();
		OUT[i] = bitset<MAXN>();
		GEN[i] = bitset<MAXN>();
		KILL[i] = bitset<MAXN>();
	}
}
static void readGraph() {
	clearGraph();
	string line, trash;
	int stId = BBlookup("Entry");
	int edId = BBlookup("Exit");
	BBgraph[stId] = invBBgraph[stId] = vector<int>();
	BBgraph[edId] = invBBgraph[edId] = vector<int>();
	while (getline(cin, line)) {
		line = trim(line);
		if (line == "```")
			return ;
		stringstream sin(line);
		string u, v;
		sin >> u >> trash >> v;
		int uid, vid;
		uid = BBlookup(u), vid = BBlookup(v);
		BBgraph[uid].push_back(vid);
		invBBgraph[vid].push_back(uid);
	}
}
static void readInst() {
	clearInst();
	string line, trash;
	getline(cin, line);
	line = trim(line);
	if (line == "```")
		return ;
	int id = 0;
	do {
		string bname = line;
		int bid = BBlookup(bname);
		while (getline(cin, line)) {
			line = trim(line);
			if (line[0] == 'B' || line[0] == '`')	break;
			int did, vid, oid;
			string var, op;
			stringstream tin(line);
			tin >> op >> var;
			Inst inst;
			inst.expId = id++;
			inst.opcode = Oplookup(op);
			inst.varId = Varlookup(var);
			while (tin >> var)
				inst.opargs.push_back(Varlookup(var));
			instList.push_back(inst);
			BBinst[bid].push_back(inst);
		}
		if (line == "```")
			break;
	} while (true);
	int stId = BBlookup("Entry");
	int edId = BBlookup("Exit");
	BBinst[stId], BBinst[edId];
}
static void readIn() {
	string line;
	while (getline(cin, line)) {
		line = trim(line);
		if (line == "```") {
			readGraph();
			break;
		}
	}
	while (getline(cin, line)) {
		line = trim(line);
		if (line == "```") {
			readInst();
			break;
		}
	}
}
static bool assignAndDiff(bitset<MAXN> &A, bitset<MAXN> B) {
	bool test = (A != B);
	A = B;
	return test;
}
static void execute() {
	initTable();
	for (auto &BBu : BBinst) {
		vector<Inst> &instSet = BBu.second;
		for (auto &Iu : instSet) {
			GEN[BBu.first].set(Iu.expId);
			for (auto &BBv : BBinst) {
				for (auto Iv : BBv.second) {
					bool same = false;
					if (BBv.first == BBu.first && (Iu.expId >= Iv.expId))
						continue;
					for (auto t : Iv.opargs) {
						if (t == Iu.varId)
							same = true;
					}
					if (same)
						KILL[BBu.first].set(Iv.expId);
				}
			}
		}
		GEN[BBu.first] = GEN[BBu.first]^(GEN[BBu.first]&KILL[BBu.first]);
	}
	
	bool changed = false;
	do {
		changed = false;
		for (auto &e : BBinst) {
			int bid = e.first;
			changed |= assignAndDiff(OUT[bid], GEN[bid] | (IN[bid]^(IN[bid]&KILL[bid])));
			bitset<MAXN> intersect;
			intersect.flip();
			for (auto &pred : invBBgraph[bid])
				intersect = intersect & OUT[pred];
			if (invBBgraph[bid].size() == 0)
				intersect = bitset<MAXN>();
			changed |= assignAndDiff(IN[bid], intersect);
		}
	} while (changed == true);
}
static void writeOut() {
	listBB();
	puts("=== Basic Block Available Expressions ===");
	for (auto &e : BBinst) {
		int bid = e.first;
		printBBinfo(bid);
	}
	puts("===\n");
}
int main() {
	readIn();
	execute();
	writeOut();
	return 0;
}
/*
```
Entry -> B1
B1 -> B2
B2 -> B3
B3 -> B4
B4 -> Exit
```
```
B1
+ a b c
B2
- b a d
B3
- c b c
B4
- d a d
```

*/
