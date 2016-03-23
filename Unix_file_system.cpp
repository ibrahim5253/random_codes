/* Illustration of directory system in linux
	using general rooted tree */

#include<iostream>
#include<string>
#include<vector>

using namespace std;

class TreeNode
{
	private:
		bool isfolder;	//Node is a folder or file
		string String;	//Node name
		TreeNode *parent_p;
		TreeNode *leftmostchild_p;
		TreeNode *rightsibling_p;
		
	public:
		TreeNode();
		void setparent( TreeNode* p );
		void setleftmostchild( TreeNode* p );
		void setrightsibling( TreeNode* p );
		void set_isfolder (bool);	//sets whether node is a folder or file
		void set_name (string);		//sets the name of the node
		TreeNode* getparent();
		TreeNode* getleftmostchild();
		TreeNode* getrightsibling();
		bool get_isfolder();
		string get_name();
}; 

TreeNode :: TreeNode()
{
	parent_p = leftmostchild_p = rightsibling_p = NULL;
}

void TreeNode :: setparent( TreeNode* p )
{
	parent_p = p;
}

void TreeNode :: setleftmostchild( TreeNode* p )
{
	leftmostchild_p = p;
}

void TreeNode :: setrightsibling( TreeNode* p )
{
	rightsibling_p = p;
}

void TreeNode :: set_isfolder( bool x )
{
	isfolder = x;
}

void TreeNode :: set_name( string name )
{
	String = name;
}

TreeNode* TreeNode :: getparent()
{
	return parent_p;
}

TreeNode* TreeNode :: getleftmostchild()
{
	return leftmostchild_p;
}

TreeNode* TreeNode :: getrightsibling()
{
	return rightsibling_p;
}

bool TreeNode :: get_isfolder ()
{
	return isfolder;
}

string TreeNode :: get_name ()
{
	return String;
}

int ispresent( TreeNode *ptr , string name )	// determines if a node is present in tree
{												// and returns if its a folder (1) , file (0)
	if (ptr) {									// or not present (-1)
		while (ptr) {
			if ( ptr->get_name() == name ) {
				if (ptr->get_isfolder())
					return 1;
				else 
					return 0;}
			ptr = ptr->getrightsibling(); }
			return -1; }
	else 
		return -1;
}

class RootedTree 
{
	private :
		TreeNode *root , *current_folder_pointer;
		
	public :
		RootedTree();
		TreeNode* get_current_pointer();
		void set_current_pointer (TreeNode *ptr);
		int set_directory( string operand );
		void print_path();	//prints the absolute path of cwd
		void add_folder(string name);
		void add_file( string name );
		void delete_file(string operand);
		void print_folder( );	//prints the contents of a folder
		void print_entire_folder( TreeNode *current , int level = 1 );	//recursively prints entire contents of folder
		void copy_file (string operand);
		void move_file (string operand);
		void copy_folder(TreeNode *source , TreeNode *destination);
		void deleteSubFolder(TreeNode *p);	//deletes a folder rooted at p
};

RootedTree :: RootedTree()	//ctor , 
{
	root = new TreeNode;	//creates a root folder
	root -> set_name("/");
	root -> setparent (root);
	root -> setrightsibling (NULL);
	root -> setleftmostchild (NULL);
	root -> set_isfolder (true);
	current_folder_pointer = root;	// and initialises current_folder_pointer to root
}

TreeNode* RootedTree :: get_current_pointer()
{
	return current_folder_pointer;
}

void RootedTree :: set_current_pointer( TreeNode *ptr )
{
	current_folder_pointer = ptr;
}

int RootedTree :: set_directory ( string operand )
{
	int  start , i = 0;
	string step;
	TreeNode *sub_folder , *temp;
	operand.append("/\0");
				if ( operand[i] == '/' ) {				//determines if the path is
					current_folder_pointer = root;		//absolute or relative (else) 
					start = i+1; }
				else 
					start = i;
				temp = current_folder_pointer;
				while ( ( i = operand.find("/",start) ) != std::string::npos ) {	//parsing operand with '/' as delimiter
					step = operand.substr(start , i-start );  	//a component of path
					if ( step == ".." )														//if component is '..'
						current_folder_pointer = current_folder_pointer -> getparent() ;	//go to the parent
					else if ( step == "." );	//if '.' , do nothing
					else if ( step.length() == 0 ); // two consecutive '/' , do nothing
					else if ( step[0] == ' ' ) // space encountered , quit , neglect the rest of path
						break;
					else {
						if (current_folder_pointer -> getleftmostchild() ) {	//childrens are there of current dir
							sub_folder = current_folder_pointer -> getleftmostchild();	//go to left most child
							while ( ( sub_folder -> get_name() != step ) && ( sub_folder -> getrightsibling() ) ) //traverse through
								sub_folder = sub_folder -> getrightsibling(); //the children list , until component found or no more childs
							if ( sub_folder->get_name() != step ) { //component not found
								cout << "\nNo such directory exists!";
								current_folder_pointer = temp;
								return 0; }
							if ( sub_folder -> get_isfolder() ) //component found and is a folder
								current_folder_pointer = sub_folder;
							else { //component found but not a dir
								cout << "\n" << operand << "is not a directory";
								current_folder_pointer = temp;
								return 0; } }
						else { //no children of current dir
							cout << "\nNo such directory exists!";
							current_folder_pointer = temp;
							return 0; } }
					start = i+1; }
	return 1;
}

