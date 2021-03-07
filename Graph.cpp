#include "Graph.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "queue" 
#include "stack"
#include "unordered_map"
using namespace std;

/*
 *  Constructs a graph by parsing a Vertex file to initialize verticies,
 *  then by parsing a Edge file to initialize edges, and finally by parsing
 *  a Categories file to initialize categories of each vertex. 
 */
Graph::Graph() {
}


/*
 *  Parse vertex data file and initialize vertices' names for each data entry
 *  Data is of type "# ArticleName" 
 */
void Graph::parseVertices(std::string filename){
    if(!fileExists(filename)){
        std::cout << "File doesn't seem to exist. Double check your directory." << std::endl;
        abort();
    }

    /* Initialize file IO variables */
    std::ifstream fileVertex(filename);
    std::string line;

    /* Initialize variables for progress percent calculations */
    const unsigned total_lines = linesInFile(filename);   // Total # of lines/verticies to be read from file
    int parsed_ct = 0;                                    // Tracks how many lines/verticies have been read so far
    int last_perc = 0;                                    // Variable that helps detect when progress percentage has changed
    int perc = 0;                                         // Percentage of lines/verticies that have been read so far

    /* Parse one line at a time */
    while(getline(fileVertex, line)) {
        perc = (parsed_ct*100)/total_lines;

        /* If progress percentage has changed, update output and last percentage variable */
        progUpdate("verticies", perc, last_perc);

        /* Copy substring of line after " " and set as vertex name */
        numToVertex.push_back(Vertex(line.substr(line.find(" ") + 1), parsed_ct)); // new Vertex constructor, name and id
        parsed_ct++;
    }
}

/*
 *  Parse edge data file and initialize neighbors list for each data entry
 *  Data is of type "tailVertex# headVertex#" (tail points to head)
 */
void Graph::parseEdges(std::string filename){
    if(!fileExists(filename)){
        std::cout << "File doesn't seem to exist. Double check your directory." << std::endl;
        abort();
    }

    /* Initialize file IO variables */
    std::ifstream fileEdge(filename);
    std::string line;

    /* Initialize variables for progress percent calculations */
    const unsigned total_lines = linesInFile(filename);  // Total # of lines/edges to be read from file
    int parsed_ct = 0;                                   // Tracks how many lines/edges have been read so far
    int last_perc = 0;                                   // Variable that helps detect when progress percentage has changed
    int perc = 0;                                        // Percentage of lines/edges that have been read so far

    /* Parse one line at a time */
    while(getline(fileEdge, line)) {
        perc = (parsed_ct*100)/total_lines;

        /* If progress percentage has changed, update output and last percentage variable */
        progUpdate("edges", perc, last_perc);

        /* Scan line for index of " ", use indexSpace to divide line into two substrings "tailVertex", "headVertex" */
        int indexSpace = line.find(" ");
        int tailVertex = std::stoi(line.substr(0,indexSpace));
        int headVertex = std::stoi(line.substr(indexSpace));

        numToVertex[tailVertex].neighbors.push_back(headVertex);
        numToVertex[headVertex].in_neighbors.push_back(tailVertex);
        parsed_ct++;
    }
}

/*
 *  Parse categories data file and initialize categories list for each data entry
 *  Data is of type "Category:CategoryName; VertexA_1, VertexA_2, ... , VertexA_n"
 *  Vertices are listed in increasing order but not all are included in every category
 */
