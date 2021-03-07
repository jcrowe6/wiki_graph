#include "Vertex.h"
#include <iostream>
#include <vector>
#include "unordered_map"
#include "stack"
#include <list>

using namespace std;
class Graph {
    private:
        /*
         * Helper function for updating terminal output of progress percentage
         * Only prints new output when necessary
         */
        void progUpdate(std::string type, int &percent, int &last_percent);
        
        /*
         * Helper function for counting the number of lines in a file.
         * This number is then compared against to calculate the pecentage of a file
         * that has been read (progress percentage).
         */
        unsigned linesInFile(std::string filename);

        /*
         * Helper function for asserting a file typed in by the user exists.
         * This does NOT asser that the file is correctly formatted, just that
         * the file exists.
         */
        bool fileExists(std::string filename);

    public:
        // these two vectors are for file reading purposes
        std::vector<Vertex> numToVertex; // i = 0 gives vertex, which can tell you name, neighbors, etc..

        /*
         * Helper function for parsing vertice file.
         * Takes argument filename, opens file with name filename,
         * parses and loads data into numToVertex and shows progression %
         * during runtime.
         */
        void parseVertices(std::string filename);

        /*
         * Helper function for parsing edges file.
         * Takes argument filename, opens file with name filename,
         * parses and loads data into numToVertex.neighbors and shows
         * progression % during runtime.
         */
        void parseEdges(std::string filename);

        /*
         * Helper function for parsing categories file.
         * Takes argument filename, opens file with name filename,
         * parses and loads data into numToVertex.categories and shows
         * progression % during runtime.
         */
        void parseCategories(std::string filename);
        
        /*
        * i = 0 gives "Buprestoidea"    i = 1 gives "People_from_Worcester" and so on
        * indexed based on order of category name
        */
        std::vector<std::string> numToCategory;

        /*
         * Default graph constructor
         */
        Graph();

        /*
         * Graph constructor that take 2 arguments: vertexFile and edgeFile
         * This allows for runtime user input of the graph files
         */
        Graph(string vertexFile, string edgeFile);

        /*
         * Graph constructor that take 3 arguments: vertexFile, edgeFile, and categoryFile
         * This allows for runtime user input of the graph files
         */
        Graph(string vertexFile, string edgeFile, string categoryFile);


        /*
         * Rudimentary print function that lists every vertice (including its neighbors
         * and categories) in order. Not ideal for large data sets
         */
        void printGraph();
        
        /*
         * prints contents in numtoname array debugging purposes 
         */
        void printNumToName();

        /*
         * neighbors for each vertex (connected articles)
         */
        std::list<int> list_neighbors(int v); 

        /*
         * categories that each article may be in
         */
        std::list<int> list_categories(int v);

        /*
         * BFS, searches for Vertex id, optional second argument for the id
         * of the vertex to start the search from
         */
        std::vector<int> BFS(int search_id, int start_id = 0); 

        /*
         * finds all strongly connected components of the graph,
         * returns them in a map of lists each representing a component
         * uses Kosaraju's algorithm
         */
        std::unordered_map<int, std::list<int>> KosarajuSCC(); 

        /*
         * Helper function using in KosarajuSCC() to mark verticies has visited in a DFS search
         */
        void visit(int u);

        void assign(int u, int root);
        
        /*
         * findCycle uses BFS in order to find a cycle path from a user defined article ID,
         * it prints will print out a visualization of the path in the terminal.
         * The first and last article should be same, 
        */
        void findCycle(int start_id);

        int visited_ct = 0;

        /*
         * Variables used in KosarajuSCC()
         */
        std::stack<int> finished;
        std::unordered_map<int, int> components;
};
