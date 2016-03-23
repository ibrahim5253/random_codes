#include<iostream>
#include<vector>

using namespace std;

/*
 * returns the index of key if it is present in 'a' (in case of multiple entries index of last occurance is returned)
 * if key is not present, the index of largest element smaller than key is returned.
 * found is set accordingly if key is present or not
 * Ofcourse, 'a' must be sorted
 */
int binary_search( int *a , int n , int key , bool &found )
{
	int low = 0 ;
	int high = n-1;
	found = false;
	if ( a[high] <= key ) {
		if (a[high]==key) found = true;
		return high;
	}
	if ( a[low] > key )
		return -1;	//key is smaller than all the elements of 'a'
	/*loop invariant:
		a[low] <= key
		a[high] > key
		*/
	while ( high - low > 1 ) {
		int mid = low + (high-low)/2;
		if ( a[mid] <= key ) 
			low = mid;
		else 
			high = mid;
	}
	if ( a[low] == key ) found = true;
	return low;
}

struct node 
{
	vector<int> keys;
	vector<node*> child;		
	bool isleaf;
};

// represents a key with its index in its node 'y'
class KEY
{
	public:
		node* y;
		int index;
		KEY(node *y , int index) {
			this->y = y;
			this->index = index;
		}
};

class B_tree
{
	private:
		int t;	
		/* t : minimum number of children of any internal node(except root which must have atleast two children and hence, one key).
		   => max. no. of children of a node = 2t, and
		   	  No. of keys in a node(except root) is between t-1 and 2t-1 (both inclusive)
		   A node is said to be full if it has 2t-1 number of keys or non-full otherwise
		*/
		node* root;
		void split_child(node*,int);
		void insert_nonfull(node*,int);
		int get_predecessor(node*);
		int get_successor(node*);
		
	public:
		B_tree (int t) {
			root = new node;
			root->isleaf = true;
			this->t = t;
		}
		node* getRoot() { return root; }
		KEY* search(node*,int);
		void insert(int);
		void Delete(node*,int);
		void Print(node*);
};

/*
 * splits the full ith child of x (x is non-full)
 */
void B_tree :: split_child(node* x , int i)
{
	node* z = new node;	//new node that would result after spliting child[i], i.e. 'y'
	node* y = x->child[i];
	z->isleaf = y->isleaf;	//'z' will be internal or leaf depending on 'y'
	for ( int j = 0 ; j < t-1 ; j++ )
		(z->keys).push_back(y->keys[t+j]);	//copy t th to (2t-1)th keys of 'y' to 'z'
	if (!y->isleaf)
		for ( int j = 0 ; j < t ; j++ )
			(z->child).push_back(y->child[t+j]);	//if 'y' is not leaf copy the corresponding children also
	int median = y->keys[t-1];
	/*
	 * delete the copied children and keys from 'y'
	 * (t-1)th key is also deleted, but has been stored in 'median' which will eventually
	 * get stored in 'x' (at ith index)
	 */
	(y->keys).erase( (y->keys).begin()+t-1 , (y->keys).end() );
	if ( !y->isleaf )
		(y->child).erase( (y->child).begin()+t , (y->child).end() ); 
	int n = (x->keys).size()+1;	//current number of children of x
	(x->child).push_back(NULL);	//increase number of children by 1
	for ( int j = n-1 ; j > i ; j-- ) 
		x->child[j+1] = x->child[j];	//shift (i+1)th to (n-1)th children one position right
	x->child[i+1] = z;	//make z (i+1)th child of x
	(x->keys).push_back(0);
	for ( int j = n-2 ; j >= i ; j-- )
		x->keys[j+1] = x->keys[j];
	x->keys[i] = median;
}

/*
 * insert 'k' in subtree rooted at non full node 'x'
 */
