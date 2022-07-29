/*
    -> HW  : 5
    -> NAME: Lefkopoulou Eleni Maria
    -> AEM : 2557
    -> DATE: 20/06/2020
*/

#include "Graph.hpp"
#ifndef _GRAPH_UI_
#define _GRAPH_UI_
using namespace std;
template <typename T>
int graphUI() {

  string option, line;
  //int distance;
  bool digraph = false;

  cin >> option;
  if(!option.compare("digraph"))
    digraph = true;
  Graph<T> g(digraph);

  while(true) {

    std::stringstream stream;
    cin >> option;

    if(!option.compare("av")) {
      getline(std::cin, line);
      stream << line;
      T vtx(stream);
      if(g.addVtx(vtx))
        cout << "av " << vtx << " OK\n";
      else
        cout << "av " << vtx << " NOK\n";
    }
    else if(!option.compare("rv")) {
        getline(std::cin, line);
        stream << line;
        T vtx(stream);
        if(g.rmvVtx(vtx))
            cout << "rv " << vtx << " OK\n";
        else
            cout << "rv " << vtx << " NOK\n";
    }
    else if(!option.compare("ae")) {
        getline(std::cin, line);
        stream << line;
        T from(stream);
        T to(stream);
        int distance ;
         stream>>distance;
        if(g.addEdg(from,to,distance))
            cout << "ae " << from<<" "<<to << " OK\n";
        else
            cout << "ae " << from<<" "<<to << " NOK\n";
    }
    else if(!option.compare("re")) {
        getline(std::cin, line);
        stream << line;
        T from(stream);
        T to(stream);
        if(g.rmvEdg(from,to))
            cout << "re " << from<<" "<<to << " OK\n";
        else
            cout << "re " << from<<" "<<to << " NOK\n";

    }
    else if(!option.compare("dot")) {
        getline(std::cin, line);
        string filenameString(line);
        //filenameString = filenameString+".dot";
        const char* filename = filenameString.c_str() ;
        bool check = g.print2DotFile(filename);
        if(check == true){
            cerr << "dot "<<filename<<" OK\n";
        }
        else{
            cerr << "dot "<<filename<<" NOK\n";
        }
    }
    else if(!option.compare("bfs")) {
      getline(std::cin, line);
      stream << line;
      T info(stream);
      std::list<T> bfs_list = g.bfs(info);
      cout << "\n----- BFS Traversal -----\n";
      auto iter = bfs_list.begin();
        while(iter!= bfs_list.end()){
           cout<<*iter;
           iter++;
           if(iter == bfs_list.end()){

            }
            else{
                cout<<" -> ";
            }
        }

      cout << "\n-------------------------\n";
    }
    else if(!option.compare("dfs")) {
        getline(std::cin, line);
        stream << line;
        T info(stream);
        std::list<T> dfs_list = g.dfs(info);
        cout << "\n----- DFS Traversal -----\n";
        auto iter = dfs_list.begin();
        while(iter!= dfs_list.end()){
           cout<<*iter;
           iter++;
           if(iter == dfs_list.end()){

            }
            else{
                cout<<" -> ";
            }
        }

      cout << "\n-------------------------\n";
    }
    else if(!option.compare("dijkstra")){

        getline(std::cin, line);
        stream << line;
        T from(stream);
        T to(stream);
        cout << "Dijkstra (" << from << " - " << to <<"): ";
        list<T> dijkstra_list= g.dijkstra(from,to);



      for(auto it =dijkstra_list.begin(); it != dijkstra_list.end(); it++ ){
            cout<<*it;
            auto check = it;
            check++;
            if(check == dijkstra_list.end()){

            }
            else{
                cout <<", ";
            }
      }
        cout <<endl;

    }
    else if(!option.compare("bellman-ford")) {
        getline(std::cin, line);
        stream << line;
        T from(stream);
        T to(stream);

        cout << "Bellman-Ford (" << from << " - " << to <<"): ";
        list<T> bellman_ford_list = g.bellman_ford(from,to);

        for(auto it =bellman_ford_list.begin(); it != bellman_ford_list.end(); it++ ){
            cout<<*it;
            auto check2 = it;
            check2++;
            if(check2 == bellman_ford_list.end()){

            }
            else{
                cout <<", ";
            }
        }
        cout <<endl;
    }
    else if(!option.compare("mst")) {
        list<Edge<T>> mst_list = g.mst();
        int sum = 0;;
        cout << "\n--- Min Spanning Tree ---\n";
        for(auto it = mst_list.begin(); it!= mst_list.end(); it++){
        cout<<*it<<endl;
        sum = sum + it->dist;
        }
        cout << "MST Cost: " << sum << endl;
    }
    else if(!option.compare("q")) {
        cerr << "bye bye...\n";
        return 0;
    }
    else if(!option.compare("#")) {
        string line;
        getline(cin,line);
        cerr << "Skipping line: " << line << endl;
    }
    else {
      cout << "INPUT ERROR\n";
      return -1;
    }
  }
  return -1;
}

#endif
