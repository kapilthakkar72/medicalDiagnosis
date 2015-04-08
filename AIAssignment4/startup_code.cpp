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


typedef pair<int, int> ii;

struct record
{
	double wt;
	vector<string> var;
	record(double w, vector<string> v)
	{
		wt = w;
		var = v;
	}
};

map<string, int> nameToIndex;
vector<record> table;

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

//Gets me the node directly
vector<list<Graph_Node>::iterator> indexToNode;

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

double getMeCPTValueWithWt(string myName, string myVal)
{
	// reading file

	//cout << "Inside get me CPT Value :\n" << endl;
	//cout << "My name :" << myName << endl;
	//cout << "My Val:" << myVal << endl;

	double overAllCount = 0.0f;
	double matchedCount = 0.0f;

	for (int i = 0; i < table.size(); i++)
	{
		bool countRow = true; // No ? in the row
		bool reqdValFound = true;

		//cout << "line" << line << endl;

		// Check for myself and my parents
		int myIndex = nameToIndex[myName];

		if (table.at(i).var.at(myIndex).compare("\"?\"") == 0)
		{
			//cout<<"Question mark found"<<endl;
			continue;
		}

		if (table.at(i).var.at(myIndex).compare((myVal)) != 0)
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
			overAllCount += table.at(i).wt;
		}
		else
		{
			overAllCount += table.at(i).wt;
			matchedCount += table.at(i).wt;
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

double getMeCPTValueWithWt(string myName, string myVal,
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

	double overAllCount = 0.0;
	double matchedCount = 0.0;

	for (int j = 0; j < table.size(); j++)
	{

		bool countRow = true; // No ? in the row
		bool reqdValFound = true;

		// Check for myself and my parents
		int myIndex = nameToIndex[myName];

		if (table.at(j).var.at(myIndex).compare("\"?\"") == 0)
		{
			continue;
		}

		if (table.at(j).var.at(myIndex).compare((myVal)) != 0)
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
			if (table.at(j).var.at(nameToIndex[nameOfMyParents.at(i)]).compare(
					"\"?\"") == 0)
			{
				countRow = false;
				break;
			}
			else if (table.at(j).var.at(nameToIndex[nameOfMyParents.at(i)]).compare(
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
			overAllCount += table.at(j).wt;
		}
		else
		{
			matchedCount += table.at(j).wt;
			overAllCount += table.at(j).wt;
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

vector<double> generateCPTValueForThisNodeWithWt(Graph_Node node, network Alarm)
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
				double oneCPTVal = getMeCPTValueWithWt(node.get_name(),
						myPossibleValues.at(i), parents, (*parentC));
				cptValues.push_back(oneCPTVal);
			}
		}
		else
		{
			double cptValForthisVal = getMeCPTValueWithWt(node.get_name(),
					myPossibleValues.at(i));
			cptValues.push_back(cptValForthisVal);
		}
	}
	return cptValues;
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

/* First is the index, next is the size
 Returns the index from which to fetch the CPT value */
int calculateIndex(vector<ii> indices)
{
	int prod = 1, sum = 0;
	for (int i = indices.size() - 1; i >= 0; i--)
	{
		sum += prod * indices[i].first;
		prod *= indices[i].second;
	}
	return sum;
}

// this function returns the index at which the give val is present in the possible vals
int indexOfValue(list<Graph_Node>::iterator it, string val)
{
	Graph_Node g = *it;
	vector<string> values = g.get_values();
	int noOfValues = g.get_nvalues();
	for (i = 0; i < noOfValues; ++i)
	{
		if (val.compare(values.at(i)) == 0)
		{
			return i;
		}
		else
		{
			cerr << "Could not find the given value\n";
			return 0;
		}
	}
}

double computeProbability(vector<string> sample)
{
	list<Graph_Node>::iterator lit, plit;
	vector<string> parents;
	vector<ii> indices;
	vector<double> myCPT;
	double prob = 1.0;
	int k, n, p;
	for (int i = 0; i < sample.size(); i++)
	{
		//TODO:ASK lit = indexToName[i];

		lit = indexToNode.at(i);

		//My cpt table
		myCPT = lit->get_CPT();
		//Index of my sample value
		k = lit->get_index(sample[i]);
		//My domain size
		n = lit->get_nvalues();
		indices.push_back(ii(k, n));
		//Get my parents
		parents = lit->get_Parents();
		for (int j = 0; j < parents.size(); j++)
		{
			// Iterator to the parent
			p = nameToIndex[parents[j]];
			plit = indexToNode[k];
			k = plit->get_index(sample[p]);
			n = plit->get_nvalues();
			indices.push_back(ii(k, n));
		}
		int index = calculateIndex(indices);
		prob *= myCPT[index];
	}
	return prob;
}

void initializeData(network Alarm)
{
	ifstream ifile("records.dat");
	list<Graph_Node>::iterator lit;
	string line, value, unknownVar;
	//TODO:ASK vector<double> domain;

	vector<string> domain;

	vector<string> tempVar(37);
	stringstream ss;
	int unknown;
	while (getline(ifile, line))
	{
		//Iterating over each line to construct the inital table
		ss.str(line);
		int i = 0;
		unknown = -1;
		while (getline(ss, value))
		{
			if (value == "\"?\"")
				unknown = i;
			else
				tempVar[i] = value;
			i++;
		}
		// If the record has a unknown value
		if (unknown != -1)
		{
			// Get the variable name
			//unknownVar = indexToName[unknown];
			// Get the node in the graph
			//lit = Alarm.search_node(unknownVar);
			lit = indexToNode[unknown];

			// Get the domain values TODO:RELATE
			domain = lit->get_values();
			// Iterate over the domain values and get the complete sample
			double w = 0.0, tw = 0.0;
			int ds = domain.size();
			for (int j = 0; j < ds; j++)
			{
				tempVar[unknown] = domain[j];
				//tempVar has the complete sample, compute its probability
				w = computeProbability(tempVar);
				tw += w;
				//Insert into the table
				record r(w, tempVar);
				table.push_back(r);
			}
			//Need to normalize the weights
			//double totalWt = accumulate(wts.begin(), wts.end(), 0.0);
			for (int j = table.size() - 1; j >= (table.size() - ds); j--)
				table[j].wt /= tw;
			wts.clear();
			domain.clear();
		}
		else
		{
			// Insert in the table the sample with weight 1
			record r(1.0, tempVar);
			table.push_back(r);
		}
		tempVar.clear();
	}
}

void recalculateTable()
{
	for (int i = 0; i < table.size(); i++)
	{
		double nw = computeProbability(table[i].var);
		table[i].wt = nw;
	}
}

int main()
{
	network Alarm;
	Alarm = read_network();

	generateMapNameToIndex(Alarm);

	// This will generate structure(vector) to map index to node
	for (int i = 0; i < Alarm.netSize(); i++)
		indexToNode.push_back(Alarm.get_nth_node(i));

	// Iterate over all nodes
	// Initialisation step

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

	cout << "Initialization done\nGoing for Algorithm\n";

	// Initializing "?" in records

	initializeData(Alarm);

	// calculating CPT values

	for (int i = 0; i < Alarm.netSize(); i++)
	{
		Graph_Node current_node = (*(Alarm.get_nth_node(i)));
		//cout << " Processing node:" << current_node.get_name() << endl;
		vector<double> cptValues = generateCPTValueForThisNodeWithWt(
				current_node, Alarm);

		(*(Alarm.get_nth_node(i))).set_CPT(cptValues);
	}

	writeOutput(Alarm);

	// Example: to do something
	cout << "Perfect! Hurrah! \n";
	return 0;
}
