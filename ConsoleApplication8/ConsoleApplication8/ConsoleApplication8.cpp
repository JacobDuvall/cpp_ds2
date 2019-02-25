#include <string>
#include <list>
#include <iostream>

using namespace std;

class ttreeException : public exception {}; // ttreeException 
class ttreeNotFound : public ttreeException {}; // ttreeNotFound 

class ttree; // Lets tnode know ttree exists

			 // tnode stores lists of words within a ttree
class tnode {
	ttree* _nextLevel; // pointer to ttree at nextLevel
	list<string>* _words; // list of words stored
public:
	tnode(); // empty tnode constructor
	~tnode(); // tnode destructor
	list<string>* getWords(); // returns words
	ttree* getNext(); // returns nextLevel
	list<string>* searchReturn(string key, int depth); // list to store all outputs from search method, moved all of search into this function 
	friend ostream& operator<<(ostream& os, const tnode& n); // ostream  to display tnode
	void insert(string key, int depth, int maxDepth); // inserts words into node alphabetically
	void search(string key, int depth, int maxDepth); // searches for words to see if they are within tree
};

// a tree that will use tnodes to store words
class ttree {
	friend class tnode; // tnode has access to ttree 
	tnode* _tnodes; // pointer to tnodes
	int _maxDepth; // max height of tree
	int _currentDepth; // current depth of tree
	void setDepth(int depth); // sets current depth
public:
	ttree(); // empty ttree constructor
	ttree(int maxDepth); // maxDepth ttree constructor
	~ttree(); // ttree destructor
	void insert(string key); // word to insert into tree
	void search(string key, int treedepth, int treemaxdepth); // word to search for in tree
	friend ostream& operator<<(ostream& os, const ttree& t); // ostream to display ttree
};

using namespace std;

// empty tnode constructor
tnode::tnode() {
	_nextLevel = NULL; // _nextLevel is set to NULL
	_words = new list<string>(); // _word is a new list of strings
}

// returns list of words
list<string>* tnode::getWords() {
	return _words;
}

//returns _nextLevel
ttree* tnode::getNext() {
	return _nextLevel;
}

// tnode destructor
tnode::~tnode() {
	delete _nextLevel;
	delete _words;
}

// ostream for tnode
ostream& operator<<(ostream& os, const tnode& n) {
	return os;
}

// list to store all outputs from search method, moved all of search into this function 
list<string>* tnode::searchReturn(string key, int depth)	{
	list<string>* retList = new list<string>(); // list that will be returned 
	if (_nextLevel != NULL) { // if nextLevel exists 
		if (depth < key.length()) { // checked to make sure we don't go outside of key range 
			list<string>* tempList = _nextLevel->_tnodes[key[depth] - 'A'].searchReturn(key, depth + 1); // recursively stores value into tempList
			retList->insert(retList->end(), tempList->begin(), tempList->end()); // retList insert takes iterator position, input first, input last 
		}
		else { // if nextLevel does not exist
			for (int i = 0; i < 26; i++) { // A - Z
				if (_nextLevel->_tnodes[i]._nextLevel != NULL || !_nextLevel->_tnodes[i]._words->empty()) { // if nextLevel isn't empty OR there are words to search
					list<string>* tempList = _nextLevel->_tnodes[i].searchReturn(key, depth + 1); // recursively stores value into tempList
					retList->insert(retList->end(), tempList->begin(), tempList->end()); // retList insert takes iterator position, input first, input last 
				}
			}
		}
	}
	else if (!_words->empty()) { // if words is not empty
		list<string>::iterator it;
		for (it = _words->begin(); it != _words->end(); it++) { // iterator loop through _words list
			int len = key.length(); // len is key length
			if ((*it).length() >= key.length() && (*it).compare(0, len, key, 0, len) == 0) // if word length is greater or equal to key length AND comparison is equal
			{
				retList->push_back(*it); // push word to back of retList
			}
		}
	}
	return retList; 
}

