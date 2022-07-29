/*
    -> HW  : 5
    -> NAME: Lefkopoulou Eleni Maria
    -> AEM : 2557
    -> DATE: 20/06/2020
*/

#ifndef _GRAPH_HPP_
#define _GRAPH_HPP_
#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include<iterator>
#include <limits>
#include <math.h>

using namespace std;
//class for negative cycle exception
class NegativeGraphCycle : public std::exception {
public:
  const char* what() const noexcept;
}negative;

const char* NegativeGraphCycle::what() const noexcept {
  return "Negative Graph Cycle!";
}
//struct for edges
template<typename T>
struct Edge {
  T from;
  T to;
  int sortOfTo;
  int dist;
  mutable bool isChecked;
  Edge(T f, T t, int d): from(f), to(t), dist(d) {
  }
  bool operator<(const Edge<T>& e) const;
  bool operator>(const Edge<T>& e) const;
  template<typename U>
  friend std::ostream& operator<<(std::ostream& out, const Edge<U>& e);
};

/////    functions of edge struct    //////
template<typename T>
bool Edge<T>::operator<(const Edge<T>& e) const{
    if(this.dist < e.dist){
        return true;
    }
    else{
        return false;
    }
}
template<typename T>
bool Edge<T>::operator>(const Edge<T>& e) const{
    if(this.dist > e.dist){
        return true;
    }
    else{
        return false;
    }

}
template<typename T>
std::ostream& operator<<(std::ostream& out, const Edge<T>& e) {
  out << e.from << " -- " << e.to << " (" << e.dist << ")";
  return out;
}

//stuct for node
template<typename T>
struct Node {
    T id;
    int sortOfNode;
    int key;
    mutable bool isVisited;
    std::list <Edge<T>> edgesList;

};

//class Graph
template <typename T>
class Graph{
  std::vector <Node<T>> nodes;
  bool isDirectedGraph;
  std::list<T>  dfs_recursion(const T& info,std::list<T> dfsList) const;
  void makeChecked(const T& from, const T& to);
  Edge<T>& TheEdge(Edge<T> E);
  Node<T>& TheNode(T info);
private:
  int NumberOfNOdes;

public:

  Graph(bool isDirectedGraph = true);
  bool contains(const T& info);
  bool addVtx(const T& info);
  bool rmvVtx(const T& info);
  bool addEdg(const T& from, const T& to, int distance);
  bool rmvEdg(const T& from, const T& to);
  std::list<T> dfs(const T& info) const;
  std::list<T> bfs(const T& info) const;
  std::list<Edge<T>> mst();
  bool print2DotFile(const char *filename) const;
  std::list<T> dijkstra(const T& from, const T& to);
  std::list<T> bellman_ford(const T& from, const T& to);

};


//Graph constractor
template <typename T>
Graph<T>::Graph(bool isDirectedGraph){
    this->NumberOfNOdes = 0;
    this->isDirectedGraph = isDirectedGraph;
}
//contains
template <typename T>
bool Graph<T>::contains(const T& info){
    bool return_value = false;
    if(nodes.begin() == nodes.end()){
        //is empty
        return return_value;
    }
    for(auto it = nodes.begin(); it!=nodes.end(); it++){
        if(it->id == info){
            return_value = true;
            break;
        }
    }
    return return_value;

}

//add Vertex
template <typename T>
bool Graph<T>::addVtx(const T& info){
    bool check  = contains(info);
    if(check == true){
        //the node already exist

        return false;
    }
    else{
        //add the node
        struct Node<T> newNode;
        newNode.id = info;
        newNode.sortOfNode = NumberOfNOdes;
        newNode.key = std::numeric_limits<int>::max();
        newNode.isVisited = false;
        nodes.push_back(newNode);
        NumberOfNOdes ++;
        return true;
    }
}