void B_tree :: insert_nonfull(node* x , int k)
{
	int i = (x->keys).size();
	if ( x->isleaf ) {	//if 'x' is a leaf node
		i--;
		(x->keys).push_back(0);	//increase the no. of keys by 1
		while ( i >= 0 && k < x->keys[i] ) {	//find the appropiate position of 'k' in x
			x->keys[i+1] = x->keys[i];	//shift the keys to right
			i--;
		}
		x->keys[i+1] = k; 
	}
	else {	//'x' is an internal node
		bool f;	//dummy variable, just required by 'binary_search'
		/*
		 * i : index of appropiate child of 'x' in which to insert 'k'
		 */
		i = binary_search ( &(x->keys[0]) , (x->keys).size() , k , f ) + 1; 
		if ( (x->child[i]->keys).size() == 2*t-1 ) {	//if child[i] is full
			split_child(x,i); //split it
			if ( x->keys[i] < k )	//find appropiate sub child
				i++;
		}
		insert_nonfull(x->child[i],k);	//recursive call on child[i]
	}
}

/*
 * returns predecessor of x's parent
 */
int B_tree :: get_predecessor(node* x)
{
	while ( !x->isleaf )
		x = (x->child).back();
	return (x->keys).back();
}

/*
 * returns successor of x's parent
 */
int B_tree :: get_successor(node* x)
{
	while ( !x->isleaf )
		x = x->child[0];
	return x->keys[0];
}

/*
 * returns a node containing 'k' and its index in it
 */
KEY* B_tree :: search(node* x,int k)
{
	bool found;	//flag to check if 'k' is found in 'x', is set by 'binary_search'
	int n = (x->keys).size();
	if (n == 0)
		return NULL;
	int i = binary_search( &(x->keys[0]) , n , k , found );
	if (found)
		return (new KEY(x,i));
	else if ( x->isleaf )
		return NULL;
	else
		search(x->child[i+1],k);
}

void B_tree :: insert(int k) 
{
	node* x = root;
	if ( (x->keys).size() == 2*t-1  ) { //if 'root' is full
		node* new_root = new node;
		root = new_root;	//assign a new root
		new_root->isleaf = false;
		(new_root->child).push_back(x);	//make old root(x) child of new root
		split_child(new_root,0);	//split 0th child of new root (i.e., x)
		insert_nonfull(new_root,k);
	}
	else 
		insert_nonfull(x,k);
}

