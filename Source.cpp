#include<iostream>
#include<string>
#include"Stack.cpp"
#include"List.cpp"
#include<fstream>
using namespace std;

//-------------------------------Class ANode-------------------------------------
template<class T>
class ANode {
	int height;
	T* data; // T would be termInfo or list of docInfo
	ANode<T>* lChild, * rChild;
public:
	template<class T>friend class AVLTree;
	ANode() {
		lChild = 0;
		rChild = 0;
		height = 0;
	}
	ANode(T& e, ANode<T>* l = 0, ANode<T>* r = 0) {
		data = new T(e);
		lChild = l;
		rChild = r;
		height = 0;
	}
};


//--------------------------------Class AVLTree----------------------------------
template<class T>
class AVLTree {
	ANode<T>* Root;

	//-----------AVLIterator Class as nested Class of AVLTree-----------
	class AVLIterator {
	public:
		template<class T>friend class AVLTree;
		AVLIterator(ANode<T>* t = NULL) {
			inorder(t);
			(*this)++;
		};

		void inorder(ANode<T>*& node) {                             //In Constructor of AVLiterator inorder
			if (node) {											    //function is called which works recursively
				inorder(node->rChild);								//and pushes all the nodes of the tree to stack in 
				stack.push(node);									//descending order
				inorder(node->lChild);
			}
		}

		AVLIterator& operator++(int) {
			AVLIterator old = *this;
			++(*this);
			return old;
		}

		AVLIterator& operator++() {                                        //Here if stack is not empty then 
			if (!stack.isEmpty()) {										   //top value of stack is assigned
				iptr = stack.top();									 	   //to iptr and then popped out from the 
				stack.pop();											   //stack
			}
			else {
				iptr = NULL;
			}
			return (*this);
		}

		T& operator*() {                                                    //All of the other operators of
			return *(iptr->data);                                           //AVLIterator are same as of List 
		}																	//Class iterator except the one
																			//which is prefix(++) operator
		bool operator==(const AVLIterator& l) const
		{
			return iptr == l.iptr;
		}

		bool operator!=(const AVLIterator& l) const {
			return !(iptr == l.iptr);
		}
	private:
		Stack<ANode<T>*> stack;
		ANode<T>* iptr;
	};
public:
	typedef AVLIterator Iterator;
	Iterator begin() {
		Iterator I(Root);
		return I;
	}
	Iterator end() {
		Iterator I(NULL);
		return I;
	}

	//------------Default Constructor--------
	AVLTree() {
		Root = NULL;
	}

	//------------Copy Constructor-----------
	AVLTree(const AVLTree& rhs) : Root{ nullptr }
	{
		Root = clone(rhs.Root);
	}

	ANode<T>* clone(ANode<T>* t) const
	{
		if (t == nullptr)
			return nullptr;
		else
			return new ANode<T>{ t->data, clone(t->lChild), clone(t->rChild) };
	}

	//------Function to Insert Node-------
	void insert(ANode<T>*& t, T& term_info) {
		if (t == nullptr) {
			t = new ANode<T>(term_info, nullptr, nullptr);
		}
		else if (term_info < *(t->data)) {
			insert(t->lChild, term_info);
		}
		else if (*(t->data) < term_info) {
			insert(t->rChild, term_info);
		}
		balance(t);
	}

	//------wrapper Function-------
	void insert(T& term_info) {
		insert(Root, term_info);
	}


	//------Function to Balance Tree------
	void balance(ANode<T>*& t)
	{
		if (t == nullptr)
			return;

		if (height(t->rChild) - height(t->lChild) < -1) {
			if (height(t->lChild->lChild) >= height(t->lChild->rChild)) {
				rightRotate(t);
			}
			else {
				doubleLeftRightRotation(t);
			}
		}

		else if (height(t->rChild) - height(t->lChild) > 1) {
			if (height(t->rChild->rChild) >= height(t->rChild->lChild)) {
				leftRotate(t);
			}
			else {
				doubleRightLeftRotation(t);
			}
		}
		t->height = max(height(t->lChild), height(t->rChild)) + 1;
	}

	int max(int a, int b)
	{
		return (a > b) ? a : b;
	}

	int height(ANode<T>* t) const
	{
		return t == nullptr ? -1 : t->height;
	}