//remove vertex
template <typename T>
bool Graph<T>::rmvVtx(const T& info){
    bool check  = contains(info);
    if(check == false){
        //node doesn't exist

        return false;
    }
    else{
        //we must erase the node
        typename std::vector<Node<T>>::iterator it =  nodes.begin();
        while(it->id != info){
            it ++ ;
        }
        if(isDirectedGraph == false){
            //diatrexei olh th lista akmvn tou kombou pros diagrafh
            for(typename std::list<Edge<T>>::iterator it_edges = it->edgesList.begin(); it_edges!=it->edgesList.end(); it_edges++){
                //psaxnei ton kombo ston opoio exei akmh o kombos
                   rmvEdg(info,it_edges->to);
                   rmvEdg(it_edges->to,info);
            }
           for(unsigned int size =0; size < it->edgesList.size(); size++){
                it->edgesList.pop_back();
            }
            nodes.erase(it);

            return true;
        }
        else{
            typename std::vector<Node<T>>::iterator del_it = nodes.begin();
            for(typename std::vector<Node<T>>::iterator it2 = nodes.begin(); it2!=nodes.end(); it2++){
                if(it2->id != info){
                        rmvEdg(it2->id,info);
                }
                else{
                    auto listSize = it2->edgesList.size();
                    del_it = it2;
                    for(unsigned int size=0; size< listSize; size++){
                        it2->edgesList.pop_back();

                    }
                }
            }
            nodes.erase(del_it);
            return true;
        }
    }
}

//add edge
template <typename T>
bool Graph<T>::addEdg(const T& from, const T& to, int distance){
    bool check;
    //check if from exists
    check = contains(from);
    if(check == false){
        return false;
    }
    //check if to exists
    check = contains(to);
    if(check == false){
        return false;
    }
    //check if the edge exists
    typename std::vector<Node<T>>::iterator it_nodes =  nodes.begin();
    while(it_nodes->id != from){
        it_nodes++;
    }
    bool exist = false;
    typename std::list<Edge<T>>::iterator it_edge =  it_nodes->edgesList.begin();
    for(unsigned int size = 0; size<it_nodes->edgesList.size(); size++ ){
        if(it_edge->to == to){
            exist = true;
            break;
        }
        it_edge++;
    }
    if(exist == true){
        return false;
    }
    else{
        it_edge =  it_nodes->edgesList.begin();
        Edge<T> newEdge(from ,to,distance);
        for(typename std::vector<Node<T>>::iterator it = nodes.begin(); it!=nodes.end(); it++){
            if(it->id == to){
                newEdge.sortOfTo = it->sortOfNode;
                break;
            }
        }
        while((it_edge->sortOfTo < newEdge.sortOfTo) && (it_edge!= it_nodes->edgesList.end())){
            it_edge++;
        }

        it_nodes->edgesList.insert(it_edge,newEdge);

        if(isDirectedGraph == false){
            //add the reverse edge
            typename std::vector<Node<T>>::iterator it_nodes_2 =  nodes.begin();
            while(it_nodes_2->id != to){
                it_nodes_2++;
            }

            it_edge =  it_nodes_2->edgesList.begin();
            Edge<T> newEdge2(to ,from,distance);
            for(typename std::vector<Node<T>>::iterator it = nodes.begin(); it!=nodes.end(); it++){
                if(it->id == from){
                    newEdge2.sortOfTo = it->sortOfNode;
                    break;
                }
            }
            while(( it_edge->sortOfTo < newEdge2.sortOfTo ) && (it_edge!=  it_nodes_2->edgesList.end())){
                it_edge++;
            }

            it_nodes_2->edgesList.insert(it_edge,newEdge2);
            return true;

        }
        else{
            return true;
        }
    }
}

