#include "Node.h"
#include <vector>
#include <iostream>
using namespace std;

Node::Node(const vector<vector<int>>& v, Node* prev_Node, int _depth) {		//passes in puzzle and parent 

	
	s = {
		{0, 0, 0},
		{0, 0, 0},
		{0, 0, 0}
	};
	
	for (int i = 0; i < v.size(); i++) {				
		for (int j = 0; j < v[i].size(); j++) {
			s[i][j] = v[i][j];
		}
	}

	for (int i = 0; i < s.size(); i++) {				//Finds blank spot
		for (int j = 0; j < s[i].size(); j++) {
			if (s[i][j] == 0) {
				blank_x = i;
				blank_y = j;
				break;
			}
		}
	}

	cost = INT_MAX;
	parent = prev_Node;
	depth = _depth + 1;  

}

void Node::PrintNode(const vector<vector<int>>& v) {

	for (int i = 0; i < v.size(); i++) {				//print custom puzzle
		for (int j = 0; j < v[i].size(); j++) {
			cout << v[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}