void RootedTree :: print_path()
{
	vector<string> stack;
	TreeNode *temp = current_folder_pointer;
	while ( temp != root ) { //move upto the root
		stack.push_back( temp -> get_name() );	//push the components of the way in stack
		temp = temp->getparent(); }
	cout << "\n/";
	for (int j = stack.size() - 1; j >= 0 ; j--) 
		cout << stack[j] << "/";	//prints the absolute path, top-bottom
	stack.clear();
}

void RootedTree :: add_folder (string name)
{
	TreeNode *new_folder =  new TreeNode , *current;
	new_folder -> setparent( current_folder_pointer );
	new_folder -> setrightsibling( NULL );
	new_folder -> setleftmostchild( NULL );
	new_folder -> set_isfolder(true);
	new_folder -> set_name (name);
	if ( current_folder_pointer -> getleftmostchild() ) { //current dir has some children
			current = current_folder_pointer -> getleftmostchild();
			while ( current->getrightsibling() ) 
				current = current -> getrightsibling();
			current -> setrightsibling( new_folder );}
	else	//no child 
			current_folder_pointer->setleftmostchild(new_folder);
}

void RootedTree :: add_file (string name)
{
	TreeNode *new_folder =  new TreeNode , *current;
	new_folder -> setparent( current_folder_pointer );
	new_folder -> setrightsibling( NULL );
	new_folder -> setleftmostchild( NULL );
	new_folder -> set_isfolder(false);
	new_folder -> set_name (name);
	if ( current_folder_pointer -> getleftmostchild() ) {
			current = current_folder_pointer -> getleftmostchild();
			while ( current->getrightsibling() ) 
				current = current -> getrightsibling();
			current -> setrightsibling( new_folder );}
	else
			current_folder_pointer->setleftmostchild(new_folder);
}

void RootedTree :: delete_file(string operand)
{
		TreeNode *current , *previous;
		current = current_folder_pointer->getleftmostchild();
		previous = NULL;
		while ( current->get_name() != operand ) {	//reach the required file in children list of current folder
			previous = current;
			current = current -> getrightsibling() ; }
		if ( previous == NULL ) 	//to be deleted file is left most child
			current_folder_pointer -> setleftmostchild(current->getrightsibling()); 	//its right sibling made left most child
		else 
			previous -> setrightsibling( current->getrightsibling() ); 
		current->setparent(NULL);
		current->setrightsibling(NULL);
		delete current;
}

void RootedTree :: print_folder()
{
	TreeNode *current = current_folder_pointer->getleftmostchild();
	if (current == NULL) {
		cout << "\nFolder Empty!";
		return; }
	while (current) {
		if ( current->get_isfolder() )
			cout << current->get_name() << " -Folder\n";
		else
			cout << current->get_name() << " -File\n";
		current = current -> getrightsibling(); }
}

void RootedTree :: print_entire_folder(TreeNode *current , int level )
{
	for (int i=1 ; i < level ; i++)
		cout << "	";
	cout << current->get_name() << (current->get_isfolder() ? "  -Folder" : "  -File") << "\n";
	if ( current->getleftmostchild() )
		print_entire_folder (current->getleftmostchild() , level+1);
	if ( current->getrightsibling() )
		print_entire_folder (current->getrightsibling() , level);
}	

void RootedTree :: copy_file ( string operand )
{
	int i , start = 0;
	string to_be_copied , where_to_copy , file_name;
	TreeNode *temp = current_folder_pointer;
	i = operand.find(" ",0);
	to_be_copied = operand.substr(0,i);
	while ( operand[i] == ' ' )
		i++;
	where_to_copy = operand.substr(i);
	while ( (i = to_be_copied.find("/",start)) != std::string::npos) 
		start = i+1; 
	file_name = to_be_copied.substr(start);
	to_be_copied.erase(start);
	set_directory (to_be_copied); 
	if ( ispresent (current_folder_pointer -> getleftmostchild() , file_name) == 0 ) {
		current_folder_pointer = temp;
			if (set_directory (where_to_copy)  == 1) {
				if (ispresent(current_folder_pointer->getleftmostchild() , file_name) != 0) 
					add_file (file_name);
				else
					cout << "\n" << file_name << "already exists in " << current_folder_pointer->get_name();}	} 
	else 
		cout << "\n" << file_name << " doesn't exist at given location";
	current_folder_pointer = temp;
}	