//remove edge
template <typename T>
bool Graph<T>::rmvEdg(const T& from, const T& to){
    bool check;
    //check if from exists
    check = contains(from);
    if(check == false){
        return false;
    }
    //check if to exists
    check = contains(to);
    if(check == false){
        return false;
    }
    typename std::vector<Node<T>>::iterator it_nodes =  nodes.begin();
    while(it_nodes->id != from){
        it_nodes++;
    }
    bool exist = false;
    typename std::list<Edge<T>>::iterator it_edge =  it_nodes->edgesList.begin();
    for(unsigned int size = 0; size<it_nodes->edgesList.size(); size++ ){
        if(it_edge->to == to){
            exist = true;
            break;
        }
        it_edge++;
    }
    if(exist == false){
        return false;
    }
    else{
        it_nodes->edgesList.erase(it_edge);
        if(isDirectedGraph == false){
            it_nodes =  nodes.begin();
            while(it_nodes->id != to){
                it_nodes++;
            }
            it_edge =  it_nodes->edgesList.begin();
            for(unsigned int size = 0; size<it_nodes->edgesList.size(); size++ ){
                if(it_edge->to == from){
                    break;
                }
                it_edge++;
            }
            it_nodes->edgesList.erase(it_edge);
            return true;
        }
        else{
            return true;
        }
    }

}

//dfs algorithm
template <typename T>
std::list<T> Graph<T>::dfs(const T& info) const{
    std::list<T> dfsList;
    for(auto it_nodes =  nodes.begin(); it_nodes!= nodes.end(); it_nodes++){
        it_nodes->isVisited = false;
    }
    auto begin_node =  nodes.begin();
    while(begin_node->id != info){
        begin_node++;
    }
    dfsList = dfs_recursion(begin_node->id,dfsList);
    return dfsList;
}

//dfs recursion function
template <typename T>
std::list<T> Graph<T>::dfs_recursion(const T& info,std::list<T> dfsList) const{

    auto cheking_node =  nodes.begin();
    for(unsigned int size=0; size< nodes.size(); size++ ){
        if(cheking_node->id == info){
            break;
        }
        cheking_node++;
    }
    if(cheking_node->isVisited == true){
        return dfsList;
    }
    else{
        dfsList.push_back(cheking_node->id);
        cheking_node->isVisited = true;
        for(auto it_edges =  cheking_node->edgesList.begin(); it_edges !=cheking_node->edgesList.end(); it_edges++ ){
            dfsList = dfs_recursion(it_edges->to,dfsList);

        }
        return dfsList;
    }

}


//bfs algorithm
template <typename T>
std::list<T> Graph<T>::bfs(const T& info) const{
    std::list<T> bfsList;
    std::list<T> stack;
    for(auto it_nodes =  nodes.begin(); it_nodes!= nodes.end(); it_nodes++){
        it_nodes->isVisited = false;
    }
    auto begin_node =  nodes.begin();
    while(begin_node->id != info){
        begin_node++;
    }
    bfsList.push_back(begin_node->id);
    begin_node->isVisited = true;
    for(auto it_edges =  begin_node->edgesList.begin(); it_edges !=begin_node->edgesList.end(); it_edges++ ){
            stack.push_back(it_edges->to);
    }
    T checkingNode;
    while(!stack.empty()){
        checkingNode = stack.front();
        auto check_node =  nodes.begin();
        while(check_node->id != checkingNode){
            check_node++;
        }
        if(check_node->isVisited == true){
            stack.pop_front();
        }
        else{
            bfsList.push_back(check_node->id);
            check_node->isVisited = true;
            stack.pop_front();
            for(auto it_edges =  check_node->edgesList.begin(); it_edges !=check_node->edgesList.end(); it_edges++ ){
                stack.push_back(it_edges->to);
            }
        }
    }

    return bfsList;
}

