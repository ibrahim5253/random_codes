#include<iostream>
#include"BST.h"
#include<fstream>

using namespace std;

struct linked_list {
	string file_name;
	int line_no;
	linked_list* next;
};

class TermRec
{
	friend ostream& operator<<( ostream&, TermRec& );
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

bool TermRec :: operator==( TermRec &B )
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

void write_to_file ( TreeNode<TermRec> *p , ofstream &fp )		//recursive function to write the content of BST to the file (preorder)
{	
	if (!p)
		return;
	fp << p->get_data()->get_word() << " ";
	linked_list *current = p->get_data()->get_occlist();
	while (current) {
		fp << current->file_name << "-" << current->line_no << " ";	//inserted as 'filename-line', each pair separated by ' '
		current = current -> next; }
	fp << endl;
	write_to_file ( p->get_lchild() , fp );
	write_to_file ( p->get_rchild() , fp );
}

int main(int argc , char* argv[])
{
	BST<TermRec> T;
	ifstream F("flist.txt") , file ;
	ofstream fp(argv[1]);
	
	if (!F.is_open()) {
		cout << "\nError opening file flist.txt , Aborting!!";
		return -1;}
	string file_to_be_indexed , line , word , file_name;
	int line_no , i , start;
	TermRec *Node , *word_obj;
	
	while (getline( F , file_to_be_indexed )) {
		file.open(file_to_be_indexed.c_str());
		if (file.is_open()) {
			start = 0;
			while ( (i = file_to_be_indexed.find("/",start)) != std::string::npos )		//finding file name
				start = i+1;
			file_name = file_to_be_indexed.substr(start);
			line_no = 1;
			while (getline(file , line)) {
				start = 0;
				line.append(" ");
				while ( (i = line.find(" ",start)) != std::string::npos) {
					word = line.substr(start , i-start);
					for ( int j=0 ; j < word.length() ; j++) 
						word[j] = tolower(word[j]);
					word_obj = new TermRec( word );
					Node = T.Insert(word_obj);
					Node->Insert(file_name , line_no);
					delete word_obj;
					start = i+1;
					while (line[start] == ' ')		//to accomodate more than one space between words
						start++;} 
				line_no++;} 	
				file.close();}
		else 
			cout << "\nError opening file " << file_to_be_indexed; }
	write_to_file(T.get_root() , fp);
	return 0;
}