void Graph::parseCategories(std::string filename){
    if(!fileExists(filename)){
        std::cout << "File doesn't seem to exist. Double check your directory." << std::endl;
        abort();
    }

    /* Initialize file IO variables */
    std::ifstream fileCat(filename);
    std::string line;
    std::string category;
    
    /* Initialize variables for progress percent calculations */
    const int total_lines = linesInFile(filename);      // Total # of lines/categories to be read from file
    int categoryIndex = 1;                              // Tracks category index (index represents line in file)
    int last_perc = 0;                                  // Variable that helps detect when progress percentage has changed
    int perc = 0;                                       // Percentage of lines/categories that have been read so far
    int vertex;

    numToCategory.push_back("NULL");

    while(getline(fileCat, line)) {
        perc = (categoryIndex*100)/total_lines;

        /* If progress percentage has changed, update output and last percentage variable */
        progUpdate("categories", perc, last_perc);

        line = line.substr(line.find(":")+1);
        numToCategory.push_back(line.substr(0,line.find(";")));

        line = line.substr(line.find(" ") + 1); // Truncate line after the " " i.e. "1 2 3" -> "2 3"

        /* While there are still spaces left, continue parsing line */
        while(int(line.find(" ")) > -1) {
            try {
                vertex = std::stoi(line.substr(0, line.find(" ")));
                numToVertex[vertex].categories.push_back(categoryIndex);

                line = line.substr(line.find(" ") + 1); // Truncate line after the " "   
            } 
            catch (exception e) {
                break; // catches on categories with no articles
            }
        }

        /* Catch last number in line (last number doesnt have a " " after it) */
        try{
            vertex = std::stoi(line.substr(0, line.find("\n")));
            numToVertex[vertex].categories.push_back(categoryIndex);
        } 
        catch (exception e){} // catches on categories with no articles

        categoryIndex++;
    }  
}

/*
 *  Rudimentary print function that lists vertices in order
 *  followed by their neighbors in order.
 */
void Graph::printGraph(){
    /* Print verticies */
    for(unsigned i = 0; i < 1000; i++){
        std::cout << "Vertex: " << numToVertex[i].name << "\nNeighbors: ";

        /* Print neighbors */
        for(int j : numToVertex[i].neighbors){
            std::cout << numToVertex[j].name << ", ";
        }
        
        /* Print categories */
        std::cout << "\nCategories: ";
        for(int k : numToVertex[i].categories){
            std::cout << k << ", ";
        }
        std::cout << "\n\n";
    }
}

void Graph::printNumToName() {
    for(unsigned int i = 0; i < numToVertex.size(); i++) {
        std::cout << "Index i: " << i << " " << numToVertex[i].name << std::endl;
    }
}

std::list<int> Graph::list_neighbors(int v) {
    return numToVertex[v].neighbors;
}
std::list<int> Graph::list_categories(int v) {
    return numToVertex[v].categories;
}

vector<int> Graph::BFS(int search_id, int start_id) {
    queue<vector<int>> q;
    unordered_map<int, bool> discovered;

    discovered[start_id] = true;
    q.push(vector<int>(1, start_id)); // start of search, node 0 by default. path of size 1

    while (!q.empty()) {
        vector<int> curr_path = q.front();
        int v_id = curr_path.back();
        q.pop();
        if (v_id == search_id) {
            return curr_path;
        }
        for (int n : numToVertex[v_id].neighbors) {
            if (!discovered[n]) {
                discovered[n] = true;
                vector<int> new_path(curr_path);
                new_path.push_back(n);
                q.push(new_path);
            }
        }
    }
    vector<int> e;
    e.push_back(-1);
    return e;
}

void Graph::findCycle(int start_id){
    // Cycle Detection with motified BFS
    std::cout << "Finding Cycle for chosen article..." << std::endl;
    queue<vector<int>> q;
    unordered_map<int, bool> discovered;

    bool found = false; // indicator for cycle detection
    vector<int> curr_path; // cycle path
    int v_id; // variable to check for starting article 

    discovered[start_id] = true;

    q.push(vector<int>(1, start_id)); // start of search, node 0 by default. path of size 1
    while (!q.empty()) {
        curr_path = q.front();
        v_id = curr_path.back(); // save starting ID
        q.pop();
        for (int n : numToVertex[v_id].neighbors) { // parse thru neighbors that haven't been discovers 
            if (!discovered[n]) {
                discovered[n] = true;
                vector<int> new_path(curr_path);
                new_path.push_back(n);
                q.push(new_path);
            }
            else if(n == start_id){
                curr_path.push_back(n);
                found = true;
                break;
            }
        }
        if(found == true){
            break;
        }
    }

    if (v_id == start_id){found = true;} // mark cycle indicator true

    int size_of_cycle = curr_path.size() - 1; // size of cycle path
    int arrows = size_of_cycle;

    if(found){ // if cycle found, print cycle with size
        std::cout << "Size of Cycle Path: " << size_of_cycle << std::endl;
        for (int v : curr_path) {
            std::cout << v << " " << numToVertex[v].name;
            if(arrows > 0){std::cout << " -> ";} // arrows for graph visual
            arrows--;
        }
        std::cout << std::endl;
    }
    else{
        std::cout << "Sorry, Cycle does not Exist!" << std::endl;
    }
    
}