void RootedTree :: move_file ( string operand )
{
	int i , start = 0;
	string move_from , move_to , file_name;
	TreeNode *temp = current_folder_pointer;
	i = operand.find(" ",0);
	move_from = operand.substr(0,i);
	while ( operand[i] == ' ' )
		i++;
	move_to = operand.substr(i);
	while ( (i = move_from.find("/",start)) != std::string::npos) 
		start = i+1; 
	file_name = move_from.substr(start);
	move_from.erase(start);
	set_directory (move_from); 
	if ( ispresent (current_folder_pointer -> getleftmostchild() , file_name) == 0 ) {
		current_folder_pointer = temp;
		if (set_directory (move_to)  == 1) {
			if (ispresent(current_folder_pointer->getleftmostchild() , file_name) != 0) {
				add_file (file_name);	//makes a copy of file in destination folder
				current_folder_pointer = temp;
				set_directory(move_from);
				delete_file(file_name); }	//deletes the file from original function
			else
				cout << "\n" << file_name << "already exists in " << current_folder_pointer->get_name();}	} 
	else 
		cout << "\n" << file_name << " doesn't exist at given location";
	current_folder_pointer = temp;
}

void RootedTree :: copy_folder(TreeNode *source , TreeNode *destination) 
{	
	current_folder_pointer = destination;
	if (source->get_isfolder())
		add_folder(source->get_name());
	else
		add_file(source->get_name());
	if (source->getleftmostchild()) {
		set_directory(source->get_name());
		copy_folder(source->getleftmostchild() , current_folder_pointer); }
	if (source->getrightsibling()) 
		copy_folder(source->getrightsibling() , destination);
}

void RootedTree :: deleteSubFolder (TreeNode *p)
{	
	if( p->getleftmostchild() )
		deleteSubFolder (p->getleftmostchild());
	if( p->getrightsibling() )
		deleteSubFolder (p->getrightsibling());
	delete p;
}

int to_be_performed( string oprtn )	//maps every operation to a unique task 
{
	if ( oprtn == "cd" )
		return 1;
	else if ( oprtn == "cwd" )
		return 2;
	else if ( oprtn == "create" )
		return 3;
	else if ( oprtn == "create_file" )
		return 4;
	else if ( oprtn == "delete" )
		return 5;
	else if ( oprtn == "delete_file" )
		return 6;
	else if ( oprtn == "ls" )
		return 7;
	else if ( oprtn == "ls_recurse" )
		return 8;
	else if ( oprtn == "copy" )
		return 9;
	else if ( oprtn == "move" )
		return 10;
	else if ( oprtn == "move_folder" )
		return 11;
	else if ( oprtn == "copy_folder" )
		return 12;
	else if ( oprtn == "quit" )
		return 13;
	else 
		return -1;
}