	//------Function to Rotate Right------
	void rightRotate(ANode<T>*& x) {
		ANode<T>* orphan = x->lChild->rChild;
		ANode<T>* y = x->lChild;

		y->rChild = x;
		x->lChild = orphan;

		x->height = height(x); //update nodes'
		y->height = height(y);
		x = y;
	}

	//------Function to Rotate Left-------
	void leftRotate(ANode<T>*& x) {
		ANode<T>* orphan = x->rChild->lChild;
		ANode<T>* y = x->rChild;

		y->lChild = x;
		x->rChild = orphan;

		x->height = height(x); // update nodes'
		y->height = height(y);
		x = y;
	}

	//------Function to Rotate LeftRight------
	void doubleLeftRightRotation(ANode<T>*& x) {
		leftRotate(x->lChild);
		rightRotate(x);
	}

	//------Function to Rotate RightLeft------
	void doubleRightLeftRotation(ANode<T>*& x) {
		rightRotate(x->rChild);
		leftRotate(x);
	}

	//-------Function to Find Minimum Node-------
	ANode<T>* findMin(ANode<T>*& t) {
		if (t == nullptr)
			return nullptr;
		if (t->lChild == nullptr)
			return t;
		return findMin(t->lChild);
	}

	//------Function to delete Node-------
	void deletedata(ANode<T>*& t, T& x) {

		if (t == nullptr) {
			return; // Item not found; do nothing
		}

		if (x < *(t->data)) {
			deletedata(t->lChild, x);
		}
		else if (*(t->data) < x) {
			deletedata(t->rChild, x);
		}
		else if (t->lChild != nullptr && t->rChild != nullptr) {//Two children
			t->data = findMin(t->rChild)->data;
			deletedata(t->rChild, *(t->data));
		}
		else {
			ANode<T>* oldNode = t;
			(t->lChild != nullptr) ? t = t->lChild : t = t->rChild;
			delete oldNode;
		}

		balance(t); // same as insertion
	}

	//-------Wrapper Function------
	void deletedata(T data) {
		deletedata(Root, data);
	}

	//---------Function Called in Destructor----------
	void Destroy(ANode<T>*& node) {
		if (node) {
			Destroy(node->lChild);
			Destroy(node->rChild);
			delete node;
		}
	}

	//----------Destructor------------
	~AVLTree() {
		Destroy(Root);
	}

};

//------------------------------------Doc_Info Class------------------------------------

class Term_Info;
class Search_Engine;
class Doc_Info {
	int Doc_iD;
	int term_f;
public:
	friend class Search_Engine;
	friend class Term_Info;
	Doc_Info() {
		Doc_iD = 0;
		term_f = 0;
	}
	Doc_Info(Doc_Info const& obj) {
		Doc_iD = obj.Doc_iD;
		term_f = obj.term_f;
	}
	Doc_Info(int Id, int freq) {
		Doc_iD = Id;
		term_f = freq;
	}

};

//------------------------------------Term_Info Class-----------------------------------

class Search_Engine;
class Term_Info {
	string key;
	List<Doc_Info> doc;
public:
	friend class Search_Engine;

	Term_Info() {
		key = "\0";
	}

	Term_Info(string k) {
		key = k;
	}

	bool operator<(const Term_Info& obj) {
		return key < obj.key;
	}

	bool operator>(const Term_Info& obj) {
		return key > obj.key;
	}

	friend ostream& operator<<(ostream& out, const Term_Info& obj) {
		out << obj.key << " ";
		return out;
	}
};

//---------------------Global Template Function to Grow the array-----------------------

template <class type>
void Grow(type*& array, int& size)
{
	type* temp = new type[size + 1];

	for (int i = 0; i < size; i++)
	{
		temp[i] = array[i];
	}
	size++;
	delete[]array;
	array = temp;
}



//------------------------------------Search_Engine Class-------------------------------

class Search_Engine {
	AVLTree<Term_Info>* Index;
public:
	Search_Engine() {
		Index = new AVLTree<Term_Info>;
	}

	//</----------------Algorithm-1---------------\> 
	void Create_Index(string* fileName, int Dsize) {
		for (int i = 0; i < Dsize; i++) {									//For each document Add_Doc_to_index 
			Add_Doc_to_Index(fileName[i]);                                  //Function is called
		}
	}

