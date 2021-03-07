#include <fstream>
#include <vector>
#include <string>
#include "Graph.h"
#include "Vertex.h"
using namespace std;

void userInputGraph(Graph* g);
void printName(Graph* g);
void printNeighbors(Graph* g);
void printCategories(Graph* g);
void BFS(Graph* g);
void cycleDetection(Graph* g);
void landmark(Graph* g);
void runKosaraju(Graph& g);
void printHelp();
bool fileExists(string filename);

int main () {
    Graph g;
    string input;

    cout << "Please enter the vertices file:" << std::endl;
    cin >> input;
    g.parseVertices(input);

    cout << "Please enter the edges file:" << std::endl;
    cin >> input;
    g.parseEdges(input);

    cout << "Please enter the categories file:" << std::endl;
    cin >> input;
    g.parseCategories(input);
   
    bool cont = true;

    while(cont){
        string input;
        cout << "What would you like to do next?" << endl;
        cin >> input;

        if(input == "pn"){
            printName(&g);
        }
        else if(input == "pN"){
            printNeighbors(&g);
        }
        else if(input == "pc"){
            printCategories(&g);
        }
        else if(input == "bfs"){
            BFS(&g);
        }
        else if(input == "cd"){
            cycleDetection(&g);
        }
        else if(input == "l"){
            landmark(&g);
        }
        else if(input == "scc"){
            runKosaraju(g);
        }
        else if(input == "help"){
            printHelp();
        }
        else if(input == "end" || input == "q"){
            cont = false;
        }
        else{
            cout << "input not recognized, please try again - or type 'help' for help" << endl;
        }
    }

    return 0;
}

void printName(Graph* g){
    unsigned aid;
    cout << "\n     Print Name\n";
    cout << "====================\n";
    cout << "ID of article: ";
    cin >> aid;
    if(aid >= g->numToVertex.size()) {
        cout << "out of bounds. Enter smaller number" << "\n";
        printName(g);
    } else {
        cout << g->numToVertex[aid].name << "\n";
    }
}

void printNeighbors(Graph* g){
    unsigned id;
    cout << "\n   Print Neighbors\n";
    cout << "====================\n";
    cout << "ID of article: ";
    cin >> id;
    cout << "article name: " << g->numToVertex[id].name << "\n";
    list<int> n = g->list_neighbors(id);
    cout << "neighbors are: " << std::endl;
    for (list<int>::iterator it = n.begin(); it != n.end(); it++) {
        cout << *it << " " << g->numToVertex[*it].name << endl;
    }
}

void printCategories(Graph* g){
        int id;
    cout << "\n Print Categories\n";
    cout << "====================\n";
    cout << "ID of article: ";
    cin >> id;
    cout << "article name: " << g->numToVertex[id].name << "\n";
    list<int> n = g->list_categories(id);
    list<int>::iterator it;  // testing neigbors of vertices
    cout << "categories are: " << std::endl;
    for (it = n.begin(); it != n.end(); it++) {
        cout << *it << " " << g->numToCategory[*it] << endl;
    }
}

void BFS(Graph* g){
    int aid;
    int bid;
    cout << "\n        BFS\n";
    cout << "====================\n";
    cout << "ID of first article: ";
    cin >> aid;
    cout << g->numToVertex[aid].name << "\n\n";
    cout << "ID of second article: ";
    cin >> bid;
    cout << g->numToVertex[bid].name << "\n\n";
    cout << "Starting BFS..." << endl;
    vector<int> path = g->BFS(bid, aid);
    cout << "size of path: " << path.size() << endl;
    cout << "\npath:\n";
    for (int v : path) {
        cout << v << " " << g->numToVertex[v].name << endl;
    }
    cout << "\n";
}

void cycleDetection(Graph* g){
    int aid;
    cout << "\n   Cycle Detection\n";
    cout << "====================\n";
    cout << "ID of first article: ";
    cin >> aid;
    cout << g->numToVertex[aid].name << "\n";
    g->findCycle(aid);
}

void landmark(Graph* g){
    int aid, bid, cid;
    cout << "\n      Landmark\n";
    cout << "====================\n";
    cout << "ID of first article: ";
    cin >> aid;
    cout << g->numToVertex[aid].name << "\n\n";
    cout << "ID of second article: ";
    cin >> bid;
    cout << g->numToVertex[bid].name << "\n\n";
    cout << "ID of third article: ";
    cin >> cid;
    cout << g->numToVertex[cid].name << "\n\n";
    cout << "Starting BFS..." << endl;
    vector<int> path1 = g->BFS(bid, aid);
    vector<int> path2 = g->BFS(cid, bid);
    cout << "size of path: " << path1.size() + path1.size() << endl;
    cout << "\npath:\n";
    for (int v : path1) {
        cout << v << " " << g->numToVertex[v].name << endl;
    }
    cout << "\n";
    for (int v : path2) {
        cout << v << " " << g->numToVertex[v].name << endl;
    }
    cout << "\n";
}

void runKosaraju(Graph& g){
    cout << "\n Enumerate strongly connected components\n";
    cout << "====================\n";
    unordered_map<int, list<int>> components = g.KosarajuSCC();
    std::cout << "# of strongly connected components: " << components.size() << std::endl;
}

void printHelp(){
    cout << "pn - Print name" << endl;
    cout << "pN - Print neighbor" << endl;
    cout << "pc - Print categories" << endl;
    cout << "bfs - Breadth first search" << endl;
    cout << "cd - Cycle detection" << endl;
    cout << "l - Landmark algorithm" << endl;
    cout << "scc - Strongly connected component enumeration" << endl;
    cout << "end/q - Terminate program" << endl;
    cout << "help - Display help" << endl;
}

bool fileExists(string filename){
    ifstream infile(filename);
    return infile.good();
}