//print2DotFile
template <typename T>
bool Graph<T>::print2DotFile(const char *filename) const{
        string fn = (string)filename+".dot";
        const char *filenameNew = fn.c_str();
        ofstream myfile (filenameNew, ios::out | ios::trunc);
        if (!myfile.is_open()) {
            cerr << "Unable to open file " << filename;
            return false;
        }

        if(isDirectedGraph == true){
             myfile << "digraph "<<filename<< "{"<<endl;
             for(auto it = nodes.begin(); it!= nodes.end(); it++){
                for(auto it_edge = it->edgesList.begin(); it_edge != it->edgesList.end(); it_edge++){
                    myfile <<"\t" <<it->id <<" -> " << it_edge->to << " [label = " <<it_edge->dist<<"] ;"<<endl;
                }
             }
        }
        else{

             myfile << "graph "<<filename<< "{"<<endl;
             for(auto it = nodes.begin(); it!= nodes.end(); it++){
                for(auto it_edge = it->edgesList.begin(); it_edge != it->edgesList.end(); it_edge++){
                    if(it_edge->isChecked == false){
                        myfile << "\t"<<it->id << " -- " << it_edge->to << " [label = " <<it_edge->dist<<"] ;"<<endl;
                        it_edge->isChecked = true;
                        auto toCheck_node = nodes.begin();
                        while(toCheck_node->id != it_edge->to){
                                toCheck_node++;
                        }
                        auto toCheck_edge = toCheck_node->edgesList.begin();
                        while(toCheck_edge->to != it_edge->from){
                            toCheck_edge++;
                        }
                        toCheck_edge->isChecked = true;
                    }
                }
             }
        }
        myfile << "}" << endl;
        myfile.close();
        return true;
}


//mst with kruskal algorithm
template <typename T>
std::list<Edge<T>> Graph<T>::mst(){
   //kruskal algorithm
   list<Edge<T>> kruskal_list;
   list<Edge<T>> mst_check;
    if(isDirectedGraph == false){
         //init all edges as unchecked and all nodes as unvisited
        for(auto it_nodes =  nodes.begin(); it_nodes!= nodes.end(); it_nodes++){
                it_nodes->isVisited = false;
                it_nodes->key = -1;
                for(auto itForEdges = it_nodes->edgesList.begin(); itForEdges != it_nodes->edgesList.end(); itForEdges++){
                        itForEdges->isChecked = false;
                }
        }
        //make a list with all edges from the sorthest to biggest
        for(auto it_nodes =  nodes.begin(); it_nodes!= nodes.end(); it_nodes++){
            for(auto itForEdges = it_nodes->edgesList.begin(); itForEdges != it_nodes->edgesList.end(); itForEdges++){
                    if(itForEdges->isChecked == false){
                        auto it_sort = mst_check.begin();
                        while(it_sort->dist <= itForEdges->dist && it_sort != mst_check.end() ){
                            it_sort++;
                        }
                        itForEdges->isChecked = true;
                        makeChecked(itForEdges->to,itForEdges->from);
                        mst_check.insert(it_sort,*itForEdges);

                    }

            }
        }
        //make all edges unchecked to start the algoritm
         for(auto it_nodes =  nodes.begin(); it_nodes!= nodes.end(); it_nodes++){
                for(auto itForEdges = it_nodes->edgesList.begin(); itForEdges != it_nodes->edgesList.end(); itForEdges++){
                        itForEdges->isChecked = false;
                }
        }
        unsigned int VisitedNodes=0;
        bool stop = false;
        while(stop != true){
            //take the sortest edge
            Edge<T> checkingEdge  = mst_check.front();
            mst_check.pop_front();
            Node<T>& NodeFrom  = TheNode(checkingEdge.from);
            Node<T>& NodeTo = TheNode(checkingEdge.to);
            if(NodeFrom.isVisited == false && NodeTo.isVisited == false){
                NodeFrom.key  =NodeFrom.sortOfNode;
                NodeFrom.isVisited = true;
                VisitedNodes++;
                NodeTo.key = NodeFrom.sortOfNode;
                NodeTo.isVisited = true;
                VisitedNodes++;
                kruskal_list.push_back(checkingEdge);

            }
            else if(NodeFrom.isVisited == true && NodeTo.isVisited == true){
                if(NodeFrom.key  == NodeTo.key){
                    //cycle ...dont put the edge at sssp
                }
                else{
                    int KeyForChange = NodeTo.key;
                    NodeTo.key = NodeFrom.key;
                    for(auto it = nodes.begin(); it!= nodes.end(); it++){
                        if(it->key == KeyForChange){
                                it->key = NodeFrom.key;
                        }
                    }
                    kruskal_list.push_back(checkingEdge);
                }

            }
            else{

                if(NodeFrom.isVisited== false){
                    NodeFrom.isVisited = true;
                    VisitedNodes++;
                    NodeFrom.key = NodeTo.key;

                }
                if(NodeTo.isVisited== false){
                    NodeTo.isVisited = true;
                    VisitedNodes++;
                    NodeTo.key = NodeFrom.key;
                }

                kruskal_list.push_back(checkingEdge);

            }
            if(VisitedNodes == nodes.size()){
                bool ok = true;
                Node<T> first = nodes.front();

                for(auto it_ok = nodes.begin(); it_ok != nodes.end(); it_ok++){
                        if(it_ok->key  != first.key){
                             ok = false;
                        }
                }
                if(ok == true){
                    stop = true;
                }
            }
        }
        for(auto final_mst = kruskal_list.begin(); final_mst!= kruskal_list.end(); final_mst++){
            if(TheNode(final_mst->from).sortOfNode > TheNode(final_mst->to).sortOfNode){
                T helping =     final_mst->from;
                final_mst->from = final_mst->to;
                final_mst->to = helping;
            }
        }
        return kruskal_list;
    }
    else{
         //return an empty list
         return list<Edge<T>>();
    }

}

