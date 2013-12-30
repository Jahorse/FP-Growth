#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <map>
#include <algorithm>

#include "fptree.hpp"

using namespace std;

// debug
void printVector(vector<vector<int> > data)
{
	for (size_t i=0; i < data.size(); i++) {
		vector<int> itemSet = data.at(i);
		for (size_t j=0; j < itemSet.size(); j++) {
			cout << itemSet.at(j) << " ";
		}
		cout << endl;
	}
}

// debug
void printMap(map<int,int> data)
{
	map<int,int>::iterator it;

	// show content:
	for (map<int,int>::iterator it=data.begin(); it != data.end(); ++it)
	{
		cout << it->first << " => " << it->second << '\n';
	}
}

// debug
void printSortedItems(vector<pair<int,int> > items)
{
	for (size_t i=0; i < items.size(); i++)
	{
		cout << items.at(i).first << " -> " << items.at(i).second << endl;
	}
}

// sorts the transactions in the order that the header table is in
vector<vector<int> > sortTransactions(vector<vector<int> > transactions, vector<pair<int,int> > sortedHeaderTable)
{
	vector<vector<int> > sortedTransactions;
	
	for (size_t i = 0; i < transactions.size(); i++)
	{
		vector<int> sortedTransaction;
		
		for (vector<pair<int,int> >::iterator it = sortedHeaderTable.begin(); it != sortedHeaderTable.end(); ++it)
		{
			if (find(transactions[i].begin(), transactions[i].end(), it->first) != transactions[i].end())
			{
				sortedTransaction.push_back(it->first);
			}
		}
		
		sortedTransactions.push_back(sortedTransaction);
	}
	
	return sortedTransactions;
}

// function used to sort the header table pairs
bool sortHeaderTableFunction(pair<int,int> i, pair<int,int> j)
{
	return (i.second > j.second);
}

// sorts the items in the header table map into pairs in a vector
// since maps can't be sorted on their keys
vector<pair<int,int> > sortHeaderTable(map<int, int> headerTable, int minSup)
{
	vector<pair<int,int> > sortedHeaderTable;
	
	for (map<int,int>::iterator it = headerTable.begin(); it != headerTable.end(); ++it)
	{
		if (it->second >= minSup)
		{
			sortedHeaderTable.push_back(*it);
		}
	}
	sort(sortedHeaderTable.begin(), sortedHeaderTable.end(), sortHeaderTableFunction);
	
	return sortedHeaderTable;
}

void projectTables
(
	int minSup,
	vector<vector<int> > sortedTransactions,
	vector<pair<int,int> > sortedHeaderTable,
	Tree fpTree,
	vector<pair<vector<int>,int> > *frequentPatterns,
	vector<int> currentPattern
)
{
	pair<vector<int>,int> patternWithCount;

	if (sortedHeaderTable.size() > 1)
	{
		for (size_t i = sortedHeaderTable.size() - 1; i > 0; i--)
		{
			vector<vector<int> > *projTransactions = new vector<vector<int> >;
			map<int,int> *projHeaderTable = new map<int,int>;
			vector<pair<int,int> > sortedProjHeaderTable;
			vector<vector<int> > sortedProjTransactions;
			Tree projTree;
		
			// Add the current frequent pattern to the list
			currentPattern.push_back(sortedHeaderTable.at(i).first);
			patternWithCount.first = currentPattern;
			patternWithCount.second = sortedHeaderTable.at(i).second;
			(*frequentPatterns).push_back(patternWithCount);

			// Create the projected transactions and header table for the current item
			fpTree.projTable(sortedHeaderTable.at(i).first, sortedHeaderTable.at(i).second, projTransactions, projHeaderTable);
		
			// Sort the projected header table
			sortedProjHeaderTable = sortHeaderTable(*projHeaderTable, minSup);
			printSortedItems(sortedProjHeaderTable); // debug
		
			// Sort the projected transactions
			sortedProjTransactions = sortTransactions(*projTransactions, sortedProjHeaderTable);
			printVector(sortedProjTransactions); // debug
	
			// Create the projected tree
			for (size_t j = 0; j < sortedProjTransactions.size(); j++)
			{
				projTree.insert(sortedProjTransactions.at(j));
			}
		
			// Do the recursive call
			if (!sortedProjHeaderTable.empty())
			{
				projectTables(minSup, sortedProjTransactions, sortedProjHeaderTable, projTree, frequentPatterns, currentPattern);
			}
			currentPattern.clear();
		}
		// Add the current frequent pattern to the list
		currentPattern.push_back(sortedHeaderTable.at(0).first);
		patternWithCount.first = currentPattern;
		patternWithCount.second = sortedHeaderTable.at(0).second;
		(*frequentPatterns).push_back(patternWithCount);
	}
	else if (sortedHeaderTable.size() == 1)
	{
		// Add the current frequent pattern to the list
		currentPattern.push_back(sortedHeaderTable.at(0).first);
		patternWithCount.first = currentPattern;
		patternWithCount.second = sortedHeaderTable.at(0).second;
		(*frequentPatterns).push_back(patternWithCount);
	}
}

