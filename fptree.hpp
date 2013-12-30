#ifndef FPTREE_HPP
#define FPTREE_HPP
  
using namespace std; 
  
class Tree 
{
	private:
	    struct Node 
		{ 
			int item; 
			int sup; 
			vector<Node*> child; 
			Node *parent;
		};
		Node *root;
		void projTableRecursion(int val, int *count, vector<vector<int> > *projTable, map<int,int> *projHeaderTable, Node *node);
    public: 
        Tree(); 
        void insert(vector<int> transaction); 
        void projTable(int val, int count, vector<vector<int> > *projTable, map<int,int> *projHeaderTable); 
        void printTree();            
}; 
  
#endif