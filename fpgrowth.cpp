#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;

// 2D array for header table
//int[][] createHeaderTable(){

//}

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

main()
{
	string line;
	char character;
	int arraySize;
	ifstream file ("test_data.txt");
	vector<vector<int> > data;
	
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
				rawText = "";
				while (line[i] != ' ' && i < line.size()) {
					rawText += line[i++];
				}
				itemSet.push_back(atoi(rawText.c_str()));

				i++;
			}
			data.push_back(itemSet);
		}
		file.close();
	}
	else
	{
		cout << "Unable to open file";
	}

	printVector(data);
}