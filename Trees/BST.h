#ifndef BST_H
#define BST_H

#include<iostream>
#include<queue>

using namespace std;

template <class T>
class TreeNode 
{
	private:
		T* data;
		TreeNode* parent;
		TreeNode* left_child;
		TreeNode* right_child;
	public:
		TreeNode ();
		void set_p(TreeNode* ptr);
		void set_lchild(TreeNode* ptr);
		void set_rchild(TreeNode* ptr);
		void set_data(T* x);
		T* get_data();
		TreeNode* get_p();
		TreeNode* get_lchild();
		TreeNode* get_rchild();
};

template <class T>
TreeNode<T> :: TreeNode () 
{
	data = NULL;
	parent = left_child = right_child = NULL;
}

template <class T>
void TreeNode<T> ::  set_p(TreeNode<T> *ptr)
{
	parent = ptr;
}	

template <class T>
void TreeNode<T> ::  set_lchild(TreeNode<T> *ptr)
{
	left_child = ptr;
}	

template <class T>
void TreeNode<T> ::  set_rchild(TreeNode<T> *ptr)
{
	right_child = ptr;
}

template <class T>
void TreeNode<T> :: set_data(T* x)
{
	if (data != NULL)
		delete data;
	if (x == NULL)
		data = NULL;
	else 
		data = new T(*x);
}	

template <class T>
TreeNode<T>* TreeNode<T> :: get_p ()
{
	return parent;
}

template <class T>
TreeNode<T>* TreeNode<T> :: get_lchild ()
{
	return left_child;
}	

template <class T>
TreeNode<T>* TreeNode<T> :: get_rchild ()
{
	return right_child;
}

template <class T>
T* TreeNode<T> ::  get_data ()
{
	return data;
}

template <class T>
class BST
{
	private:
		TreeNode<T> *root;
	public:
		BST() {
			root = NULL; }
		TreeNode<T>* get_root() {
			return root; }
		T* Insert (T* datap);
		void Delete (T* datap);
		T* Search (T* datap);	//returns pointer to element 'datap'
		TreeNode<T>* search (T* datap);		//returns pointer to node containing 'datap'
		T* Min();
		T* Max();
		T* InOrderSucc (T* datap);
		T* InOrderPred (T* datap);
		int Height(TreeNode<T>*);
		void PrintInOrder(TreeNode<T>*);
		void PrintPreOrder(TreeNode<T>*);
		void PrintPostOrder(TreeNode<T>*);
};

template <class T>
T* BST<T> :: Insert (T* datap) 
{
	TreeNode<T> *current;
	if ( Search(datap) == NULL ) {	//if element not present
		TreeNode<T> *node = new TreeNode<T> ;	//creates a new node
		node->set_data(datap);
		if (root == NULL) //if tree is null
			root = node;
		else {
			current = root;	
			while ( 1 ) {	//determining parent of the element to be inserted
				if ( *datap < *current->get_data() ) {
					if ( current->get_lchild() ) 
						current = current->get_lchild();
					else
						break; }
				else {
					if ( current->get_rchild() ) 
						current = current -> get_rchild();
					else
						break;} }
			node->set_p(current);
			if ( *datap < *current->get_data() )
				current -> set_lchild(node);
			else
				current -> set_rchild(node);}
		return node->get_data(); }
	else 
		return Search (datap);
}

template <class T>
void BST<T> :: Delete (T* datap) 
{
	TreeNode<T> *current , *pred;
	if (Search (datap)) {	//if element is present
		current = search(datap);
		if ( ( !current->get_lchild() ) && ( !current->get_rchild() ) ) {	//element is a leaf node
			if ( current == root )
				root = NULL;
			else {  if (current == current->get_p()->get_lchild())
						current -> get_p() -> set_lchild(NULL);
					else 
						current -> get_p() -> set_rchild(NULL); }
			current -> set_p(NULL);
			current -> set_data(NULL);
			delete current;}
		else if ( current->get_lchild()  && ( !current->get_rchild() ) ) {	//element has a left child but not a right one
			current -> get_lchild() -> set_p ( current -> get_p() );
			if ( current == root ) 
				root = current->get_lchild();
			else {
				if ( current->get_p()->get_lchild() == current )
					current->get_p()->set_lchild(current->get_lchild());
				else
					current->get_p()->set_rchild(current->get_lchild()); }
			current->set_p(NULL);
			current->set_lchild(NULL);
			current->set_data(NULL);
			delete current;  }
		else if ( current->get_rchild()  && ( !current->get_lchild() ) ) {	//element has a right child but not a left one
			current -> get_rchild() -> set_p ( current -> get_p() );
			if ( current == root ) 
				root = current->get_rchild();
			else {
				if ( current->get_p()->get_lchild() == current )
					current->get_p()->set_lchild(current->get_rchild());
				else
					current->get_p()->set_rchild(current->get_rchild()); }
			current->set_p(NULL);
			current->set_rchild(NULL);
			current->set_data(NULL);
			delete current;	}
		else{																//element has both the child
			pred = current->get_lchild();
			while (pred->get_rchild())
				pred = pred->get_rchild();
			current->set_data(pred->get_data());
			if (pred->get_lchild()) {
				pred->get_lchild()->set_p(pred->get_p());
				if (pred == current->get_lchild())
					current->set_lchild(pred->get_lchild());
				else
					pred->get_p()->set_rchild(pred->get_lchild());}
			else {
				if ( pred == current->get_lchild() )
					current->set_lchild(NULL);
				else 
					pred->get_p()->set_rchild(NULL);}
			pred->set_p(NULL);
			pred->set_lchild(NULL);
			pred->set_data(NULL);
			delete pred; } }
	else 	//element not present in tree
		cout << "\nData not found";		
}