// debug
void printPatterns(vector<pair<vector<int>,int> > frequentPatterns)
{
	for (size_t i = 0; i < frequentPatterns.size(); i++)
	{
		for (size_t j = 0; j < frequentPatterns.at(i).first.size(); j++)
		{
			cout << frequentPatterns.at(i).first.at(j) << " ";
		}
		cout << "(" << frequentPatterns.at(i).second << ")" << endl;
	}
}

int main()
{
	string line;
	int arraySize;
	int minSup;
	string filename = "";
	vector<vector<int> > transactions;
	map<int,int> headerTable;
	vector<pair<int,int> > sortedHeaderTable;
	vector<vector<int> > sortedTransactions;
	Tree fpTree;
	vector<pair<vector<int>,int> > frequentPatterns;
	
	// debug uncomment this when finished testing
//	while (filename.size() == 0)
//	{
//		cout << "Please enter the name of a valid file to mine" << endl;
//		getline(cin, filename);
//	}

	filename = "test1.txt"; // debug
	
	ifstream file(filename.c_str());
	
	//open file
	if (file.is_open())
	{
		cout << "Please enter a minSup" << endl;
		getline(cin, line);
		minSup = atoi(line.c_str());
	
		//Get the first number in the file to determine the size of the array
		if (getline(file,line))
		{
			arraySize = line[0] - '0';
		}
		else
		{
			cout << "Unable to read file";
			exit(1);
		}

		//read file line by line
		while(getline(file,line))
		{
			string rawText = "";
			int itemSetSize;
			size_t i = 0;

			// Find the number of items on the line
			while (line[i] != '\t')
			{
				i++;
			}
			i++;

			while (line[i] != '\t') {
				rawText += line[i++];
			}
			itemSetSize = atoi(rawText.c_str());

			// Find the items on the line
			vector<int> itemSet;
			i++;
			while (i < line.size())
			{
				int item;
				
				rawText = "";
				while (line[i] != ' ' && i < line.size()) {
					rawText += line[i++];
				}
				item = atoi(rawText.c_str());
				itemSet.push_back(item);
				
				// Add to header table
				headerTable[item]++;

				i++;
			}
			transactions.push_back(itemSet);
		}
		file.close();
	}
	else
	{
		cout << "Unable to open file" << endl;
	}

//	printVector(transactions); // debug
//	cout << "Map" << endl; // debug
//	printMap(headerTable); // debug
//	cout << endl; // debug
	sortedHeaderTable = sortHeaderTable(headerTable, minSup);
	printSortedItems(sortedHeaderTable); // debug
	
	// Sort the transactions
	sortedTransactions = sortTransactions(transactions, sortedHeaderTable);
	printVector(sortedTransactions); // debug
	
	// Create the tree
	for (size_t i = 0; i < sortedTransactions.size(); i++)
	{
		fpTree.insert(sortedTransactions.at(i));
	}
	
	// Create the projection tables, their header tables, and their trees
	vector<int> currentPattern;
	projectTables(minSup, sortedTransactions, sortedHeaderTable, fpTree, &frequentPatterns, currentPattern);

	printPatterns(frequentPatterns); // debug
	
	cout << "End of processing."; // debug
}