	//----Function to Calculate Term Frequency-----
	int termfreq(string key, string fname) {
		int size = 0;
		int ind = -1;
		int count = 0;
		string* word = new string[size];
		ifstream fin(fname.c_str());
		while (!fin.eof()) {
			Grow(word, size);
			ind++;
			fin >> word[ind];
		}
		fin.close();
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < word[i].length(); j++)
				word[i][j] = tolower(word[i][j]);
		}
		for (int i = 0; i < size; i++) {
			if (key == word[i]) {
				count++;
			}
		}
		return count;
	}

	//---------Functions to Remove Duplicate Words--------

	void removeSpecificWord(string* arr, int idx, int& size)
	{
		int i;
		for (i = idx; i < size - 1; i++)
			arr[i] = arr[i + 1];
		size--;
	}

	void removeDuplicateWord(string* arr, int& size)
	{
		int i, j;
		string target;
		for (i = 0; i < size; i++)
		{
			arr[i];
			target = arr[i];
			for (j = i + 1; j < size; j++)
			{
				if (target == arr[j]) {
					removeSpecificWord(arr, j, size);
					arr[i];
					j--;
				}
			}
		}
	}


	void Add_Doc_to_Index(string fName) {
		ifstream fin(fName.c_str());
		int size = 0;
		int index = -1;
		string* arr = new string[size];
		while (!fin.eof()) {                                        //Adding words of each document in array
			string temp;											//then converting all the words to lower
			fin >> temp;											//case and then removing duplicates
			if (temp != " " && temp != "") {						//from array
				Grow(arr, size);
				index++;
				arr[index] = temp;
			}
		}
		fin.close();
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < arr[i].length(); j++)
				arr[i][j] = tolower(arr[i][j]);
		}
		removeDuplicateWord(arr, size);

		//***Using AVL Tree Iterators instead of List Iterators***
		for (AVLTree<Term_Info>::Iterator itr = Index->begin(); itr != Index->end(); itr++)
		{
			for (int i = 0; i < size; i++)
			{
				if (arr[i] == (*itr).key)                                //If term is found then it inserts
				{                                                        //the doc_info object into the doc
					Doc_Info D(fName[3] - 48, termfreq(arr[i], fName));  //list in that term and then deleting 
					(*itr).doc.insertAtEnd(D);                           //that term from array

					shrink(arr, i, size);
					break;
				}
			}
		}
		if (size != 0)
		{
			for (int i = 0; i < size; i++)                              //if any term left in array it
			{                                                           //adds that term at the left or right
				Term_Info T(arr[i]);                                    //of Index Tree and initialize the
				Doc_Info doc(fName[3] - 48, termfreq(arr[i], fName));   //Doc_Info List in that term
				T.doc.insertAtEnd(doc);
				Index->insert(T);
			}
		}
		delete[]arr;
	}
	void shrink(string*& arr, int index, int& size) {
		for (int i = index; i + 1 < size; i++) {
			arr[i] = arr[i + 1];
		}
		size--;
	}


	//</-----------------Algorithm-2---------------\>
	void Search_Document(string* query, int Qsize) {
		int isize = 0;
		Doc_Info* iarr = new Doc_Info[isize];
		int Iindex = -1;
		for (int i = 0; i < Qsize; i++)
		{
			for (int j = 0; j < query[i].length(); j++)
				query[i][j] = tolower(query[i][j]);
		}

		//*using AVLTree Iterator*
		for (AVLTree<Term_Info>::Iterator itr = Index->begin(); itr != Index->end(); itr++)
		{																		            //Iterating in the Tree
																							//if the key is matched 
			for (int i = 0; i < Qsize; i++)                                                 //with query then it  
			{																		        //adds them in array
				if (query[i] == (*itr).key)
				{
					List<Doc_Info>::Iterator it = (*itr).doc.begin();
					if (it == (*itr).doc.end()) {                                   //If there is only one
						Grow(iarr, isize);										    //Node in Doc_List then 
						Iindex++;													//it inserts it in array
						iarr[Iindex] = (*it);
					}
					else {
						for (it = (*itr).doc.begin(); it != NULL; it++) {           //else Inserting the whole 
							Grow(iarr, isize);										//doc_Info List from the 
							Iindex++;                                               //term_info into the array
							iarr[Iindex] = (*it);
						}
					}
				}
			}
		}

		if (isize == 0) {                                                          //Checking if no query 
			cout << "Not Found...:(\n";                                            //word matched to key terms
		}                                                                          //then displays some msg

		//Rule-1
		int fsize = 0;
		int* farr = new int[fsize];
		int ind = -1;
		for (int i = 0; i < isize; i++) {
			int count = 1;
			for (int j = i + 1; j < isize; j++) {
				if (iarr[i].Doc_iD == iarr[j].Doc_iD) {
					count++;                                                       //Removing duplicate words 
					iarr[i].term_f = iarr[i].term_f + iarr[j].term_f;              //from the array and adding
					removeD(iarr, j, isize);									   //their corresponding term
					j--;														   //frequency(to rank them later)
				}
			}
			Grow(farr, fsize);
			ind++;
			farr[ind] = count;													   //Creating a parallel array
		}																		   //for storing query terms frequency
		for (int i = 0; i < fsize - 1; i++) {
			for (int j = 0; j < fsize - i - 1; j++) {
				if (farr[j] < farr[j + 1]) {
					swap(farr[j], farr[j + 1]);									   //Bubble sorting the arrays in 
					swap(iarr[j], iarr[j + 1]);									   //descending order w.r.t to query
				}																   //terms
			}
		}

		//Rule-2
		Doc_Info key;
		int k;
		for (int a = 0; a < fsize;) {
			int tcount = 0;
			for (int i = a; i < fsize; i++) {                                     //Calculating the number of Doc_Info 
																				  //having same query term frequency
				if (farr[i] == farr[a]) {
					tcount++;
				}
			}
			for (int j = a + 1; j < tcount + a; j++)
			{
				key = iarr[j];
				for (k = j - 1; (k >= 0) && (iarr[k].term_f < key.term_f); k--)		//Insertion sorting according to 	
				{																    //the term frequency in descending  
					iarr[k + 1] = iarr[k];                                          //order
				}
				iarr[k + 1] = key;
			}
			a += tcount;
		}

		//Rule-3
		for (int s = 0; s < fsize;) {
			int fcount = 0;
			for (int m = s; m < fsize; m++) {								  //Calculating the number of Doc_Info 
				if (iarr[m].term_f == iarr[s].term_f) {					      //having same term frequency
					fcount++;
				}
			}
			for (int j = s + 1; j < fcount + s; j++)
			{
				key = iarr[j];
				for (k = j - 1; (k >= 0) && (iarr[k].Doc_iD > key.Doc_iD) && (iarr[k].term_f == key.term_f); k--)    //Insertion sorting according to
				{																									 //the Doc_id and term_f in ascending order															 	
					iarr[k + 1] = iarr[k];
				}
				iarr[k + 1] = key;
			}
			s += fcount;
		}
		for (int i = 0; i < isize; i++) {                                           //Displaying the result for 
			cout << "Doc " << iarr[i].Doc_iD << endl;                               //each query term
		}
	}
	void removeD(Doc_Info*& arr, int j, int& isize) {
		for (int i = j; i < isize; i++) {
			arr[i] = arr[i + 1];
		}
		isize--;
	}

	//---Destructor---
	~Search_Engine()
	{
		if (Index) {
			delete Index;
		}
		Index = NULL;
	}
};


//------------Function to tokenize Query------------------
void Querytokenize(string query, string*& Qarr, int& Qsize) {
	ofstream fout;
	fout.open("temp.txt");
	fout << query;
	fout.close();
	int Qindex = -1;                                            //Writing query string to file 
	ifstream fin;											    //then making the array of each  
	fin.open("temp.txt");										//word and then deleting the file
	while (!fin.eof()) {                                        //and then converting each word to  
		Grow(Qarr, Qsize);										//lowercase
		Qindex++;
		fin >> Qarr[Qindex];
	}
	fin.close();
	remove("temp.txt");
}


//----------------------------------Main-----------------------------------
int main() {
	Search_Engine S;
	int size = 4;
	string Docs[4] = { "Doc1.txt", "Doc2.txt", "Doc3.txt", "Doc4.txt" };
	S.Create_Index(Docs, size);
	S.Add_Doc_to_Index("Doc5.txt");
	string query;
	cout << "Enter Search Query: ";
	getline(cin, query);
	int Qsize = 0;
	string* Qarr = new string[Qsize];
	Querytokenize(query, Qarr, Qsize);
	S.Search_Document(Qarr, Qsize);
	return 0;
}