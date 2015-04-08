#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <map>
#include <fstream>
#include <ctime>
#include <bits/stdc++.h>

using namespace std;

// Global Variables
map<string, int> nameToIndex;

// Format checker just assumes you have Alarm.bif and Solved_Alarm.bif (your file) in current directory


// Our graph consists of a list of nodes where each node is represented as follows:
class Graph_Node
{

private:
	string Node_Name; // Variable name
	vector<int> Children; // Children of a particular node - these are index of nodes in graph.
	vector<string> Parents; // Parents of a particular node- note these are names of parents
	int nvalues; // Number of categories a variable represented by this node can take
	vector<string> values; // Categories of possible values
	vector<double> CPT; // conditional probability table as a 1-d array . Look for BIF format to understand its meaning

public:
	// Constructor- a node is initialised with its name and its categories
	Graph_Node(string name, int n, vector<string> vals)
	{
		Node_Name = name;
		nvalues = n;
		values = vals;

	}
	string get_name()
	{
		return Node_Name;
	}
	vector<int> get_children()
	{
		return Children;
	}
	vector<string> get_Parents()
	{
		return Parents;
	}
	vector<double> get_CPT()
	{
		return CPT;
	}
	int get_nvalues()
	{
		return nvalues;
	}
	vector<string> get_values()
	{
		return values;
	}
	void set_CPT(vector<double> new_CPT)
	{
		CPT.clear();
		CPT = new_CPT;
	}
	void set_Parents(vector<string> Parent_Nodes)
	{
		Parents.clear();
		Parents = Parent_Nodes;
	}
	// add another node in a graph as a child of this node
	int add_child(int new_child_index)
	{
		for (int i = 0; i < Children.size(); i++)
		{
			if (Children[i] == new_child_index)
				return 0;
		}
		Children.push_back(new_child_index);
		return 1;
	}

};

// The whole network represted as a list of nodes
class network
{

	list<Graph_Node> Pres_Graph;

public:
	int addNode(Graph_Node node)
	{
		Pres_Graph.push_back(node);
		return 0;
	}

	int netSize()
	{
		return Pres_Graph.size();
	}
	// get the index of node with a given name
	int get_index(string val_name)
	{
		list<Graph_Node>::iterator listIt;
		int count = 0;
		for (listIt = Pres_Graph.begin(); listIt != Pres_Graph.end(); listIt++)
		{
			if (listIt->get_name().compare(val_name) == 0)
				return count;
			count++;
		}
		return -1;
	}
	// get the node at nth index
	list<Graph_Node>::iterator get_nth_node(int n)
	{
		list<Graph_Node>::iterator listIt;
		int count = 0;
		for (listIt = Pres_Graph.begin(); listIt != Pres_Graph.end(); listIt++)
		{
			if (count == n)
				return listIt;
			count++;
		}
		return listIt;
	}
	//get the iterator of a node with a given name
	list<Graph_Node>::iterator search_node(string val_name)
	{
		list<Graph_Node>::iterator listIt;
		for (listIt = Pres_Graph.begin(); listIt != Pres_Graph.end(); listIt++)
		{
			if (listIt->get_name().compare(val_name) == 0)
				return listIt;
		}

		cout << "node not found\n";
		return listIt;
	}

};

