//References: https://www.youtube.com/watch?v=-xyju56LLMI  teaches how to use priority queue to sort your queue
//https://www.geeksforgeeks.org/2d-vector-in-cpp-with-user-defined-size/ teaches how to use functionality of 2d vectors
//https://stackoverflow.com/questions/21243671/should-all-comparison-operators-be-constexpr-for-a-potentially-constexpr-object had error because I didn't use constexpr when overloading operator function
																																
#include <iostream>
#include "Node.h"
#include <vector>
#include <queue>
using namespace std;

int expanded = 0;															//Stores nodes expanded
int maxQueue = 0;															//Gets Max Size
int SumManhattan_Dist(vector<vector<int>> puzz);							//Calculates total manhattan distance of puzzle		
int Manhattan_dist(vector<vector<int>> puzz, int x, int y);					//Calcutes manhattan distance of a single index in the puzzle				
int MisplacedTiles(vector<vector<int>> puzz);								//Calculates amount of tiles misplaced in puzzle
void swap(Node* n, int nX, int oX, int nY, int oY);							//Swaps blank spot with valid swappable index/spot in puzzle
Node* MoveUp(Node* temp);												
Node* MoveDown(Node* temp);																								
Node* MoveLeft(Node* temp);													
Node* MoveRight(Node* temp);											
Node* A_Manhattan(vector<vector<int>> _puzzle);								//Solves puzzle using Manhattan heuristic
Node* A_Misplaced(vector<vector<int>> _puzzle);								//Solves puzzle using Misplaced Tiles heuristic
Node* A_UniformCost(vector<vector<int>> _puzzle);							//Solves puzzle using Uniform Cost heuristic

vector<vector<int>> goalState{
	{1, 2, 3},
	{4, 5, 6},
	{7, 8, 0}
};

struct sortQueue {																	//Sorts Queue by orderning nodes from least to greatest in cost
public:
	constexpr bool operator()(const Node* a, const Node* b) const noexcept {
		return a->depth + a->cost > b->depth + b->cost;								//f(n) = g[n] + h[n], g[n] is the distance away from the goal, h[n] is the cost to get to the goal
	}

};

//----------------------------------------   Helper Functions for Solve    ---------------------------------//

void PrintSolution(Node* curr) {

	vector<Node*> solutionPath;
	Node* temp;

	while (curr->parent != nullptr) {
		curr = curr->parent;
		solutionPath.push_back(curr);
	}

	reverse(solutionPath.begin(), solutionPath.end());

	for (int i = 0; i < solutionPath.size(); ++i) {
		temp = solutionPath[i];
		cout << "h[n] = " << temp->cost << ", " << "g[n] = " << temp->depth - 1<< endl;
		temp->PrintNode(temp->s);
	}
}


int Manhattan_dist(vector<vector<int>> puzz, int x, int y) {	//Calcutes manhattan distance of a single index in the puzzle

	bool found = false;
	int _x;
	int _y;
	int distance = 0;


	for (unsigned int i = 0; i < puzz.size(); ++i) {
		for (unsigned int j = 0; j < puzz[i].size(); ++j) {
			if (puzz[x][y] == goalState[i][j]) {
				found = true;
				_x = i;
				_y = j;
			}
			else {
				found = false;
			}
			if (found == true) {
				return distance = abs(x - _x) + abs(y - _y);

			}
		}
	}

	return distance;

}

int SumManhattan_Dist(vector<vector<int>> puzz) {				//Calculates total manhattan distance of puzzle

	int sum = 0;

	for (unsigned int i = 0; i < puzz.size(); ++i) {
		for (unsigned int j = 0; j < puzz[i].size(); ++j) {
			if (puzz[i][j] != 0) {
				sum += Manhattan_dist(puzz, i, j);
			}
		}
	}

	return sum;
}

int MisplacedTiles(vector<vector<int>> puzz) {					//Calculates amount of tiles misplaced in puzzle

	int count = 0;

	for (unsigned int i = 0; i < puzz.size(); ++i) {
		for (unsigned int j = 0; j < puzz[i].size(); ++j) {
			if (puzz[i][j] != goalState[i][j]) {
				++count;
			}
		}
	}

	return count;
	
}

void swap(Node* n, int nX, int oX, int nY, int oY) {			//Swaps blank spot with valid swappable index/spot in puzzle

	int temp = n->s[nX][nY];									//stores coords of non blank number so that we can swap it with the blank
	n->s[nX][nY] = n->s[oX][oY];								//Swaps blank spot
	n->s[oX][oY] = temp;										//Puts non blank number back into the old blank spot index

	n->blank_x = nX;
	n->blank_y = nY;

}

Node* MoveUp(Node* temp) {

	swap(temp, temp->blank_x - 1, temp->blank_x , temp->blank_y , temp->blank_y - 0);

	return temp;
}

