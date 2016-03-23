/*
 * NAME: 			IBRAHIM DALAL
 * ROLL NO.:			CS14BTECH11041
 * 
 * Problem Statement:	kd Tree implementation
 */

#include<iostream>
#include<vector>
#include<climits>
#include<cmath>
#include<limits>

using namespace std;

template <class T>
void input (T& m)
{
	cin >> m;
	while (cin.fail()) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(),'\n');
		cin >> m;
	}
}

//returns the squared distance between two given point
int sq_distance ( vector<int>& a , vector<int>& b )
{
	int dis = 0;
	int len = a.size();
	for ( int i = 0 ; i < len ; i++ ) 
		dis += (a[i]-b[i])*(a[i]-b[i]);
	return dis;
}

//prints the coordinates of a given point
void print_point ( vector<int>& a )
{
	int len = a.size();
	cout << "(";
	for ( int i = 0 ; i < len ; i++ )
		cout << a[i] << ((i+1==len)?")":",");
}

//denotes a point in k-dimensional space
class node {
	public:
		string name;	//caption
		vector<int> co_od;	//co-ordinates
		node* right;
		node* left;
		node(string,vector<int>&); 
};

node :: node(string name , vector<int>& co_od )
{
	this->name = name;
	this->co_od = co_od;
	right = NULL;
	left = NULL;
} 

class kdTree {
	private:
		int K;	//tree dimension
		node* root;
		void nn_recurse ( vector<int>& , node* , int , node*& , int& );
	public:
		kdTree ( int K ) {
			this->K = K;
			root = NULL;
		}
		node* getRoot() { return root; }
		void insert(string,vector<int>);
		node* search(vector<int>&);
		node* nearest_neighbour(vector<int>&);
		void Print(node*);
};

//recursive function to find the nearest neighbour of a given point 'p'
void kdTree :: nn_recurse ( vector<int>& p , node* curr , int i , node*& curr_best , int& min )
{
	if ( curr == NULL )
		return;
	node* other;
	//moving to an appropiate node
	if ( curr->co_od[i] <= p[i] ) {
		nn_recurse(p,curr->right,(i+1)%K,curr_best,min); 
		other = curr->left;	
	}
	else {
		nn_recurse(p,curr->left,(i+1)%K,curr_best,min);
		other = curr->right;
	}
	
	int d = sq_distance(curr->co_od,p);
	if ( d < min ) {	//if current node is nearer to 'p' than current best
		curr_best = curr;	//update current best to current
		min = d;	//and minimum squared distance to d
	}
	
	int t = abs(curr->co_od[i]-p[i]);
	if ( t*t < min )
		/*
		 *if hypersphere of radius 'min' centered at 'p' intersects the
		  splitting hyperplane at current node, check the other side of the plane
		 */
		nn_recurse(p,other,(i+1)%K,curr_best,min);
}

void kdTree :: insert ( string name , vector<int> co_od )
{
	node* p = new node(name,co_od);
	node* x = root;
	node* y = NULL;
	int i = 0;
	while ( x != NULL ) {
		y = x;
		if ( x->co_od[i] <= co_od[i] )
			x = x->right;
		else
			x = x->left;
		i = (i+1)%K;
	}
	if ( y == NULL )
		root = p;
	else if ( y->co_od[(i-1+K)%K] <= co_od[(i-1+K)%K] )
		y->right = p;
	else
		y->left = p;
}

node* kdTree :: search( vector<int>& co_od )
{
	node *x = root;
	int i = 0;
	while ( x!=NULL && x->co_od != co_od ) {
		if ( x->co_od[i] <= co_od[i] )
			x = x->right;
		else
			x = x->left;
		i = (i+1)%K;
	}
	return x;
}

node* kdTree :: nearest_neighbour (vector<int>& p)
{
	node* nn = NULL;
	int min_dis = INT_MAX;
	nn_recurse(p,root,0,nn,min_dis);
	if (nn) {
		cout << "Point nearest to ";
		print_point(p);
		cout << " is at a distance of " << pow(min_dis,0.5) << " from it and is ";
	}
	return nn;
}

void kdTree :: Print (node* x)
{
	if ( x == NULL )
		return;
	Print(x->left);
	if ( x == root ) cout << "*";
	cout << x->name; print_point(x->co_od); cout << " ";
	Print(x->right);
}

int main()
{
	cout << "Enter dimension of the kd-tree(>=1): ";
	int k;
	do {
		input<int>(k);
	}while (k<1);
	
	kdTree T(k);
	
	while (1) {
		cout << "1. Search\n2. Insert\n3. Find a nearest neighbour\n4. Print\n5. Exit\nEnter your choice: ";
		int ch; input<int>(ch);
		vector<int> point(k);
		node* p; string name;
		switch(ch) {
			case 1:
				cout << "Enter the co-ordinates of the point to be searched: ";
				for ( int i = 0 ; i < k ; i++ )
					input<int>(point[i]);
				p = T.search(point);
				if (p) {
					print_point(p->co_od);
					cout << " is present and is denoted by " << p->name << "\n";
				}
				else {
					print_point(point);
					cout << " doesn't exist\n"; 
				}
				break;
			case 2:
				cout << "Enter the co-ordinates of the point to be inserted: ";
				for ( int i = 0 ; i < k ; i++ )
					input<int>(point[i]);
				cout << "Enter a caption for the point: ";
				input<string>(name);
				if ( p = T.search(point) ) {
					print_point(p->co_od);
					cout << " is already present and is denoted by " << p->name << "\n";
				}
				else
					T.insert(name,point);
				break;
			case 3:
				cout << "Enter a point: ";
				for ( int i = 0 ; i < k ; i++ )
					input<int>(point[i]);
				p = T.nearest_neighbour(point);
				if (p) {
					cout << p->name;
					print_point(p->co_od);
					cout << endl;
				}
				else {
					cout << "No nearest point\n";
				}
				break;
			case 4:
				T.Print(T.getRoot());
				cout << endl;
				break;
			case 5:
				return 0;
			default:
				cout << "Enter a valid choice!\n";
		}
	}
	return 0;
}

/*===============================================================

	Sample Input/Output

Enter dimension of the kd-tree(>=1): 2
1. Search
2. Insert
3. Find a nearest neighbour
4. Print
5. Exit
Enter your choice: 2
Enter the co-ordinates of the point to be inserted: 0 0
Enter a caption for the point: O
1. Search
2. Insert
3. Find a nearest neighbour
4. Print
5. Exit
Enter your choice: 2
Enter the co-ordinates of the point to be inserted: 2 3
Enter a caption for the point: A
1. Search
2. Insert
3. Find a nearest neighbour
4. Print
5. Exit
Enter your choice: 1
Enter the co-ordinates of the point to be searched: 0 0
(0,0) is present and is denoted by O
1. Search
2. Insert
3. Find a nearest neighbour
4. Print
5. Exit
Enter your choice: 1
Enter the co-ordinates of the point to be searched: 2 5
(2,5) doesn't exist
1. Search
2. Insert
3. Find a nearest neighbour
4. Print
5. Exit
Enter your choice: 3
Enter a point: 1 1
(0,0) is the nearest point and is denoted by O
1. Search
2. Insert
3. Find a nearest neighbour
4. Print
5. Exit
Enter your choice: 3
Enter a point: 4 1
(2,3) is the nearest point and is denoted by A
1. Search
2. Insert
3. Find a nearest neighbour
4. Print
5. Exit
Enter your choice: 4
O(0,0) A(2,3) 
1. Search
2. Insert
3. Find a nearest neighbour
4. Print
5. Exit
Enter your choice: 5

===============================================================*/