int main()
{
	int  location , task , start , i;
	RootedTree T;
	string command , operation , operand , copy_to , copy_from , folder_name;
	TreeNode *p , *current ,*previous ,*source;
	bool move;
	
	cout << "\n*****COMMAND LIST*****";
		cout << "\ncd \"path\"  \\\\ change current directory to path";
		cout << "\ncwd \\\\ show the current working directory";
		cout << "\ncreate \"folder_name\" \\\\ create a new folder in cwd";
		cout << "\ncreate_file \"file_name\" \\\\ create a new file in cwd";
		cout << "\ndelete \"folder_name\" \\\\ delete this folder from cwd";
		cout << "\ndelete_file \"file_name\" \\\\ delete this file from cwd";
		cout << "\nls \\\\ list the contents of cwd";
		cout << "\nls_recurse \\\\ recursively list the entire contents of cwd";
		cout << "\nls \"path\" \\\\ list contents of this folder";
		cout << "\nls_recurse \"path\" \\\\ recursively list contents of this folder";
		cout << "\ncopy \"file\" \"destination_folder\" \\\\ copy this file to destination folder";
		cout << "\nmove \"file\" \"destination_folder\" \\\\ move this file to destination folder";
		cout << "\nmove_folder \"source folder\" \"destination folder\" \\\\ move this folder to destination folder";
		cout << "\ncopy_folder \"source folder\" \"destination folder\" \\\\ copy this folder to destination folder";
		cout << "\nquit";
	
	while(1)
	{
		cout << "\nYour command: ";
		getline(cin , command);
		if ( (location = command.find(" ")) != std::string::npos ) {	//operand provided for a operation
			operation = command.substr(0,location);
			while ( command[location] == ' ' )
				location++;
			operand = command.substr(location); }
		else {	//only operation , no operand
			operation = command;
			operand = "\0"; }
		
		task = to_be_performed(operation);
		move = false;
		
		switch(task){
			case 1:
				T.set_directory( operand );
				break;
			case 2:
				T.print_path();
				break;
			case 3:
				if ( ispresent(T.get_current_pointer()->getleftmostchild() , operand) == 1)	//if folder exists
					cout << "\nFolder " << operand << " already exists!";
				else
					T.add_folder( operand ); 
				break;
			case 4:
				if ( ispresent(T.get_current_pointer()->getleftmostchild() , operand)  == 0 )	//if file exists
					cout << "\nFile " << operand << " already exists!";
				else
					T.add_file( operand ); 
				break;
			case 5:
				if ( ispresent(T.get_current_pointer()->getleftmostchild() , operand) == 1) {
					current = T.get_current_pointer()->getleftmostchild();
					previous = NULL;
					while ( current->get_name() != operand ) {
						previous = current;
						current = current -> getrightsibling() ; }
					if ( previous == NULL ) 
						T.get_current_pointer() -> setleftmostchild(current->getrightsibling()); 
					else 
						previous -> setrightsibling( current->getrightsibling() ); 
					current->setparent(NULL);
					current->setrightsibling(NULL);
					T.deleteSubFolder(current);}
				else 
					cout << "\n" << "Folder " << operand << " dosen't exist!";
				break;
			case 6:
				if ( ispresent(T.get_current_pointer()->getleftmostchild() , operand) == 0)
					T.delete_file(operand);
				else 
					cout << "\n" << "File " << operand << " dosen't exist!";
				break;
			case 7:
				if ( operand.length() == 0 ) {
					T.print_folder();}
				else {
					p = T.get_current_pointer();
					if ( T.set_directory(operand) == 1 )
						T.print_folder();
					T.set_current_pointer(p); }
				break;
			case 8:
				if ( operand.length() == 0 ) {
					if ( T.get_current_pointer()->getleftmostchild() )
						T.print_entire_folder(T.get_current_pointer()->getleftmostchild());
					else
						cout << "\nFolder Empty!"; }
				else {
					p = T.get_current_pointer();
						if ( T.set_directory(operand) == 1 ) {
							if ( T.get_current_pointer()->getleftmostchild() )
								T.print_entire_folder(T.get_current_pointer()->getleftmostchild());
							else
								cout << "\nFolder Empty!";	}
					T.set_current_pointer(p); }
				break;
			case 9:
				T.copy_file(operand);
				break;
			case 10:
				T.move_file (operand);
				break;
			case 11:
				move = true;
			case 12:
					p = T.get_current_pointer();
					i = operand.find(" ",0);
					copy_from = operand.substr(0,i);
					while ( operand[i] == ' ' )
						i++;
					copy_to = operand.substr(i);
					start = 0;
					while ( (i = copy_from.find("/",start)) != std::string::npos) 
						start = i+1; 
					folder_name = copy_from.substr(start);
					if (T.set_directory (copy_from) == 1) {
						source = T.get_current_pointer();
						T.set_current_pointer(p);
						if (T.set_directory (copy_to)  == 1) {
							if (ispresent(T.get_current_pointer()->getleftmostchild() , folder_name) != 1) {
								T.add_folder(folder_name);
								T.set_directory(folder_name);
								if ( source->getleftmostchild() )  
									T.copy_folder(source->getleftmostchild() , T.get_current_pointer());
									if (move) {
									current = source->getparent()->getleftmostchild();
									previous = NULL;
									while ( current != source ) {
										previous = current;
										current = current -> getrightsibling() ; }
									if ( previous == NULL ) 
										source->getparent()->setleftmostchild(source->getrightsibling()); 
									else 
										previous -> setrightsibling( current->getrightsibling() ); 
									current->setparent(NULL);
									current->setrightsibling(NULL);
									T.deleteSubFolder(current); } }
							else 
								cout << "\n" << folder_name << " already exists in " << T.get_current_pointer()->get_name();	} }
					T.set_current_pointer(p);	
					break;	 
				case 13:
					return 0;
				default :
					cout << "\n" << operation << "is not a valid command!";}
	}
	return 0;
}
