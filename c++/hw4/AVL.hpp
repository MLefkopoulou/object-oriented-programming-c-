/*
    >HW  : 4
    >NAME: Lefkopoulou Eleni Maria
    >AEM : 2557
    >DATE: 10/05/2020
*/
#ifndef __AVL_HPP_
#define __AVL_HPP_

#include <iostream>
#include <fstream>
#include <stack>
using namespace std;

class AVL {
private:
  class Node {
    Node *parent, *left, *right;
    int height;
    string element;

  public:
    Node(const string& e, Node *parent, Node *left, Node *right);

    Node*  getParent() const;
    Node*  getLeft() const;
    Node*  getRight() const;
    string getElement() const;
    int    getHeight() const;

    void setLeft(Node *);
    void setRight(Node *);
    void setParent(Node *);
    void setElement(string e);

    bool isLeft() const;
    bool isRight() const;
    int  rightChildHeight() const;
    int  leftChildHeight() const;
    int  updateHeight();
    bool isBalanced();
  };
private:
    int   size;
    Node* root;


public:

    class Iterator {
    private:
        Node* current;
        Node* endnode;
        stack <Node*> PreorderStack;

        stack<Node*> preorderFunction(stack<Node*> reversePreorder,Node* node);
    public:
      Iterator(Node* root);
      Iterator(const Iterator &it);
      Node* getEndnode()const;
      Node* getCurrent()const;
      void setEndnode(Node* node);
      void setCurrent(Node* node);
      stack<Node*> getStack()const;
      Iterator& operator++();
      Iterator operator++(int a);
      string operator*();
      bool operator!=(Iterator it);
      bool operator==(Iterator it);
    };

  Iterator begin() const;
  Iterator end() const;

  static const int MAX_HEIGHT_DIFF = 1;
  AVL();
  AVL(const AVL&);
  bool contains(string e);
  bool search(AVL::Node* startNode,string element);
  bool add(string e);
  Node* insert(Node* checkForAdd, string addElement);
  Node* rebalanceSon(Node* parentNode);
  void rebalance(Node* NodeForRebalance);
  Node* reconstruct(Node* v,Node* w,Node* u);
  bool rmv(string e);
  Node* findDelNode(string element,Node* startNode);
  Node* findTheMostLeftNode(Node* givenNode);
  void print2DotFile(char *filename);
  void pre_order(std::ostream& out);
  friend std::ostream& operator<<(std::ostream& out, const AVL& tree);
  AVL& operator  =(const AVL& avl);
  AVL  operator  +(const AVL& avl);
  AVL& operator +=(const AVL& avl);
  AVL& operator +=(const string& e);
  AVL& operator -=(const string& e);
  AVL  operator  +(const string& e);
  AVL  operator  -(const string& e);
};

#endif

