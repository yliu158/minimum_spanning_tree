// name: Yang Liu
// email: yliu158@binghamton.edu
// b#: B00481024
#include <iostream> // cout cin
#include <vector> // heap_v pre forest etc
#include <iomanip> // setw print in certain format
#include <algorithm> // sort array
#include <sstream>
#include <fstream>
#include <string>

using namespace std;

vector<vector<int> > graph; // graph
vector<pair<int, int> > heap_v; // all edges, first:vertice second:length
vector<int> pre; // pre vertice of each vertice
vector<int> forest; // parent of each vertice for kruskal
struct edge{
  int length;
  int v;
  int u;
  bool operator<(const edge & r) const {return length < r.length;}
};
vector<edge> weight; // sort edge based on lenght in ascending order
vector<edge> solution; // solution of kruskal


//declaration of functions
void execute(); // all executable lines
void getGraphFromFile();
void prepareGraph(); // prepare the graph
void printGraph(); // print graph using list

void prim(); // Minimum spanning tree using prim algorithm
void miniHeapify_Prim(const int i, int n); // miniHeapify_Prim the cost
pair<int, int> removeMini_Prim(); // remove the minimum vertice from the heap
void printCost_Prim(); // print heap of current status
void printSolution_Prim(); // print final solution

void kruskal(); // minimum Spanning tree using kruskal algorithm
int find_Kruskal(int vertice); // find the root of given vertice
void union_Kruskal(int v, int u); // union two separate graphs
void printEdges_Kruskal(); // print all edges in ascending order
void printSolution_Kruskal(); // print solution in ascending order

int main() {
  execute();
  return 0;
}

// implementation of functions
void execute() {
  srand(time(NULL));
  string input = "";
  bool sign = true;
  while (sign) {
    prepareGraph();

    // getGraphFromFile();

    string algo = "";
    cout << "Press 'p' to use prim algorithm." << endl;
    cout << "Press 'k' to use kruskal algorithm." << endl;
    cin >> algo;
    if (algo[0] == 'p') {
      prim();
    } else if (algo[0] == 'k') {
      kruskal();
    } else {
      while (algo[0] != 'p' && algo[0] != 'k') {
        cout << "Error: Only p or k algorithm can be chose." << endl;
        cout << "Press 'p' to use prim algorithm." << endl;
        cout << "Press 'k' to use kruskal algorithm." << endl;
        cin.clear();
        cin >> algo;
        while (cin.fail()) {
          cin.clear();
          cin.ignore();
          cout << "Error: Only p or k algorithm can be chose." << endl;
          cout << "Press 'p' to use prim algorithm." << endl;
          cout << "Press 'k' to use kruskal algorithm." << endl;
          cin >> algo;
        }
      }
      if (algo[0] == 'p') {
        prim();
      } else if (algo[0] == 'k') {
        kruskal();
      }
    }

    // prim();
    // kruskal();

    cout << "Press any other button to continue." << endl;
    cout << "Press e to Exit." << endl;
    cin >> input;
    if (input[0] == 'e' || input[0] == 'E') {
      sign = false;
    }

    while (cin.fail()) {
      cin.clear();
      cin.ignore();
      cin >> input;
      if (input[0] == 'e' || input[0] == 'E') {
         sign = false;
      }
    }
  }
  cout << "Successfully Exited" << endl;
}

void getGraphFromFile() {
  graph.clear();
  ifstream file("graph.txt");
  string str;
  while (getline(file, str)) {
    istringstream ss(str);
    string num;
    // cout << str << endl;
    vector<int> line;
    while (getline(ss, num, ' ')) {
      if (num != " ") {
        line.push_back(stoi(num));
      }
    }
    graph.push_back(line);
  }
  printGraph();
}

// get graph prepared
void prepareGraph(){
  int size = rand()%6+5;
  graph.clear();
  graph = vector<vector<int> >(size, vector<int>(size));
  pre.clear();
  pre = vector<int>();

  for (int i = 0; i < size; i++) {
    pre.push_back(i);
    for (int j = 0; j <= i; j++) {
      if (i == j) {
        graph.at(i).at(j) = 0;
      } else {
        graph.at(i).at(j) = rand()%10+1;
        graph.at(j).at(i) = graph.at(i).at(j);
      }
    }
  }
  printGraph();
}

// print the graph
void printGraph() {
  cout << "Graph.\n";
  for (int i = 0; i < graph.size(); i++){
    for (int j = 0; j < graph.at(i).size(); j++) {
      cout << setw(3) << setfill(' ')<< graph.at(i).at(j);
    }
    cout << endl;
  }
}