network read_network()
{
	network Alarm;
	string line;
	int find = 0;
	ifstream myfile("alarm.bif");
	string temp;
	string name;
	vector<string> values;

	if (myfile.is_open())
	{
		while (!myfile.eof())
		{
			stringstream ss;
			getline(myfile, line);

			ss.str(line);
			ss >> temp;

			if (temp.compare("variable") == 0)
			{

				ss >> name;
				getline(myfile, line);

				stringstream ss2;
				ss2.str(line);
				for (int i = 0; i < 4; i++)
				{

					ss2 >> temp;

				}
				values.clear();
				while (temp.compare("};") != 0)
				{
					values.push_back(temp);

					ss2 >> temp;
				}
				Graph_Node new_node(name, values.size(), values);
				int pos = Alarm.addNode(new_node);

			}
			else if (temp.compare("probability") == 0)
			{

				ss >> temp;
				ss >> temp;

				list<Graph_Node>::iterator listIt;
				list<Graph_Node>::iterator listIt1;
				listIt = Alarm.search_node(temp);
				int index = Alarm.get_index(temp);
				ss >> temp;
				values.clear();
				while (temp.compare(")") != 0)
				{
					listIt1 = Alarm.search_node(temp);
					listIt1->add_child(index);
					values.push_back(temp);

					ss >> temp;

				}
				listIt->set_Parents(values);
				getline(myfile, line);
				stringstream ss2;

				ss2.str(line);
				ss2 >> temp;

				ss2 >> temp;

				vector<double> curr_CPT;
				string::size_type sz;
				while (temp.compare(";") != 0)
				{

					curr_CPT.push_back(atof(temp.c_str()));

					ss2 >> temp;

				}

				listIt->set_CPT(curr_CPT);

			}
			else
			{

			}
		}

		if (find == 1)
			myfile.close();
	}

	return Alarm;
}

vector<string> getPossibleValues(string nodeName, network Alarm)
{
	// iterate over the graph and get the possible values of the node whose name matches with nodeName

	list<Graph_Node>::iterator temp = Alarm.search_node(nodeName);

	//if (temp != NULL) { TODO: what to write here
	return (*temp).get_values();
	//}

	cerr << "Node of the given name " << nodeName
			<< " not found during searching for values";

	return (*temp).get_values();
}

string convertIntToString(int a)
{
	stringstream ss;
	ss << a;
	return ss.str();
}
void printStringVector(vector<string> v)
{
	for (vector<string>::iterator i = v.begin(); i != v.end(); i++)
	{
		cout << *i << " ";
	}
	cout << endl;
}
void splitString(string message, string delimiter, string result[])
{
	int i = 0;
	int pos = 0;
	int length = 0;
	int temp;
	temp = message.find(delimiter.c_str(), pos);
	while (temp != -1)
	{
		length = temp - pos;
		result[i] = message.substr(pos, length);
		pos = temp + delimiter.size();
		temp = message.find(delimiter.c_str(), pos);
		i++;
	}

	result[i] = message.substr(pos);
	i++;

}

vector<string> possibleCombinations(vector<string> parents, network Alarm)
{
	vector<string> temp;
	// take out first node

	if (parents.size() == 0)
		return temp;

	string first_node = parents.front();
	vector<string> my_values = getPossibleValues(first_node, Alarm);
	//cout << "My Values in..." << endl;
	//printStringVector(my_values);
	parents.erase(parents.begin());
	vector<string> possible_values_for_remaining = possibleCombinations(
			parents, Alarm);

	for (vector<string>::iterator my_value = my_values.begin(); my_value
			!= my_values.end(); my_value++)
	{
		if (possible_values_for_remaining.size() != 0)
		{
			for (vector<string>::iterator other =
					possible_values_for_remaining.begin(); other
					!= possible_values_for_remaining.end(); other++)
			{
				string t = *my_value;
				t = t + " " + (*other);
				temp.push_back(t);
			}
		}
		else
		{
			string t = *my_value;
			temp.push_back(t);
		}
	}

	return temp;
}

