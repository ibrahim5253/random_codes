#include<iostream>
#include<fstream>
#include<stdlib.h>
#include"BST.h"

using namespace std;

struct linked_list {	//struct for occurance list
	string file_name;
	int line_no;
	linked_list* next;
};

struct line_list {		//struct for nodes of list of line no.s associated with each file in query result
	int line_no;
	line_list *next;
};

struct file_node {		//struct for maintaining nodes in list of matched files
	string file_name;
	int no_of_words;
	line_list *ll;
	file_node *next;
};

class file_list			//maintains the list of matched files
{
	private:
		file_node *start;	//pointer to the beginning of the list
	public:
		file_list () {
			start = NULL; }
		void Insert ( string fname , int lno );
		file_node* search (string fname);
		void sortdesc();
		void print();
}; 

void file_list :: Insert ( string fname , int lno )
{
	file_node* ptr;
	if ( ptr = search (fname) ) {	//file name already in the list
		ptr -> no_of_words++;
		line_list *p = new line_list;
		p->line_no = lno;
		p->next = NULL;
		line_list *current = ptr->ll;
		line_list *previous = NULL;
		while ( lno > current->line_no ) {
			previous = current;
			if ( current -> next )
				current = current -> next;
			else 
				break; }
		if ( current->line_no == lno ) {
			delete p; 
			return ; }
		if ( current == previous ) {
			current -> next = p;
			return ;}
		if ( previous == NULL ) {
			p->next = ptr->ll;
			ptr->ll = p; 
			return ;}
		previous->next = p;
		p->next = current; }
	else {	//file not in the list , creates a new file node
		ptr = new file_node;
		ptr -> file_name = fname;
		ptr -> no_of_words = 1;
		ptr -> ll = new line_list;
		ptr -> ll -> line_no = lno;
		ptr -> ll -> next = NULL;
		if (!start)
			start = ptr;
		else {
			ptr->next = start;
			start = ptr;}}	
}

file_node* file_list :: search ( string fname )
{
	file_node *current = start;
	while ( current ) {
		if ( current -> file_name == fname )
			return current;
		current = current -> next; }
	return NULL;
}

void file_list :: sortdesc()	//sorts matched file wrt no of words found in desc order
{
	if (!start)
		return;
	file_node *current = start , *temp , *max;
	file_node* p;
	while ( current->next ) {
		p = current->next;
		max = current;
		while ( p ) {
			if (p->no_of_words > max -> no_of_words)
				max = p; 
			p = p->next;}
		temp = new file_node;
		temp->file_name = current->file_name;
		temp->ll = current->ll;
		temp->no_of_words = current->no_of_words;
		current->file_name = max->file_name;
		current->ll = max->ll;
		current->no_of_words = max->no_of_words;
		max->file_name = temp->file_name;
		max->ll = temp->ll;
		max->no_of_words = temp->no_of_words;
		delete temp;
		current = current->next;}
}

void file_list :: print()	//prints matched file list with corresponding line numbers
{
	if (!start) {
		cout << "\nQuery not found";
		return; }
	file_node *current = start;
	line_list *p;
	while (current) {
		cout << "\n" << current->file_name;
		p = current->ll;
		while (p) {
			cout << " " << p->line_no;
			p = p->next;}
		current = current -> next; }
}

class TermRec
{
	friend ostream& operator<<( ostream&, TermRec& );	// '<<' operator overloaded to print TermRec objects
	private:
		string Word;
		linked_list *occ_list;
	public:
		TermRec () {
			occ_list = NULL;}
		TermRec (string x) {
			occ_list = NULL;
			Word = x;}
		string get_word();
		void Insert(string fname , int l_no);
		linked_list* get_occlist();
		bool operator==( TermRec& );
		bool operator<( TermRec& );
		bool operator>( TermRec& );
};

string TermRec :: get_word()
{
	return Word;
}

void TermRec :: Insert(string fname , int l_no)
{
	linked_list* ptr = new linked_list;
	ptr->file_name = fname;
	ptr->line_no = l_no;
	ptr->next = NULL;
	if (occ_list == NULL)
		occ_list = ptr;
	else {
		linked_list *current = occ_list;
		while (current->next)
			current = current -> next;
		current->next = ptr;}
}

linked_list* TermRec :: get_occlist()
{
	return occ_list;
}

bool TermRec :: operator==( TermRec &B )	// '==' operator overloaded to compare two TermRec objects wrt their 'Word' lexicographically
{
	if (this->Word == B.Word)
		return true;
	else 
		return false;
}

bool TermRec :: operator<( TermRec &B )
{
	if (this->Word < B.Word)
		return true;
	else 
		return false;
}

bool TermRec :: operator>( TermRec &B )
{
	if (this->Word > B.Word)
		return true;
	else 
		return false;
}

ostream& operator<<( ostream& output, TermRec& B)
{
	output << B.Word;
	return output;
}

int main( int argc , char* argv[])
{
	ifstream fp(argv[1]);
	if (!fp.is_open()) {
		cout << "Error opening " << argv[1];
		return -1; }
	string word_info , word , fname , query , unit;
	int lno , ch , i , start , k;
	TermRec *obj , *ptr;
	linked_list *p;
	file_list *matched_file_list;
	BST<TermRec> T;
	while (getline(fp , word_info)) {	//reconstructing BST from file
		word = word_info.substr(0 ,start = word_info.find(" ",0));
		obj = new TermRec(word);
		start++;
		while ( ( i = word_info.find(" ", start) )  != std::string::npos ) {
			unit = word_info.substr(start,i-start);
			fname = unit.substr(0, k = unit.find("-",0));
			lno = atoi(unit.substr(k+1).c_str());
			start = i+1;
			obj->Insert ( fname , lno );}
		T.Insert(obj);
		delete obj; }
	while (1) {	//search module
		cout << "\nSearch query: ";
		getline (cin , query);
		query.append(" ");
		matched_file_list = new file_list;
		start = 0;
		while ( (i = query.find(" " , start)) != std::string::npos) {
			word = query.substr(start , i-start);
			for (int j=0; j<word.length(); j++)
				word[j] = tolower(word[j]);
			obj = new TermRec (word);
			if (ptr = T.Search(obj)) {	//if word exists in the BST (searched through creating an object containing the word)
				p = ptr -> get_occlist();
				while (p) {	//scans the occurance list of matched word
					matched_file_list->Insert ( p->file_name , p->line_no );	//occurances stored in matched file list
					p = p->next;} } 
			delete obj; 
			start = i+1;}
		matched_file_list->sortdesc();	//matched file list sorted
		matched_file_list->print();
		delete matched_file_list;
		cout << "\nMore queries(1/0)?";
		cin >> ch;
		cin.ignore();
		if (!ch)
			break; }				
	return 0;
}
