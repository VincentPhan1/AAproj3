#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

//Using an adjacency list, graphs are vectors of vectors of pairs (vertex, weight)
using Graph = vector<vector<pair<int, int>>>;

vector<Graph> readGraphs(const string& filename) {
    fstream inputFile(filename);
    if (!inputFile.is_open()) throw runtime_error("Error opening file!");

    int numGraphs;
    if (!(inputFile >> numGraphs) || numGraphs <= 0) throw runtime_error("Invalid number of graphs!");

    vector<Graph> graphs; //incase there are multiple graphs we need to process

    for (int g = 0; g < numGraphs; ++g) {
        int numVertices, numEdges;
        inputFile >> numVertices >> numEdges;  // Read number of vertices and edges

        Graph graph(numVertices);  // Create an empty graph with numVertices vertices

        for (int e = 0; e < numEdges; ++e) {
            int u, v, w;
            inputFile >> u >> v >> w;  // Read the edge data

            // Add the edge to the adjacency list (directed)
            graph[u].emplace_back(v, w);  // Add directed edge from u to v with weight w
        }

        graphs.push_back(graph);  // Store the created graph in the list
    }

    inputFile.close();  // Close the input file
    return graphs;  // Return the list of graph
}

pair<int, vector<pair<int, int>>> prim(const Graph& graph) {
    int n = graph.size();
    vector<bool> inMST(n, false);  // Track vertices in MST
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    pq.push({0, 0});

    vector<pair<int,int>> mstEdges;
    int mstWeight = 0;

    while (!pq.empty()) {
        pair<int,int> current = pq.top();
        pq.pop();

        int weight = current.first;
        int vertex = current.second;
        std::cout << "Processing vertex " << vertex << " with weight " << weight << std::endl;

        if (inMST[vertex]) continue;

        // Adding to MST
        inMST[vertex] = true;
        mstWeight += weight;

        if (vertex != 0) { // Avoid adding the initial zero-weight edge to the MST
            mstEdges.push_back(current);
        }

        // Add adjacent edges
        for (const auto& neighbor : graph[vertex]) {
            int adjVertex = neighbor.first;
            int adjWeight = neighbor.second;
            if (!inMST[adjVertex]) {
                pq.push({adjWeight, adjVertex});

            }

        }
    }

    sort(mstEdges.begin(), mstEdges.end());
    return {mstWeight, mstEdges};
}

void writeMSTOutput(const string& filename, const pair<int, vector<pair<int, int>>>& mstResult) {
    ofstream outputFile(filename);
    if (!outputFile.is_open()) throw runtime_error("Error opening output file!");

    // Write total weight of MST
    outputFile << mstResult.first << endl;

    // Write edges in the MST in sorted order
    for (const auto& edge : mstResult.second) {
        outputFile << edge.first << " " << edge.second << endl;
    }
    outputFile.close();
}

void displayGraph(const Graph& graph) {
    for (size_t i = 0; i < graph.size(); ++i) {
        cout << "Vertex " << i << " connects to:";
        for (const auto& edge : graph[i]) {
            cout << " (" << edge.first << ", " << edge.second << ")";
        }
        cout << endl;
    }
}

void processFile(const char* inputFile, const char* outputFile) {
    vector<Graph> graphs; //incase there are multiple graphs we need to process
    try {
        graphs = readGraphs(inputFile);
        if (graphs.empty()) {
            cerr << "No graphs found in input." << endl;
            return;
        }
        displayGraph(graphs[0]);

        for (int i = 0; i < graphs.size(); ++i) {
            auto mstResult = prim(graphs[i]);
            writeMSTOutput(outputFile, mstResult);
        }

    } catch (const runtime_error& e) {
        cerr << "Error: " << e.what() << endl;
    }
}

void usage(char* program) {
    cerr << "Usage: " << program << " <input file> <output file>" << endl;
    exit(1);
}

int main(int argc, char** argv) {
    if (argc < 3) {
        usage(argv[0]);
    }

    const char* inputFile = argv[1];
    const char* outputFile = argv[2];

    processFile(inputFile, outputFile);

    return 0;
}