double getMeCPTValue(string myName, string myVal)
{
	// reading file

	//cout << "Inside get me CPT Value :\n" << endl;
	//cout << "My name :" << myName << endl;
	//cout << "My Val:" << myVal << endl;

	string inputFileName = "records.dat";
	ifstream inputFile;
	string line;
	inputFile.open(inputFileName.c_str());

	double overAllCount = 0.0f;
	double matchedCount = 0.0f;

	if (inputFile.is_open())
	{
		while (getline(inputFile, line))
		{

			string vars[37];
			splitString(line, " ", vars);

			bool countRow = true; // No ? in the row
			bool reqdValFound = true;

			//cout << "line" << line << endl;

			// Check for myself and my parents
			int myIndex = nameToIndex[myName];

			if (vars[myIndex].compare("\"?\"") == 0)
			{
				//cout<<"Question mark found"<<endl;
				continue;
			}

			if (vars[myIndex].compare((myVal)) != 0)
			{
				//cout << "index:" << vars[myIndex] << endl;
				//cout << "myval:" << myVal << endl;
				reqdValFound = false;
			}

			if (countRow == false)
			{
				continue;
			}
			else if (reqdValFound == false)
			{
				overAllCount += 1;
			}
			else
			{
				overAllCount += 1;
				matchedCount += 1;
			}
		}
	}
	if (overAllCount != 0)
	{
		//cout<<"Node:"<<myName<<" Value:"<<myVal<<endl;
		//cout << "Overall Count" << overAllCount << endl;
		//cout << "matched count" << matchedCount << endl;
		double result = matchedCount / overAllCount;
		//cout << "Result" << result << endl;
		return result;
	}
	else
	{
		cout << "Error while reading data inside 2 parameters" << endl;
		return 0.0;
	}
}

double getMeCPTValue(string myName, string myVal,
		vector<string> nameOfMyParents, string valuesOfMyParents)
{
	// reading file

//	cout << "Inside get me CPT Value :\n" << endl;
//	cout << "My name :" << myName << endl;
//	cout << "My Val:" << myVal << endl;
//	cout << "Values of my parents:" << valuesOfMyParents << endl;
//	cout << "Name of my parents:\n";
//	printStringVector(nameOfMyParents);
	//cout << endl;
	string inputFileName = "records.dat";
	ifstream inputFile;
	string line;
	inputFile.open(inputFileName.c_str());

	double overAllCount = 0.0f;
	double matchedCount = 0.0f;

	if (inputFile.is_open())
	{
		while (getline(inputFile, line))
		{
			string vars[37];
			splitString(line, " ", vars);

			bool countRow = true; // No ? in the row
			bool reqdValFound = true;

			// Check for myself and my parents
			int myIndex = nameToIndex[myName];

			if (vars[myIndex].compare("\"?\"") == 0)
			{
				continue;
			}

			if (vars[myIndex].compare((myVal)) != 0)
			{
				reqdValFound = false;
			}

			// check for each parent
			string parentsVal[37];
			splitString(valuesOfMyParents, " ", parentsVal);
			//cout<<"Loop start\n";
			for (int i = 0; i < nameOfMyParents.size(); i++)
			{
				//cout<<"i is:"<<i<<" Checking for parent "<<nameOfMyParents.at(i)<<" at index "<<nameToIndex[nameOfMyParents.at(i)]<<endl;
				if (vars[nameToIndex[nameOfMyParents.at(i)]].compare("\"?\"")
						== 0)
				{
					countRow = false;
					break;
				}
				else if (vars[nameToIndex[nameOfMyParents.at(i)]].compare(
						parentsVal[i]) != 0)
				{
					countRow = false;
					//break;
				}
			}
			//cout<<"Loop end\n";

			if (countRow == false)
			{
				continue;
			}
			else if (reqdValFound == false)
			{
				overAllCount += 1;
			}
			else
			{
				matchedCount += 1;
				overAllCount += 1;
			}

		}
	}
	if (overAllCount != 0)
	{
//		cout<<"Node:"<<myName<<" Value:"<<myVal<<endl;
//		cout << "Overall Count" << overAllCount << endl;
//		cout << "matched count" << matchedCount << endl;
		double result = matchedCount / overAllCount;
//		cout << "Result" << result << endl;
		return result;
	}
	else
	{
		cout << "Error while reading data inside 4 parameters" << endl;
		return 0.0;
	}
}

