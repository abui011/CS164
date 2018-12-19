#include <iostream>
#include <cstdlib>
#include <queue>
#include <cmath>
#include <algorithm>

using namespace std;

//node that holds each puzzle progression
struct node
{
	int array[3][3];
	int weight;
	int heuristic;
	node* parent;

    node()
    {
    	weight = 0;
    	heuristic = 0;
    }

	node (int p[3][3])
	{
		weight = 0;
		heuristic = 0;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j <3; j++)
			{
				array[i][j] = p[i][j];
			}
		}
	}
	//print function for node
	void print()
	{
		for (int i = 0; i<3; i++) {
			for (int j = 0; j<3; j++) {
				if (j<2) {
					cout << array[i][j] << " ";
				}
				else {
					cout << array[i][j] << endl;
				}
			}
		}
	}
// how you compare and dictate placement in the priority queue
// arranges it based on larger heuristic
	bool operator <(const node& lhs) const
	{
		return (this->weight + this->heuristic) > (lhs.weight + lhs.heuristic) ;
	}

};

int goal[3][3] = {{1,2,3},{4,5,6},{7,8,0}};
int expand = 0;
int maxnodes = 0;
int depth = 0;
//final check
bool goaltest(int arr[3][3])
{
	for (int i = 0; i < 3; i++)
	{
		for (int j =0; j < 3; j++)
		{
			if (arr[i][j] != goal[i][j])
			{
				return false;
			}
		}
	}
	return true;
}
//used to get the row and column of the blank
//****ALSO used for Manhattan//
void blank(int arr[3][3],int &row, int &col, int target)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j =0; j < 3; j++)
		{
			if (arr[i][j] == target)
			{
				row = i;
				col = j;
			}
		}
	}
}
//Misplaced Tiles
int mt(node&n)
{
	int count = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j <3; j++)
		{
			if(n.array[i][j] != goal[i][j])
			{
				count++;
			}
		}
	}
	return count;
}
//Manhattan Distance
int md(node&n)
{
	int count = 0;
	int x = 0;
	int y = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j <3; j++)
		{
			if (n.array[i][j] != 0)
			{
				//where i used blank, it just finds the goal placement
				blank(goal,x,y,n.array[i][j]);
				count += abs(i-x);
				count += abs(j-y);
			}
		}
	}
	return count;

}
//heuristics function
void heuy(node& n, int alg)
{
	if (alg == 1)
	{
		n.heuristic = 0;
	}
	else if (alg == 2)
	{
		n.heuristic = mt(n);
	}
	else if (alg == 3)
	{
		n.heuristic = md(n);
	}
}
//changes to the node all put into one
void alter(priority_queue<node>& q, node & n, int alg)
{

	n.weight++;
	heuy(n,alg);
	q.push(n);
	expand++;

}
//LEFT
void L(priority_queue<node>& q,node n, int row, int col, int a)
{
	int temp = n.array[row][col-1];
	n.array[row][col-1] = 0;
	n.array[row][col] = temp;

	alter(q,n,a);
}
//RIGHT
void R(priority_queue<node>& q,node n, int row, int col, int a)
{
	int temp = n.array[row][col+1];
	n.array[row][col+1] = 0;
	n.array[row][col] = temp;


	alter(q,n,a);
}
//UP
void U(priority_queue<node>& q,node n, int row, int col, int a)
{
	int temp = n.array[row-1][col];
	n.array[row-1][col] = 0;
	n.array[row][col] = temp;

	alter(q,n,a);
}
//DOWN
void D(priority_queue<node>& q,node n, int row, int col, int a)
{
	int temp = n.array[row+1][col];
	n.array[row+1][col] = 0;
	n.array[row][col] = temp;

	alter(q,n,a);
}

