/*
 * WeightedGraph.h
 *
 *  Created on: 24 Apr 2025
 *      Author: Dongmo
 */
#ifndef H_weightedGraph
#define H_weightedGraph

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cfloat> // for DBL_MAX, meaning the maximal number in double

using namespace std;

class WeightedGraphType {
protected:
	int gSize;      //number of vertices
	list<int> *graph; // Store adjacency list
	double **weights; // Store weights of edges
public:
	WeightedGraphType(int size = 0);
	~WeightedGraphType();

	list<int> getAdjancencyList(int index) {
		return graph[index];
	}

	double getWeight(int i, int j) {
		return weights[i][j];
	}

	void printAdjacencyList();
	void printAdjacencyMatrix();

	vector<int> breadthFirstTraversal();
};

WeightedGraphType::WeightedGraphType(int size) {
	gSize = 0;
	ifstream infile;
	char fileName[50] = "Weights.txt";

//	cout << "Enter graph adjacency matrix file name: ";
//	cin >> fileName;
//	cout << endl;

	infile.open(fileName);

	if (!infile) {
		cout << "Cannot open input file." << endl;
		return;
	}

	gSize = size;
	//array of lists, store the numbers of adjacent vertices of each node
	graph = new list<int> [gSize];
	//2D array of double, stores  weight (distance) of node to adjacent vertices
	weights = new double*[gSize];

	for (int i = 0; i < gSize; i++)
		weights[i] = new double[gSize];

	for (int i = 0; i < gSize; i++) {
		for (int j = 0; j < gSize; j++) {
			double value;
			infile >> value;
			if (value == 0)
				weights[i][j] = DBL_MAX; //system constant - maximum value of double
			else {
				weights[i][j] = value;
				graph[i].push_back(j);
			}
		}
	}
	infile.close();
}

WeightedGraphType::~WeightedGraphType() {
	for (int i = 0; i < gSize; i++)
		delete[] weights[i];

	delete[] weights;

	for (int index = 0; index < gSize; index++)
		graph[index].clear();

	delete[] graph;
}

void WeightedGraphType::printAdjacencyMatrix() { //print adjacency matrix for debug purpose
	cout<<"\nAdjacency Matrix" << endl;
	for (int i = 0; i < gSize; i++) {
		for (int j = 0; j < gSize; j++) {
			cout << setw(8) << (weights[i][j] == DBL_MAX ? 0.0 : weights[i][j]); //as adjacency value, zero means no direct connection
		}
		cout << endl;
	}
}

void WeightedGraphType::printAdjacencyList() { //print adjacency list for debug purpose
	cout<<"\nAdjacency List" << endl;
	for (int index = 0; index < gSize; index++) {
		cout << index << ": ";
		list<int>::iterator it;
		for (it= graph[index].begin();it != graph[index].end(); it++)
			cout << *it << " ";
		cout << endl;
	}

	cout << endl;
}

vector<int> WeightedGraphType::breadthFirstTraversal() {
    vector<int> result;
    queue<int> queue;
    bool *visited = new bool[gSize];

    for (int ind = 0; ind < gSize; ind++)
        visited[ind] = false;

    for (int index = 0; index < gSize; index++) {
        if (!visited[index]) {
            queue.push(index);
            visited[index] = true;
            result.push_back(index);

            while (!queue.empty()) {
                int u = queue.front();
                queue.pop();

                for (int w : graph[u]) {
                    if (!visited[w]) {
                        queue.push(w);
                        visited[w] = true;
                        result.push_back(w);
                    }
                }
            }
        }
    }

    delete[] visited;
    return result;
}

#endif
