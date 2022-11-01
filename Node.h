#include<vector>
using namespace std;

#ifndef NODE_H__
#define NODE_H__

class Node {
public:

	vector<vector<int>> s;			//vector containing puzzle
	Node* parent;
	int blank_x;
	int blank_y;
	int depth = 0;
	int	cost;
	Node(const vector<vector<int>>& v, Node* prev_Node, int _depth);
	void PrintNode(const vector<vector<int>>& v);

	friend struct sortQueue;
};

#endif