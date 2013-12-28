#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <map>
#include <algorithm>

using namespace std;

// debug
void printVector(vector<vector<int> > data)
{
	for (int i=0; i < data.size(); i++) {
		vector<int> itemSet = data.at(i);
		for (int j=0; j < itemSet.size(); j++) {
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
	for (int i=0; i < items.size(); i++)
	{
		cout << items.at(i).first << " -> " << items.at(i).second << endl;
	}
}

// function used to sort the header table pairs
bool sortFunction(pair<int,int> i, pair<int,int> j)
{
	return (i.second >= j.second);
}

// sorts the items in the header table map into pairs in a vector
// since maps can't be sorted on their keys
vector<pair<int,int> > sortItems(map<int, int> items)
{
	vector<pair<int,int> > sortedItems;
	
	for (map<int,int>::iterator it = items.begin(); it != items.end(); ++it)
	{
		sortedItems.push_back(*it);
	}
	sort(sortedItems.begin(), sortedItems.end(), sortFunction);
	
	return sortedItems;
}

main()
{
	string line;
	char character;
	int arraySize;
	ifstream file ("10k5L.txt");
	vector<vector<int> > itemSets;
	map<int,int> headerTable;
	vector<pair<int,int> > sortedHeaderTable;
	
	//open file
	if (file.is_open())
	{
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
			int i = 0;

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
			itemSets.push_back(itemSet);
		}
		file.close();
	}
	else
	{
		cout << "Unable to open file";
	}

//	printVector(itemSets); // debug
//	cout << "Map" << endl; // debug
//	printMap(headerTable); // debug
//	cout << endl; // debug
	sortedHeaderTable = sortItems(headerTable);
	printSortedItems(sortedHeaderTable); // debug
	cout << "End of processing."; // debug
	
	// Sort the itemsets
}