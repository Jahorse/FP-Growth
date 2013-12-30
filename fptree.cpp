#include <iostream> 
#include <fstream> 
#include <string> 
#include <vector> 
#include <cstdlib> 
#include <map> 
#include <algorithm> 

#include "fptree.hpp"

using namespace std; 
  
Tree::Tree() 
{ 
	root = new Node; 
	root->item = 0; 
	root->sup = 0; 
	root->parent = NULL;
} 
  
void Tree::insert(vector<int> transaction)
{    
	Node *current_node = root; 
	bool added = false;
	bool createNewBranch = false;
	  
	if(root->child.empty())  
	{     
		//add each element of the transaction  
		for(int i = 0; i < transaction.size(); i++)  
		{  
			Node *new_node = new Node;  
			new_node->item = transaction.at(i);  
			new_node->sup = 1;
			new_node->parent = current_node;  
				
			current_node->child.push_back(new_node);  
			current_node = current_node->child.at(0);  
		}  
	} 
	else
	{ 
		for(int i = 0; i < transaction.size(); i++) 
		{ 
			if(!createNewBranch) 
			{ 
				for(int j = 0; j < current_node->child.size(); j++) 
				{ 
					if(transaction.at(i) == current_node->child.at(j)->item) 
					{ 
						current_node->child.at(j)->sup++; 
						current_node = current_node->child.at(j); 
						added = true; 
						break; 
					} 
				}
				if (!added)
				{
					Node *new_node = new Node;  
					new_node->item = transaction.at(i);  
					new_node->sup = 1; 
					new_node->parent = current_node;  
					current_node->child.push_back(new_node);  
					current_node = new_node;
					createNewBranch = true;
				}
				else
				{
					added = false;
				}
			} 
			else
			{ 
				Node *new_node = new Node;  
				new_node->item = transaction.at(i);  
				new_node->sup = 1; 
				new_node->parent = current_node;  
				current_node->child.push_back(new_node);  
				current_node = new_node;
			} 
		} 
	} 
} 

void Tree::projTable(int val, int count, vector<vector<int> > *projTable, map<int,int> *projHeaderTable)
{
	for (int i = 0; i < root->child.size(); i++)
	{
		projTableRecursion(val, &count, projTable, projHeaderTable, root->child.at(i));
	}
}

void Tree::printTree() 
{ 
//	Node *current_node = root; 
//	Node *child = current_node->child.at(0); 
 
//	while (current_node != NULL) 
//	{ 
//		for (int i = 0; i < current_node->child.size(); i++) { 
//			current_node = current_node->child.at(i).printBranch(); 
//		}
//	}
}

void Tree::projTableRecursion(int val, int *count, vector<vector<int> > *projTable, map<int,int> *projHeaderTable, Node *node)
{
	vector<int> itemSet;
	Node *currentNode = node;

	if(node->item == val)
	{
		currentNode = currentNode->parent;
		while(currentNode != root)
		{
			itemSet.push_back(currentNode->item);
			(*projHeaderTable)[currentNode->item] += node->sup;
			currentNode = currentNode->parent;
		}
		for (int i = 0; i < node->sup; i++)
		{
			(*projTable).push_back(itemSet);
			(*count)--;
		}
	}
	else
	{
		if (*count > 0)
		{
			for (int i = 0; i < node->child.size(); i++)
			{
				projTableRecursion(val, count, projTable, projHeaderTable, node->child.at(i));
			}
		}
	}
}


//int main() 
//{ 
//    Tree fpTree;  
//    vector<int> trans1;  
//    vector<int> trans2; 
//	vector<vector<int> > projTable;
//	map<int,int> projHeaderTable;
//        
//    trans1.push_back(1);
//    trans1.push_back(2);
//    trans1.push_back(3);
//  
//    trans2.push_back(1);
//    trans2.push_back(3);
//    trans2.push_back(4);
//        
//    fpTree.insert(trans1);
//    fpTree.insert(trans2);
//
//	fpTree.projTable(3, 2, &projTable, &projHeaderTable);
//
//    fpTree.printTree();  
//}