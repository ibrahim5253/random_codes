#include<iostream>

using namespace std;

//represents a tree node
struct node {
	int key;
	char color;
	node* parent;
	node* right;
	node* left;
};

//Red Black Tree
class RB_Tree {
	private:
		node* root;	//root of the tree
		node* nil;	//sentinel 
		void left_rotate(node*);
		void right_rotate(node*);
		void restore_rb_property(node*);
	public:
		RB_Tree () {
			nil = new node;
			nil->key = -1;
			nil->color = 'B';
			nil->parent = nil->right = nil->left = nil;
			root = nil;
		}
		node* getRoot() { return root; }
		node* Search (int);
		void Insert (int);
		void Print (node*);
};

//replaces x with its right child or left rotation
void RB_Tree :: left_rotate(node* x)
{
	node* y = x->right;
	if ( y != nil ) {
		x->right = y->left;
		if (y->left != nil)
			y->left->parent = x;
		y->parent = x->parent;
		if ( x->parent == nil )
			root = y;
		else if ( x->parent->left == x )
			x->parent->left = y;
		else
			x->parent->right = y;
		y->left = x;
		x->parent = y;
	}
}


//replaces x with its left child or right rotation
void RB_Tree :: right_rotate(node* x)
{
	node* y = x->left;
	if ( y != nil ) {
		x->left = y->right;
		if (y->right != nil)
			y->right->parent = x;
		y->parent = x->parent;
		if ( x->parent == nil )
			root = y;
		else if ( x->parent->right == x )
			x->parent->right = y;
		else
			x->parent->left = y;
		y->right = x;
		x->parent = y;
	}
}

/*
 * restores any rb_tree property that has been violated after insertion
 */
void RB_Tree :: restore_rb_property(node* z) 
{
	while ( z->parent->color == 'R' ) {	//two adjacent nodes of red color (z,z->parent)
		if ( z->parent == z->parent->parent->left ) {
			node* y = z->parent->parent->right;	//y : uncle of z
			if ( y->color == 'R' ) {	//y is red
				y->color = 'B';
				z->parent->color = 'B';
				z->parent->parent->color = 'R';
				z = z->parent->parent;	//move z to its grand parent
			}
			else{ //y is black
				if ( z == z->parent->right ) {
					z = z->parent;
					left_rotate(z);
				}
				z->parent->color = 'B';
				z->parent->parent->color = 'R';
				right_rotate(z->parent->parent);
			}
		}
		else {	//same as if part just left , right interchanged
			node* y = z->parent->parent->left;
			if ( y->color == 'R' ) {
				y->color = 'B';
				z->parent->color = 'B';
				z->parent->parent->color = 'R';
				z = z->parent->parent;
			}
			else{ 
				if ( z == z->parent->left ) {
					z = z->parent;
					right_rotate(z);
				}
				z->parent->color = 'B';
				z->parent->parent->color = 'R';
				left_rotate(z->parent->parent);
			}			
		}
	}
	root->color = 'B';
}

node* RB_Tree :: Search(int key)
{
	node* x = root;
	while ( x != nil && x->key != key ) {
		if ( x->key > key )
			x = x->left;
		else
			x = x->right;
	}
	if ( x == nil )
		return NULL;
	else
		return x;	
}

void RB_Tree :: Insert(int key)
{
	node* p = new node;
	p->key = key;
	/*
	 * find the parent of node to be inserted i.e., find 'y'
	 */
	node* y = nil;
	node* x = root;
	while (x != nil) {
		y = x;
		if ( x->key < key )
			x = x->right;
		else
			x = x->left;
	}
	p->parent = y;
	if ( y == nil )	//tree is empty
		root = p;
	else if ( y->key < key )
		y->right = p;
	else
		y->left = p;
	p->left = nil;
	p->right = nil;
	p->color = 'R';
	restore_rb_property(p);
}

/*
 * Prints Tree elements(inorder traversal)
 * 'r' after an element represents that it is a red node
 * and similarly 'b' for black node
 */
void RB_Tree :: Print(node* x)
{
	if (x!=nil) {
		Print(x->left);
		cout << x->key << ((x->color=='R')?'r':'b') << " ";
		Print(x->right);
	}
}

int main()
{
	RB_Tree T;
	while (1) {
		cout << "\n1. Search\n2. Insert\n3. Print\n4. Exit\nEnter your choice: ";
		int ch,ele;
		cin >> ch;
		switch(ch) {
			case 1:
				cout << "Element to be searched: ";
				cin >> ele;
				if (T.Search(ele))
					cout << T.Search(ele)->key << " is present and its color is " << ((T.Search(ele)->color == 'R')?"red":"black");
				else
					cout << ele << " is not present.";
				break;
			case 2:
				cout << "Element to be inserted: ";
				cin >> ele;
				T.Insert(ele);
				break;
			case 3:
				T.Print(T.getRoot());
				break;
			case 4:
				return 0;
			default:
				cout << "Enter a valid option!";
		}
	}
	return 0;
}