template <class T>
T* BST<T> :: Search (T* datap)
{
	TreeNode<T> *current = root;
	while ( current != NULL) {	//binary search for '*datap'
		if ( *current->get_data()  == *datap )
			return current->get_data();
		else if ( *current->get_data() > *datap )
			current = current->get_lchild();
		else
			current = current->get_rchild();
	}
	return NULL;
}

template <class T>
TreeNode<T>* BST<T> :: search (T* datap)
{
	TreeNode<T> *current = root;
	while ( current ) {
		if ( *current->get_data()  == *datap )
			return current;
		else if ( *current->get_data() > *datap )
			current = current->get_lchild();
		else
			current = current->get_rchild();
	}
	return NULL;
}

template <class T>
T* BST<T> :: Min()
{
	TreeNode<T> *current = root;
	while ( current->get_lchild() )
		current = current->get_lchild();	//move to the leftmost node
	return current->get_data();
}

template <class T>
T* BST<T> :: Max()
{
	TreeNode<T> *current = root;
	while ( current->get_rchild() )
		current = current->get_rchild();	//move to the rightmost node
	return current->get_data();
}

template <class T>
T* BST<T> :: InOrderSucc(T* datap)
{
	TreeNode<T> *x = search(datap) , *current ,*y;
	if (x->get_rchild()) {	//if 'x' has a right child
		current = x->get_rchild();	//move right
		while (current->get_lchild())
			current = current -> get_lchild();	//go straight to left most node
		return current->get_data();}
	y = x->get_p();	// if no right child
	while ( (y != NULL) && (x == y->get_rchild()) ) {
		x = y;	//go up , till 'x' is a right child of its parent
		y = y->get_p(); }
	if (y)
		return y->get_data();
	else 
		return NULL;	//no successor
}

template <class T>
T* BST<T> :: InOrderPred(T* datap)
{
	TreeNode<T> *x = search(datap) , *current ,*y;
	if (x->get_lchild()) {	//if left child is there
		current = x->get_lchild();	//move left
		while (current->get_rchild())	
			current = current -> get_rchild();	//straight down to rightmost child
		return current->get_data();}
	y = x->get_p();//if no left child
	while ( (y != NULL) && (x == y->get_lchild()) ) {
		x = y;	//move up ,till 'x' is leftchild of its parent
		y = y->get_p(); }
	if (y)
		return y->get_data();
	else
		return NULL;
}

template <class T>
int BST<T> :: Height(TreeNode<T>* node)
{
	if (node == NULL)
		return 0;
	else {
		int l_rsubtree = Height(node->get_rchild()) + 1;	//height of left subtree + 1
		int l_lsubtree = Height(node->get_lchild()) + 1;	//height of right subtree + 1
		if ( l_rsubtree > l_lsubtree )
			return l_rsubtree;
		else
			return l_lsubtree; }
}

template <class T>
void BST<T> :: PrintInOrder(TreeNode<T>* node)
{
	if (node == NULL) 
		return ; 
	PrintInOrder( node->get_lchild() );
	cout << *(node->get_data()) << "\t";
	PrintInOrder( node->get_rchild() );
}

template <class T>
void BST<T> :: PrintPreOrder(TreeNode<T>* node)
{
	if (node == NULL) 
		return ; 
	cout << *(node->get_data()) << "\t";
	PrintInOrder( node->get_lchild() );
	PrintInOrder( node->get_rchild() );
}

template <class T>
void BST<T> :: PrintPostOrder(TreeNode<T>* node)
{
	if (node == NULL) 
		return ; 
	PrintInOrder( node->get_lchild() );
	PrintInOrder( node->get_rchild() );
	cout << *(node->get_data()) << "\t";
}

#endif