vector<double> generateCPTValueForThisNode(Graph_Node node, network Alarm)
{
	// here we will have parents
	// we need to iterate through one possible value of this node and one value of this parent
	vector<double> cptValues;
	vector<string> myPossibleValues = node.get_values();

	//cout << "My Possible Values:" << endl;
	//printStringVector(myPossibleValues);

	vector<string> parents = node.get_Parents();

	//cout << "My patents:" << endl;
	//printStringVector(parents);

	vector<string> possibleCombinationsOfParents = possibleCombinations(
			parents, Alarm);

	//cout << "Possible combinations of parents:" << endl;
	//printStringVector(possibleCombinationsOfParents);

	for (int i = 0; i < node.get_nvalues(); i++)
	{
		if (parents.size() != 0)
		{
			for (vector<string>::iterator parentC =
					possibleCombinationsOfParents.begin(); parentC
					!= possibleCombinationsOfParents.end(); parentC++)
			{
				double oneCPTVal = getMeCPTValue(node.get_name(),
						myPossibleValues.at(i), parents, (*parentC));
				cptValues.push_back(oneCPTVal);
			}
		}
		else
		{
			double cptValForthisVal = getMeCPTValue(node.get_name(),
					myPossibleValues.at(i));
			cptValues.push_back(cptValForthisVal);
		}
	}
	return cptValues;
}

void generateMapNameToIndex(network Alarm)
{
	for (int i = 0; i < Alarm.netSize(); i++)
	{
		Graph_Node current_node = (*(Alarm.get_nth_node(i)));
		nameToIndex[current_node.get_name()] = i;
	}
}

void printCPTValues(vector<double> v)
{
	for (vector<double>::iterator i = v.begin(); i != v.end(); i++)
	{
		cout << *i << " ";
	}
	cout << endl;
}

void writeOutput(network Alarm)
{
	list<Graph_Node>::iterator lit;
	ofstream ofile("solved_alarm.bif");
	ifstream ifile("alarm.bif");
	vector<double> cpts;
	stringstream ss;
	string line, temp;

	if (ifile.is_open())
	{
		while (getline(ifile, line))
		{
			ss.str(line);
			ss >> temp;

			/* Traverse the graph and get the values */
			if (temp.compare("probability") == 0)
			{
				ofile << line << endl;
				ofile << "\ttable ";

				/* Getting the variable */
				ss >> temp;
				ss >> temp;

				/* Getting the node and the CPT values */
				lit = Alarm.search_node(temp);
				cpts = lit->get_CPT();

				/* Outputting the CPT values to the file */
				for (int i = 0; i < cpts.size(); i++)
					ofile << fixed << setprecision(4) << cpts[i] << " ";

				// Ending the probability part
				ofile << ";" << endl << "}" << endl;

				cpts.clear();

				getline(ifile, line);
				getline(ifile, line);
			}

			else
				ofile << line << endl;

			ss.str("");
			ss.clear();
		}

		ofile.close();
		ifile.close();
	}

	else
		cout << "Unable to open file\n";
}

int main()
{
	network Alarm;
	Alarm = read_network();

	generateMapNameToIndex(Alarm);

	// Iterate over all nodes

	for (int i = 0; i < Alarm.netSize(); i++)
	{
		Graph_Node current_node = (*(Alarm.get_nth_node(i)));
		//cout << " Processing node:" << current_node.get_name() << endl;
		vector<double> cptValues = generateCPTValueForThisNode(current_node,
				Alarm);

		(*(Alarm.get_nth_node(i))).set_CPT(cptValues);
	}

	// printing CPT values of the all nodes
	for (int i = 0; i < Alarm.netSize(); i++)
	{
		Graph_Node current_node = (*(Alarm.get_nth_node(i)));
		cout << "Current Node:" << current_node.get_name() << endl;
		printCPTValues(current_node.get_CPT());
	}

	writeOutput(Alarm);

	// Example: to do something
	cout << "Perfect! Hurrah! \n";
	return 0;
}