unordered_map<int, list<int>> Graph::KosarajuSCC() {
    // KOSARAJU's ALGORITHM
    std::cout << "Starting Kosaraju's algorithm" << std::endl;
    std::cout << "Visiting all nodes..." << std::endl;
    for (unsigned v_id = 0 ; v_id < numToVertex.size(); v_id++) {
        visit(v_id);
    }
    std::cout << "Placing into components..." << std::endl;
    while (!finished.empty()) {
        int top = finished.top();
        finished.pop();
        assign(top, top); 
    }

    // Taking the output of the alg (a map of id->root of the components) and making a vector of vectors to enumerate them
    std::cout << "Saving into map..." << std::endl;
    unordered_map<int, list<int>> return_map;
    for (std::pair<int, int> id_pair : components) {
        int v_id = id_pair.first;
        int root = id_pair.second;
        unordered_map<int, list<int>>::iterator f = return_map.find(root);
        if (f == return_map.end()) {
            list<int> component;
            component.push_back(v_id);
            return_map[root] = component;
        } else {
            (*f).second.push_back(v_id);
        }
    }
    return return_map;
}

void Graph::visit(int v_id) {
    if (numToVertex[v_id].visited == false) {
        std::stack<int> to_visit; // uses DFS to visit all possible neighbors of each node.
        to_visit.push(v_id);
        std::unordered_map<int, bool> seen;
        while (!to_visit.empty()) {
            int t_id = to_visit.top();
            to_visit.pop();
            numToVertex[t_id].visited = true;
            if (!seen[t_id]) {
                to_visit.push(t_id);
                seen[t_id] = true;
            } else {
                finished.push(t_id);
            }
            for (int n_id : numToVertex[t_id].neighbors) {
                if (!numToVertex[n_id].visited) {
                    to_visit.push(n_id);
                }
            }
        }
    }
}

void Graph::assign(int v_id, int root) {
    if (components.find(v_id) == components.end()) { // v_id has not been assigned component
        std::stack<int> to_visit; // uses DFS to visit all possible neighbors of each node.
        to_visit.push(v_id);
        while (!to_visit.empty()) {
            int t_id = to_visit.top();
            to_visit.pop();
            numToVertex[t_id].visited = false; // reusing same flag in opposite way :)
            components[t_id] = root; // assign root to all DFS children of the original
            for (int n_id : numToVertex[t_id].in_neighbors) {
                if (numToVertex[n_id].visited) {
                    to_visit.push(n_id);
                }
            }
        }
    }
}

void Graph::progUpdate(std::string type, int &percent, int &last_percent){
    if (percent != last_percent) {
            cerr.flush();
            cerr << "Parsing " << type << ": " << percent << "% complete" << string(30, ' ') << "\r";
            last_percent = percent;
        }
}

unsigned Graph::linesInFile(std::string filename){
    unsigned lines = 0;
    std::ifstream fileEdge(filename);
    std::string line;

    cerr.flush();
    cerr << "Getting number of lines in file " << filename << "...\r";
    while(getline(fileEdge, line)) {
        lines++;
    } return lines;
}

bool Graph::fileExists(std::string filename){
    ifstream f(filename);
    return f.good();
<<<<<<< HEAD
}
=======
}
>>>>>>> 623fa989864b3515018b8fbd3b49dbdcfc1135bb