// insert a string into the tree, adding levels if necessary
void tnode::insert(string key, int depth, int maxDepth) {
	if (_nextLevel && (int)key.size() > depth) { // level after current and size of string key are checked
		(*_nextLevel).insert(key); // insert at nextLevel
		return;
	}
	char next = key[depth]; // next is a char representing the current depth
	list<string>::iterator it;
	for (it = _words->begin(); it != _words->end(); it++) { // iterators goes through list of _words
		string& ot = *it; // ot is current word iterator
		if (ot == key) { // if the current word is already in the list, do nothing
			return;
		}
		if (ot[depth] == next && depth < maxDepth) { // if the current letter depth of ot and depth is less than maxDepth
			_nextLevel = new ttree(maxDepth); // create new ttree 
			_nextLevel->setDepth(depth + 1); // new ttree's current depth is incremented
			it = _words->begin(); // it is the first word in the list
			if ((int)it->size() == depth) { // if size of it is equal to current depth, increment it
				it++;
			}
			while (it != _words->end()) { // loop through list of words
				list<string>::iterator it2 = it; // it2 is it
				it2++; // increment it2
				_nextLevel->_tnodes[(*it)[depth] - 'A']._words->push_back(*it);
				_words->erase(it); // erase it from words list
				it = it2; // set it to it2
			}
			_nextLevel->insert(key); // insert at nextLevel

			return;
		}
		if (ot > key) { // if current word is greater than insert word, break
			break;
		}
	}
	_words->insert(it, key); // 
}

// search tnode for specific word
void tnode::search(string key, int depth, int maxDepth) {
	list<string>* ls = searchReturn(key, depth); // ls is the returned list from searchReturn
	if (ls->empty()) { // if ls has no values, i.e. the word is not found
		throw ttreeNotFound();
	}
	else {
		list<string>::iterator it; 
		for (it = ls->begin(); it != ls->end(); it++) { // iterate through ls list and print out the values
			cout << *it << " ";
		}
		cout << endl;
	}
}

void ttree::setDepth(int depth) {
	_currentDepth = depth; // sets _currentDepth to depth
}

// ttree empty constructor
ttree::ttree() : ttree(5) { // Creates a ttree of maxDepth 5

}

// ttree maxDepth constructor
ttree::ttree(int maxDepth) {
	_tnodes = new tnode[26]; // array of 26 tnodes
	_maxDepth = maxDepth; // _maxDepth is set to maxDepth
	_currentDepth = 1; // currentDepth set to 1
}

// ttree destructor
ttree::~ttree() {
	delete[] _tnodes; // delete array of _tnodes
}

// insert string key into ttree
void ttree::insert(string key) {
	_tnodes[key[_currentDepth - 1] - 'A'].insert(key, _currentDepth, _maxDepth); // insert into tnode, key reduced by A to get ASCII value
}

// search tnode for word
void ttree::search(string key, int treedepth, int treemaxdepth) {
	_tnodes[key[_currentDepth - 1] - 'A'].search(key, _currentDepth, _maxDepth); // search tnode
}

// ostream for ttree
ostream& operator<<(ostream& os, const ttree& t) {
	for (int i = 0; i < 26; i++) {
		if (!t._tnodes[i].getWords()->empty()) { // if the list is not empty
			for (int j = 0; j < 3 * (t._currentDepth - 1); j++) {
				os << ' '; // print three extra spaces before each new level
			}
			os << (char)('A' + i); // output letter value
								   //for (int k = 0; k < (t._tnodes)[i].getWords()->size(); k++) {
			list<string>::iterator it;
			for (it = t._tnodes[i].getWords()->begin(); it != t._tnodes[i].getWords()->end(); it++) {
				os << " " << *it; // print each string in words
			}

			os << endl;
		}
		else if (t._tnodes[i].getNext() != NULL) {
			for (int j = 0; j < 3 * (t._currentDepth - 1); j++) {
				os << ' '; // print three extra spaces before each new level
			}
			os << (char)('A' + i) << endl;
			os << *(t._tnodes)[i].getNext(); //recursive getNext to go all the way down the tree
		}
	}
	return os;
}

int main()
{

	char submission; // characters collected from the user
	string input = ""; // string collected from the user
	ttree* dict = new ttree();

	while (cin >> submission) {
		switch (submission) { // I, S, or D

		case 'I': // Insert word
		{
			cin >> input; 
			dict->insert(input);

			break;
		}

		case 'S': // Search words
		{
			try {
				cin >> input;
				dict->search(input, 0, 5);
			}
			catch (ttreeNotFound e) {
				cout << "not found" << endl;
			}
			break;
		}

		case 'D': // Display 
		{
			cout << (*dict) << endl;

			break;
		}

		default: cout << "Error: Input not recognized.\n"; // outputs if input is not I, S, or D
		}
	}

	return 0;
}