// Minimum Spanning Tree
void prim() {
  heap_v = vector<pair<int, int> >(graph.size(), pair<int, int>(0, INT_MAX));
  int root = rand()%graph.size();
  for (int i = 0; i < heap_v.size(); i ++) {
    if (i == root) {
      heap_v.at(i).second = 0;
    }
    heap_v.at(i).first = i;
  }
  for (int i = heap_v.size()/2; i >= 0; i--) {
    miniHeapify_Prim(i, heap_v.size());
  }

  // for each v not in the tree
  while (heap_v.size() != 0) {
    // printCost_Prim();
    pair<int, int> v = removeMini_Prim();
    // for each v not in the tree
    for (int i = 0; i < heap_v.size(); i++) {
      int w = graph.at(heap_v.at(i).first).at(v.first);
      if(heap_v.at(i).second > w) {
        // update the shorest distance to the graph
        heap_v.at(i).second = w;
        pre.at(heap_v.at(i).first) = v.first;
        // decrease key
        pair<int, int> temp = heap_v.at(i);
        heap_v.erase(heap_v.begin()+i);
        heap_v.insert(heap_v.begin(), temp);
        miniHeapify_Prim(0, heap_v.size());
      }
    }
  }
  printSolution_Prim();
}

// miniHeapify_Prim cost
void miniHeapify_Prim(const int i, int n) {
  if (i >= n) return;
  int smallest = i;
  int left = i*2+1;
  int right = i*2+2;

  if (left < n  && heap_v.at(left).second < heap_v.at(i).second) {
    smallest = left;
  }
  if (right < n && heap_v.at(right).second < heap_v.at(smallest).second) {
    smallest = right;
  }
  if (smallest != i) {
    pair<int, int> temp = heap_v.at(i);
    heap_v.at(i) = heap_v.at(smallest);
    heap_v.at(smallest) = temp;
    miniHeapify_Prim(smallest, n);
  }
}

pair<int, int> removeMini_Prim() {
  if (heap_v.size() == 0) return make_pair<int, int>(0, -1);

  pair<int, int> mini = heap_v.at(0);
  heap_v.at(0) = heap_v.at(heap_v.size()-1);
  heap_v.pop_back();
  miniHeapify_Prim(0, heap_v.size());
  return mini;
}

// print cost
void printCost_Prim() {
  cout << "Heap: " << endl;
  for (int i = 0; i < heap_v.size(); i++) {
    cout << "vertice: " << heap_v.at(i).first;
    cout << "  cost: " << heap_v.at(i).second << endl;
  }
  cout << endl;
}

// print prim Solution
void printSolution_Prim() {
  cout << "Prim Solution: " << endl;
  for (int i = 0; i < pre.size(); i++) {
    cout << "v: " << i << "    pre: "<< pre.at(i);
    if (pre.at(i) != i) {
      cout << "    length: " << graph.at(i).at(pre.at(i)) << endl;
    } else {
      cout << "    length: 0 (ROOT)" << endl;
    }
  }
}

// kruskal algorithm
void kruskal(){
  solution.clear();
  weight.clear();
  struct edge edges[graph.size()*graph.size()];
  int count = 0;
  for (int i = 0; i < graph.size(); i++) {
    for (int j = 0; j < i; j++) {
      struct edge e;
      e.length = graph.at(i).at(j);
      e.v = i;
      e.u = j;
      edges[count] = e;
      count++;
    }
  }

  sort(edges, edges+count);
  for (int i = 0; i < count; i++) {
    weight.push_back(edges[i]);
  }
  // setup the parent of each vertice
  forest.clear();
  for (int i = 0; i < graph.size(); i++ ) {
    forest.push_back(i);
  }

  int shortest = 0;
  while (shortest < weight.size()){
    int v = weight.at(shortest).v;
    int u = weight.at(shortest).u;
    if (find_Kruskal(v) != find_Kruskal(u)) {
      struct edge e;
      e.length = weight.at(shortest).length;
      e.v = v;
      e.u = u;
      solution.push_back(e);
      union_Kruskal(v, u);
      // if (solution.size() >= graph.size()-1) break;
    }
    shortest++;
    // cout << "shortest " << shortest <<"  v: " << v << "   u:" << u << endl;
    // for (int i = 0; i < forest.size(); i++ ) {
    //   cout << "forest v:" << i << " "<< forest.at(i) << endl;
    // }
    // cout << endl;
  }
  printSolution_Kruskal();
}

// find the root of given vertice
int find_Kruskal(int vertice) {
  int x = vertice;
  while (forest.at(x) != x) {
    x = forest.at(x);
  }
  return x;
}

// union two graphs
void union_Kruskal(int v, int u) {
  int s = min(find_Kruskal(v), find_Kruskal(u));
  int l = max(find_Kruskal(v), find_Kruskal(u));
  forest.at(l) = s;
}

// Kruskal print Edges
void printEdges_Kruskal() {
  cout << "Edges: " << endl;
  for (int i = 0; i < weight.size(); i++) {
    cout << "length:" << weight.at(i).length;
    cout << "   v:" << weight.at(i).v;
    cout << "   u:" << weight.at(i).u<< endl;
  }

  cout << "=======================total:" << weight.size();
  cout << "===============================" << endl;
}

// print final solution
void printSolution_Kruskal() {
  cout << "Kruskal Solution: " << endl;
  for (int i = 0; i < solution.size(); i++) {
    cout << "Edge   v: " << solution.at(i).v;
    cout << "   u: " << solution.at(i).u;
    cout << "   length: " << solution.at(i).length << endl;
  }
  cout << "=======================total:" << solution.size();
  cout << "===============================" << endl;
}