void B_tree :: Delete ( node* x, int k )
{
	bool f;
	int n = (x->keys).size();
	if ( n == 0 )
		return;
	/*
	 * search 'k' in current node i.e., 'x'
	 */
	int i = binary_search( &(x->keys[0]) , n , k , f );
	if (f) {	//if 'k' is found in 'x'
		if ( x->isleaf )	//if 'x' is leaf just delete 'k' from it
			(x->keys).erase( (x->keys).begin() + i );
		else {	//if 'x' is an internal node
			node* y = x->child[i];	//node left to 'k'
			node* z = x->child[i+1];	//node right to 'k'
			if ( (y->keys).size() >= t ) {		//if 'y' has atleast t keys
				int k1 = get_predecessor(y);	//find predecessor of 'x'
				x->keys[i] = k1;	//replace 'k' by k1 
				Delete(y,k1); 		//recursively call Delete on 'y' for 'k1'
			}
			/*
			 * if 'y' has onlt (t-1) keys, check symmetrically for z
			 */
			else if ( (z->keys).size() >= t ) {
				int k2 = get_successor(z);
				x->keys[i] = k2;
				Delete(z,k2);
			}
			/*
			 * if both 'y' and 'z' have only (t-1) keys, merge 'z' into 'y'
			 * also move 'k' from 'x' to 'y' and it the median of 'y'
			 * then recursively call Delete on 'y' 
			 */
			else {
				(y->keys).push_back(x->keys[i]);
				for ( int j = 0 ; j < (z->keys).size() ; j++ )
					(y->keys).push_back( z->keys[j] );
				if ( !y->isleaf )
					for ( int j = 0 ; j < (z->child).size() ; j++ )
						(y->child).push_back( z->child[j] );
				(x->keys).erase( (x->keys).begin()+i );
				(x->child).erase( (x->child).begin()+i+1 );
				delete z;
				if ( x == root && (x->keys).size() == 0 )
					root = y;
				Delete(y,k);
			}
		}
	}
	else { //if 'k' is not present in 'x'
		if (x->isleaf)	//if 'x' is leaf, 'k' doesn't exist in tree
			return;
		i++;
		node* c = x->child[i];	//appropiate child which can have 'k'
		/*
		 * if 'c' has only (t-1) keys, make it have atleast t keys by either borrowing from siblings(if and else if blocks)
		 * or merging with one of the siblings(else block)
		 */
		if ( (c->keys).size() < t ) {
			node* l_sib = (i-1>=0)?x->child[i-1]:NULL;
			node* r_sib = (i+1<=n)?x->child[i+1]:NULL;
			if ( l_sib && (l_sib->keys).size() >= t ) {
				(c->keys).insert( (c->keys).begin() , x->keys[i-1] );
				x->keys[i-1] = (l_sib->keys).back();
				(l_sib->keys).pop_back();
				if (!c->isleaf) {
					(c->child).insert( (c->child).begin() , (l_sib->child).back() );
					(l_sib->child).pop_back();
				}
			}	
			else if ( r_sib && (r_sib->keys).size() >= t ) {
				(c->keys).push_back( x->keys[i] );
				x->keys[i] = r_sib->keys[0];
				(r_sib->keys).erase( (r_sib->keys).begin() );
				if (!c->isleaf) {
					(c->child).push_back( r_sib->child[0] );
					(r_sib->child).erase( (r_sib->child).begin() );
				}
			}
			else {
				if ( !r_sib ) {
					r_sib = c;
					c = l_sib;
					i--;
				}
				(c->keys).push_back(x->keys[i]);
				for ( int j = 0 ; j < (r_sib->keys).size() ; j++ )
					(c->keys).push_back( r_sib->keys[j] );
				if ( !c->isleaf )
					for ( int j = 0 ; j < (r_sib->child).size() ; j++ )
						(c->child).push_back( r_sib->child[j] );
				(x->keys).erase( (x->keys).begin()+i );
				(x->child).erase( (x->child).begin()+i+1 );
				delete r_sib;
				if ( x == root && (x->keys).size() == 0 )
					root = c;
			}
		}
		Delete(c,k);
	}
}

/*
 * print the content of the tree (inorder)
 */
void B_tree :: Print(node* x)
{
	int n = (x->keys).size();
	if (!x->isleaf) {
		for ( int i = 0 ; i < n ; i++ ) {
			Print(x->child[i]);
			cout << x->keys[i] << " ";	
		}
		Print(x->child[n]);
	}
	else {
		for ( int i = 0 ; i < n ; i++ )
			cout << x->keys[i] << " ";
	}
}

int main()
{	
	int n;
	cout << "Minimum no. of children a node(except root) should have: ";
	cin >> n;
	
	B_tree BT(n);
	while (1) {
		cout << "\n1. Search\n2. Insert\n3. Delete\n4. Print\n5. Exit\nEnter your choice: ";
		int ch , ele; cin >> ch;
		KEY *p;
		switch(ch) {
			case 1:
				cout << "Element to be searched: ";
				cin >> ele;
				p = BT.search(BT.getRoot(),ele);
				if (!p)
					cout << ele << " doesn't exist\n";
				else {
					cout << p->y->keys[p->index] << " exists at " << p->index << " in its node\n";
					cout << "contents of the node are (in that order): ";
					for ( int i = 0 ; i < (p->y->keys).size() ; i++ )
						cout << p->y->keys[i] << " ";
				}
				break;
			case 2:
				cout << "Element to be inserted: ";
				cin >> ele;
				BT.insert(ele);
				break;
			case 3:
				cout << "Element to be deleted: ";
				cin >> ele;
				BT.Delete(BT.getRoot(),ele);
			case 4:
				BT.Print(BT.getRoot());
				break;
			case 5:
				return 0;
			default:
				cout << "Enter a valid option!\n";
		}
	}
	return 0;
}