Node* MoveDown(Node* temp) {

	swap(temp, temp->blank_x + 1, temp->blank_x, temp->blank_y, temp->blank_y - 0);

	return temp;
}

Node* MoveLeft(Node* temp) {

	swap(temp, temp->blank_x, temp->blank_x, temp->blank_y - 1, temp->blank_y - 0);

	return temp;
}

Node* MoveRight(Node* temp) {

	swap(temp, temp->blank_x, temp->blank_x, temp->blank_y + 1, temp->blank_y - 0);

	return temp;
}



//-------------------------------------------------      END      ---------------------------------------------------------------//



//----------------------------------------  A* solve using 3 different heuristics  ---------------------------------------------//

Node* A_Manhattan(vector<vector<int>> _puzzle) {											//General Search algorithm
	
	priority_queue<Node*, vector<Node*> , sortQueue> que;

	Node* root = new Node(_puzzle, nullptr, 0);

	root->cost = SumManhattan_Dist(root->s);

	que.push(root);
	
	Node* curr = nullptr;

	while (!que.empty()) {																	//if empty return 
		curr = que.top();																	//Make curr equal to the puzzle/node that has lowest manhattan distance

		if (que.size() >> maxQueue) {
			maxQueue = que.size();
		}

		que.pop();																			

		if (curr->cost == 0) {																//Check if we are already at the goal state
			PrintSolution(curr);
			cout << "h[n] = " << curr->cost << ", " << "g[n] = " << curr->depth - 1 << endl;
			return curr;
		}

		expanded += 1;

		if (curr->blank_x != 0) {															//Move Up
			Node* OperatorUp = new Node(curr->s, curr, curr->depth);						//Makes new operator node
			MoveUp(OperatorUp);																
			OperatorUp->cost = SumManhattan_Dist(OperatorUp->s);							//Calculate Manhattan cost
			que.push(OperatorUp);															//Caculate Manhattan cost then push node
		}
		if (curr->blank_x != 2) {															//Move Down
			Node* OperatorDown = new Node(curr->s, curr, curr->depth);						//Makes new operator node
			MoveDown(OperatorDown);
			OperatorDown->cost = SumManhattan_Dist(OperatorDown->s);
			que.push(OperatorDown);													
		}
		if (curr->blank_y != 0) {															//Move Left
			Node* OperatorLeft = new Node(curr->s, curr, curr->depth);						//Makes new operator node
			MoveLeft(OperatorLeft);
			OperatorLeft->cost = SumManhattan_Dist(OperatorLeft->s);	
			que.push(OperatorLeft);
		}
		if (curr->blank_y != 2) {															//Move right
			Node* OperatorRight = new Node(curr->s, curr, curr->depth);						//Makes new operator node
			MoveRight(OperatorRight);
			OperatorRight->cost = SumManhattan_Dist(OperatorRight->s);
			que.push(OperatorRight);
		}
	}
	return nullptr;
}

Node* A_Misplaced(vector<vector<int>> _puzzle) {											//General Search algorithm

	priority_queue<Node*, vector<Node*>, sortQueue> que;

	Node* root = new Node(_puzzle, nullptr, 0);

	root->cost = MisplacedTiles(root->s);

	que.push(root);

	Node* curr = nullptr;

	while (!que.empty()) {
		curr = que.top();

		if (que.size() >> maxQueue) {
			maxQueue = que.size();
		}

		que.pop();

		expanded += 1;


		if (curr->cost == 0) {
			PrintSolution(curr);
			cout << "h[n] = " << curr->cost << ", " << "g[n] = " << curr->depth - 1 << endl;
			return curr;
		}

		if (curr->blank_x != 0) {															//Move Up
			Node* OperatorUp = new Node(curr->s, curr, curr->depth);						//Makes new operator node
			MoveUp(OperatorUp);
			OperatorUp->cost = MisplacedTiles(OperatorUp->s);
			que.push(OperatorUp);
		}
		if (curr->blank_x != 2) {															//Move Down
			Node* OperatorDown = new Node(curr->s, curr, curr->depth);						//Makes new operator node
			MoveDown(OperatorDown);
			OperatorDown->cost = MisplacedTiles(OperatorDown->s);
			que.push(OperatorDown);
		}
		if (curr->blank_y != 0) {															//Move Left
			Node* OperatorLeft = new Node(curr->s, curr, curr->depth);						//Makes new operator node
			MoveLeft(OperatorLeft);
			OperatorLeft->cost = MisplacedTiles(OperatorLeft->s);
			que.push(OperatorLeft);
		}
		if (curr->blank_y != 2) {															//Move right
			Node* OperatorRight = new Node(curr->s, curr, curr->depth);						//Makes new operator node
			MoveRight(OperatorRight);
			OperatorRight->cost = MisplacedTiles(OperatorRight->s);
			que.push(OperatorRight);
		}
	}
	return nullptr;
}