//make an edge checked
template <typename T>
void Graph<T>::makeChecked(const T& from, const T& to){
    typename std::vector<Node<T>>::iterator it_nodes =  nodes.begin();
    while(it_nodes->id != from){
        it_nodes++;
    }
    typename std::list<Edge<T>>::iterator it_edge =  it_nodes->edgesList.begin();
    for(unsigned int size = 0; size<it_nodes->edgesList.size(); size++ ){
        if(it_edge->to == to){
            it_edge->isChecked = true;
            break;
        }
        it_edge++;
    }
}

//return a reference from a node
template <typename T>
Node<T>& Graph<T>::TheNode(T info){
    auto it_nodes = nodes.begin();
    while(it_nodes->id != info){
        it_nodes++;
    }
    return *it_nodes;
}

//return a reference from an edge
template <typename T>
Edge<T>& Graph<T>::TheEdge(Edge<T> E){
    auto it_nodes = nodes.begin();
    while(it_nodes->id != E.from){
        it_nodes++;
    }
    auto it_edges = it_nodes->edgesList.begin();
    while(it_edges->to != E.to){
        it_edges++;
    }
    return *it_edges;
}

//dijkstra algorithm
template <typename T>
std::list<T> Graph<T>::dijkstra(const T& from, const T& to){
     bool negative_edge = false;
     int total = 0;
    //list to return
    list<T> dijkstra_list;
    //list of  edges which are in the ssspath
    list<Edge<T>> sssp_list;
    //helping queue
    list<Edge<T>> queue;
    //init all nodes as unvisited and all edges as unchecked
    for(auto it_nodes =  nodes.begin(); it_nodes!= nodes.end(); it_nodes++){
            it_nodes->isVisited = false;
            it_nodes->key = std::numeric_limits<int>::max();
            for(auto itForEdges = it_nodes->edgesList.begin(); itForEdges != it_nodes->edgesList.end(); itForEdges++){
                    itForEdges->isChecked = false;
            }
    }
    Node<T>& theStart = TheNode(from);
    theStart.key = 0;
    for(auto it = theStart.edgesList.begin(); it!= theStart.edgesList.end(); it++){
            auto it_q1 = queue.begin();
            while((it_q1 != queue.end()) &&((theStart.key+it_q1->dist)<(theStart.key+it->dist))){
                it_q1++;
            }
            queue.insert(it_q1,*it);
    }
    while(!queue.empty()){
        Edge<T> testEdge = queue.front();
        queue.pop_front();
        Edge<T>& theRealEdge = TheEdge(testEdge);
        if(theRealEdge.isChecked == false){
            Node<T>& theRealTo = TheNode(testEdge.to);
            if(TheNode(testEdge.from).key + testEdge.dist <= theRealTo.key  && testEdge.to != from){
                theRealTo.key = TheNode(testEdge.from).key + testEdge.dist;
                if(theRealTo.isVisited == true ){
                    auto it_sssp = sssp_list.begin();
                    while((it_sssp->to != theRealTo.id) && (it_sssp != sssp_list.end())){
                            it_sssp++;
                    }
                    sssp_list.erase(it_sssp);
                    it_sssp->isChecked = false;
                    sssp_list.push_back(testEdge);
                    TheEdge(testEdge).isChecked = true;

                }
                else{
                    theRealTo.isVisited = true;
                    sssp_list.push_back(testEdge);
                    if(theRealTo.id == to){
                        break;
                    }
                    for(auto it = theRealTo.edgesList.begin(); it != theRealTo.edgesList.end(); it++){
                        auto it_q2 = queue.begin();
                        while((it_q2 != queue.end()) && ((TheNode(it_q2->from).key+it_q2->dist) < (TheNode(it->from).key+it->dist)) ){
                            it_q2++;
                        }
                        queue.insert(it_q2,*it);
                    }
                }

            }
        }
    }

            auto iter  = queue.begin();
            while(iter->to != to && iter != queue.end()){
                iter ++;
            }
            if(iter->to == to){
                if(TheNode(iter->from).key+ iter->dist <= TheNode(to).key ){
                    auto it_sssp2 = sssp_list.begin();
                    while((it_sssp2->to != to) && (it_sssp2 != sssp_list.end())){
                            it_sssp2++;
                    }
                    sssp_list.erase(it_sssp2);
                    it_sssp2->isChecked = false;
                    sssp_list.push_back(*iter);
                    TheEdge(*iter).isChecked = true;
                }
            }

T info = to;
dijkstra_list.push_front(info);
for(unsigned int i = 0; i<= sssp_list.size(); i++){
    auto it_s = sssp_list.end();
    it_s--;
    auto it_ss = sssp_list.begin();
    it_ss--;
    while(it_s->to != info && it_s != it_ss){
        it_s--;
    }
    if(it_s->to == info){
        total = total+it_s->dist;
        if(it_s->dist < 0 ){
           negative_edge = true;
        }

            if(it_s->from == from){
                 dijkstra_list.push_front(from);
                 if(negative_edge == true && total<=0){
                    dijkstra_list = bellman_ford(from,to);
                 }
                 return dijkstra_list;
            }
            else{
                dijkstra_list.push_front(it_s->from);
                info = it_s->from;
            }

    }
    else{
        return  list<T>();;
    }

}
if(negative_edge == true && total<=0){
    dijkstra_list = bellman_ford(from,to);
}
return dijkstra_list;
}