//QUEUEING FUNCTION
void queuingFunction(priority_queue<node>& nodes, node currentnode, int algorithm, int row, int col)
{
	node n = currentnode;
	n.parent = &currentnode;
//logic
//if on the top row, you only want down
	if (row == 0)
	{
		D(nodes,n,row,col,algorithm);
	}
//if in the middle, you want both
	else if (row ==1)
	{
		U(nodes,n,row,col,algorithm);
		D(nodes,n,row,col,algorithm);
	}
//etc
	else if (row == 2)
	{
		U(nodes,n,row,col,algorithm);
	}
	if (col == 0)
	{
		R(nodes,n,row,col,algorithm);
	}
	else if (col ==1)
	{
		R(nodes,n,row,col,algorithm);
		L(nodes,n,row,col,algorithm);
	}
	else if (col == 2)
	{
		L(nodes,n,row,col,algorithm);
	}

}

//brings all previous functions together
void search(int puzzle[3][3], int algorithm)
{
	node initial(puzzle);
	priority_queue <node> nodes;
	nodes.push(initial);
	node currentnode;
	int row = 0;
	int col = 0;
	int badcode = 0;
	do
	{
		currentnode = nodes.top();
		blank(currentnode.array, row, col, 0);
		

		if (badcode == 0)
		{
			cout << "Expanding state\n";
		}
		else
		{
			cout << "The best state to expand with a g(n} = ";
			cout << currentnode.weight;
			cout << " and h(n) = ";
			cout << currentnode.heuristic;
			cout << " is .. \n";
		}
		badcode ++;
		currentnode.print();
		int size = nodes.size();
		maxnodes = max(maxnodes, size);
		nodes.pop();
		if (goaltest(currentnode.array))
		{
			depth = currentnode.weight;
			cout << "YAAAY!!\n";
			return;
		}
		else
		{
			currentnode.heuristic = 0;
			queuingFunction(nodes, currentnode, algorithm, row, col);
		}
	}while(!nodes.empty());
}


void buildarray (int (&array)[3][3], int a, int b, int c, int d,
				int e, int f, int g, int h, int i)
{
	array [0][0] = a;
	array [0][1] = b;
	array [0][2] = c;
	array [1][0] = d;
	array [1][1] = e;
	array [1][2] = f;
	array [2][0] = g;
	array [2][1] = h;
	array [2][2] = i;

}


int main ()
{
	int opt;
	int alg;
	cout << "Welcome to my 8-puzzle solver!! \n";

	cout << "Type '1' to use a default puzzle, or '2' to enter your own \n";

	cin >> opt;

	int puzzle[3][3] ;

	while(opt != 560)
	{
		if (opt == 1)
		{
			cout << "Default puzzle : \n";
			buildarray(puzzle,1,2,3,4,0,6,7,5,8);
			opt = 560;
		}
		else if(opt == 2)
		{
			int a,b,c,d,e,f,g,h,i;
			cout << "Enter your puzzle one by one. Use 0 for the blank\n";
			cin >> a;
			cin >> b;
			cin >> c;
			cin >> d;
			cin >> e;
			cin >> f;
			cin >> g;
			cin >> h;
			cin >> i;
			buildarray(puzzle,a,b,c,d,e,f,g,h,i);
			opt = 560;
			cout << "Custome puzzle: \n";
			cout << a<<" "<<b<< " "<< c<< "\n";
			cout << d<<" "<<e<< " "<< f<< "\n";
			cout << g<<" "<<h<< " "<< i<< "\n";

		}
		else
		{
			cout << "Invalid number\n";

			cout << "Type '1' to use a default puzzle, or '2' to enter your own \n";

			cin >> opt;
		}
	}
	cout << "Enter your choice of algorithm\n";
	cout << "1. Uniform Cost Search\n";
	cout << "2. A* with Misplaced Tile heur.istic.\n";
	cout << "3. A* with Manhattan Distance heuristic\n";
	cin >> alg;
	while (alg != 1 && alg != 2 && alg != 3)
	{
			cout << "Invalid number \n";
			cout << "Enter your choice of algorithm\n";
			cout << "1. Uniform Cost Search\n";
			cout << "2. A* with Misplaced Tile heuristic.\n";
			cout << "3. A* with Manhattan Distance heuristic\n";

			cin >> alg;
	}

    search(puzzle, alg);	

    cout << "To solve this problem the search algorithm expanded a total of " << expand << " nodes.\n";
    cout << "The maximum number of nodes in the queue at any one time was " << maxnodes << ".\n";
    cout << "The depth of the goal node was " << depth << ".\n";
}