Node* A_UniformCost(vector<vector<int>> _puzzle) {											//General Search Algorithm

	priority_queue<Node*, vector<Node*>, sortQueue> que;

	Node* root = new Node(_puzzle, nullptr, 0);

	root->cost = 1;

	que.push(root);

	Node* curr = nullptr;

	while (!que.empty()) {
		curr = que.top();

		if (que.size() >> maxQueue) {
			maxQueue = que.size();
		}

		que.pop();

		expanded += 1;

		if (curr->s == goalState) {
			PrintSolution(curr);
			curr->cost = 0;
			cout << "h[n] = " << curr->cost << ", " << "g[n] = " << curr->depth - 1 << endl;
			return curr;
		}

		if (curr->blank_x != 0) {															//Move Up
			Node* OperatorUp = new Node(curr->s, curr, curr->depth);						//Makes new operator node
			MoveUp(OperatorUp);
			OperatorUp->cost = 0;
			que.push(OperatorUp);
		}
		if (curr->blank_x != 2) {															//Move Down
			Node* OperatorDown = new Node(curr->s, curr, curr->depth);						//Makes new operator node
			MoveDown(OperatorDown);
			OperatorDown->cost = 0;
			que.push(OperatorDown);
		}
		if (curr->blank_y != 0) {															//Move Left
			Node* OperatorLeft = new Node(curr->s, curr, curr->depth);						//Makes new operator node
			MoveLeft(OperatorLeft);
			OperatorLeft->cost = 0;
			que.push(OperatorLeft);
		}
		if (curr->blank_y != 2) {															//Move right
			Node* OperatorRight = new Node(curr->s, curr, curr->depth);						//Makes new operator node
			MoveRight(OperatorRight);
			OperatorRight->cost = 0;
			que.push(OperatorRight);
		}
	}
	return nullptr;
}

//-------------------------------------------------      END      ---------------------------------------------------------------//

int main() {

	int userInput = 0;
	int count = 0;

	int blank_x = 0;
	int blank_y = 0;

	vector<vector<int>> puzzle{
		{0, 0, 0},
		{0, 0, 0},
		{0, 0, 0}
	};

	cout << "Enter your custom 8 puzzle: " << endl << endl;

	cout << "Enter each spot of the 8-puzzle by typing a number (1 - 8) or typing 0 for the blank. Continuously press enter while entering a number for each part of the puzzle" << endl;

	cout << endl << "Enter first row of puzzle:" << endl;

	for (int i = 0; i < puzzle.size(); i++) {				//Takes in userInput to populate 2d vector to fill puzzle
		for (int j = 0; j < puzzle[i].size(); j++) {
			cin >> userInput;
			puzzle[i][j] = userInput;
			++count;
			if (count == 3) {
				cout << endl << "Enter second row of puzzle" << endl;
			}
			if (count == 6) {
				cout << endl << "Enter third row of puzzle" << endl;
			}
		}
	}

	cout << endl;

	for (int i = 0; i < puzzle.size(); i++) {				//print custom puzzle
		for (int j = 0; j < puzzle[i].size(); j++) {
			cout << puzzle[i][j] << " ";
		}
		cout << endl;
	}
	
	cout << endl;

	Node* root = new Node(puzzle, nullptr, 0);				//Create the root node, so we will always have the original puzzle

	cout << "Now enter what heuristic you want to use to solve the puzzle: (1) for Manhattan, (2) for Misplaced Tiles, (3) Uniform Cost: ";

	cin >> userInput;
	cout << endl;

	if (userInput == 1) {
		Node* solved = new Node(root->s, nullptr, root->depth);
		solved = A_Manhattan(puzzle);									//A* solve (Manhattan)
		solved->PrintNode(solved->s);
		cout << "Nodes Expanded: " << expanded << endl;
		cout << "Solution Depth: " << solved->depth - 1 << endl;
		cout << "Max Queue Size: " << maxQueue << endl;
	}
	else if (userInput == 2) {
		Node* solved = new Node(root->s, nullptr, root->depth);
		solved = A_Misplaced(puzzle);									//A* solve	(Misplaced Tiles)
		solved->PrintNode(solved->s);
		cout << "Nodes Expanded: " << expanded << endl;
		cout << "Solution Depth: " << solved->depth - 1 << endl;
		cout << "Max Queue Size: " << maxQueue << endl;
	}
	else if (userInput == 3) {
		Node* solved = new Node(root->s, nullptr, root->depth);
		solved = A_UniformCost(puzzle);									//A* solve	(Uniform cost)
		solved->PrintNode(solved->s);
		cout << "Nodes Expanded: " << expanded << endl;
		cout << "Solution Depth: " << solved->depth - 1 << endl;
		cout << "Max Queue Size: " << maxQueue << endl;
	}


	return 0;
}