//bellman_ford algorithm
template <typename T>
std::list<T> Graph<T>::bellman_ford(const T& from, const T& to){
    //init all distances from source to nodes as infinity
     for(auto it_nodes =  nodes.begin(); it_nodes!= nodes.end(); it_nodes++){
        it_nodes->isVisited = false;
        it_nodes->key = std::numeric_limits<int>::max();
        for(auto itForEdges = it_nodes->edgesList.begin(); itForEdges != it_nodes->edgesList.end(); itForEdges++){
                itForEdges->isChecked = false;
        }
    }
    //check at every iteration if there are changes
    bool changes = false;
    //list with path from the node from to node to
    list<T> bellman_ford_list;
    //list fir the edges which are into the sssp
    list<Edge<T>> sssp_list;
    //node which is the source for our graph
    Node<T>& startNode = TheNode(from);
    startNode.key = 0;
    startNode.isVisited = true;
    //has the location of source
    auto it_start = nodes.begin();
    while(it_start->id != from){
        it_start++;
    }
    //Relax all edges V-1 times
    for(unsigned int stages = 1; stages < nodes.size(); stages++){
        //at every iteration make isChecked = false at the begin
        for(auto iter =  nodes.begin(); iter!= nodes.end(); iter++){
            for(auto iterEdges = iter->edgesList.begin(); iterEdges != iter->edgesList.end(); iterEdges++){
                    iterEdges->isChecked = false;
            }
        }
        changes = false;
        for(auto it =it_start; it!=nodes.end(); it++ ){
            for(auto it_edge = it->edgesList.begin(); it_edge != it->edgesList.end(); it_edge++){
                if(it_edge->to != from){//we want the source has 0
                    if(it_edge->isChecked == false){
                        Node<T>& theRealNode = TheNode(it_edge->to);
                        if(theRealNode.isVisited == true){
                                  if((it->key != std::numeric_limits<int>::max()) && (theRealNode.key > (it->key+it_edge->dist))){

                                    auto sssp_it = sssp_list.begin();
                                    while(sssp_it->to != theRealNode.id){
                                        sssp_it++;
                                    }
                                    if(sssp_it->from == it_edge->from){
                                         theRealNode.key = it->key+it_edge->dist;
                                         changes = true;
                                    }
                                    else{
                                            theRealNode.key = it->key+it_edge->dist;
                                            changes = true;
                                            sssp_list.erase(sssp_it);
                                            it_edge->isChecked = false;
                                            sssp_list.push_back(*it_edge);
                                            it_edge->isChecked = true;
                                        }
                            }
                        }
                        else{
                            if((it->key != std::numeric_limits<int>::max()) && (theRealNode.key > (it->key+it_edge->dist))){
                                theRealNode.isVisited = true;
                                theRealNode.key = it->key+it_edge->dist;
                                changes = true;
                                sssp_list.push_back(*it_edge);
                                it_edge->isChecked = true;
                            }
                        }
                    }
                }
            }
        }
        for(auto it =nodes.begin(); it!=it_start; it++ ){
            for(auto it_edge = it->edgesList.begin(); it_edge != it->edgesList.end(); it_edge++){
                if(it_edge->to != from){//we want the source has 0
                    if(it_edge->isChecked == false){
                        Node<T>& theRealNode = TheNode(it_edge->to);
                        if(theRealNode.isVisited == true){

                             if((it->key != std::numeric_limits<int>::max()) && (theRealNode.key > (it->key+it_edge->dist))){

                                    auto sssp_it = sssp_list.begin();
                                    while(sssp_it->to != theRealNode.id){
                                        sssp_it++;
                                    }
                                    if(sssp_it->from == it_edge->from){
                                         theRealNode.key = it->key+it_edge->dist;
                                         changes = true;
                                    }
                                    else{

                                            theRealNode.key = it->key+it_edge->dist;
                                            changes = true;
                                            sssp_list.erase(sssp_it);
                                            it_edge->isChecked = false;
                                            sssp_list.push_back(*it_edge);
                                            it_edge->isChecked = true;
                                    }
                            }
                        }
                        else{
                            if((it->key != std::numeric_limits<int>::max()) && (theRealNode.key > (it->key+it_edge->dist))){
                                theRealNode.isVisited = true;
                                theRealNode.key = it->key+it_edge->dist;
                                changes = true;
                                sssp_list.push_back(*it_edge);
                                it_edge->isChecked = true;
                            }
                        }
                    }
                }
            }
        }

        if(changes == false){
            break;
        }

    }
    if(changes == true){
        //exei arnhtiko kuklo
       try{
             throw negative;
        }
        catch(const NegativeGraphCycle &ex){
            cout <<  ex.what() ;
        }

        return  list<T>();

    }
    else{
        T info = to;
        bellman_ford_list.push_front(info);
        for(unsigned int i = 0; i< sssp_list.size(); i++){
            auto it_s = sssp_list.end();
            it_s--;
            auto it_ss = sssp_list.begin();
            it_ss--;
            while(it_s->to != info && it_s != it_ss){
                it_s--;
            }
            if(it_s->to == info){
                if(it_s->from == from){
                     bellman_ford_list.push_front(from);
                     break;
                }
                else{
                    bellman_ford_list.push_front(it_s->from);
                    info = it_s->from;
                }
            }
            else{
                return  list<T>();
            }

        }
        return bellman_ford_list;
    }

}
#endif
