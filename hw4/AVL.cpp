/*
    >HW  : 4
    >NAME: Lefkopoulou Eleni Maria
    >AEM : 2557
    >DATE: 10/05/2020
*/
#include "AVL.hpp"
#include <cstdlib>
#include <cmath>
#include <stack>
using namespace std;
    //Node constractor
    AVL::Node::Node(const string& e, AVL::Node *parent, AVL::Node *left, AVL::Node *right){
        element = e;
        this->parent = parent;
        this->left = left;
        this->right = right;
    }
    //Node getParent()
    AVL::Node*  AVL::Node::getParent() const{
        return this->parent;
    }
    //Node getLeft
    AVL::Node*  AVL::Node::getLeft() const{
        return this->left;
    }
    //Node getRight
    AVL::Node*  AVL::Node::getRight() const{
        return this->right;
    }
    //Node getElement
    string AVL::Node::getElement() const{
        return this->element;
    }
    //Node getHeight
    int AVL::Node::getHeight() const{
        return this->height;
    }
    //Node setLeft
    void AVL::Node::setLeft(AVL::Node *leftNode){
        left = leftNode;
    }
    //Node setRight
    void AVL::Node::setRight(AVL::Node *rightNode){
        right = rightNode;
    }
    //Node setParent
    void AVL::Node::setParent(AVL::Node *parentNode){
        parent = parentNode;
    }
    //Node setElement
    void AVL::Node::setElement(string e){
        element = e;
    }
    //Node isLeft
    bool AVL::Node::isLeft() const{
        if(parent == nullptr){
            //is the root of the tree
            return false;
        }
        else{
            //every element can be added only one time so it is unique in tree
            if(this->parent->left!= nullptr){
                if(this->parent->left->element == this->element){
                    return true;
                }
                else{
                    return false;
                }
            }
            else{
                return false;
            }
        }
    }
    //Node isRight
    bool AVL::Node::isRight() const{
      if(parent == NULL){
          //is the root of the tree
            return false;
        }
        else{
          //every element can be added only one time so it is unique in tree
            if(this->parent->right != nullptr){
                if(this->parent->right->element == this->element){
                    return true;
                }
                else{
                    return false;
                }
            }
            else{
                return false;
            }
        }
    }
    //Node rightChildHeight
    int  AVL::Node::rightChildHeight() const{
        if(right != nullptr){
            return right->getHeight();
        }
        else{
            return 0;
        }
    }
    //Node leftChildHeight
    int  AVL::Node::leftChildHeight() const{
         if(left != nullptr){
            return left->getHeight();
        }
        else{
            return 0;
        }
    }
    //Node updateHeight
    int  AVL::Node::updateHeight(){
        int rightHeight ;
        int leftHeight;
        rightHeight = rightChildHeight();
        leftHeight = leftChildHeight();
        height = max(rightHeight,leftHeight) +1;
        return 0;
    }
    //Node isBalanced
    bool AVL::Node::isBalanced(){
        int rightHeight ;
        int leftHeight;
        rightHeight = rightChildHeight();
        leftHeight = leftChildHeight();
        if(fabs(rightHeight -leftHeight)<= 1){
            return true;
        }
        else{
            return false;
        }
    }
    //AVL constractor
    AVL::AVL(){
        size = 0;
        root = nullptr;
    }
    //AVL copy constractor
    AVL::AVL(const AVL& r){
      size = 0;
      root = nullptr;
      for(Iterator it = r.begin(); it != r.end(); it++) {

        this->add(it.getCurrent()->getElement());
      }
    }
    //AVL contains
    bool AVL::contains(string e){
        bool check = false;
        if(root == nullptr && size == 0){
            return false;
        }
        check = search(root,e);
        return check;
    }
    //AVL search
    bool AVL::search(AVL::Node* startNode,string element){
        bool check= false;
        Node* checkNode;
        if(element == startNode->getElement()){
            return true ;
        }
        else{
            if(element > startNode->getElement()){
                if(startNode->getRight() !=nullptr){
                    checkNode = startNode->getRight();
                    check = search(checkNode,element);
                }
                else{
                    return false;
                }
            }
            else{
                if(startNode->getLeft() != nullptr){
                    checkNode = startNode->getLeft();
                    check = search(checkNode,element);
                }
                else{
                    return false;
                }

            }
        }
        if(check == false){
            return false ;
        }
        else{
            return true;
        }
    }
    //AVL add
    bool AVL::add(string e){
        bool exist;
        exist = contains(e);
        if(exist == false){
            if(root==nullptr && size==0){
                Node* newNode = new (nothrow) Node(e,nullptr,nullptr,nullptr);
                if(newNode == nullptr){
                    cerr<<"error"<<endl;
                    exit(1);
                }
                newNode->updateHeight();
                root = newNode;
                size++;
                return true;
            }
            else{
                Node* nodeForAdd = insert(root,e);
                Node* newNode = new (nothrow) Node(e,nodeForAdd,nullptr,nullptr);
                if(newNode == nullptr){
                    cerr<<"error"<<endl;
                    exit(1);

                }
                newNode->updateHeight();
                if(e>nodeForAdd->getElement()){
                    nodeForAdd->setRight(newNode);
                }
                else{
                    nodeForAdd->setLeft(newNode);
                }

                size++;
                //rebalance
                rebalance(nodeForAdd);
                return true;
            }
        }
        else{
            return false;
        }
    }
    //AVL insert
    AVL::Node* AVL::insert(AVL::Node* checkForAdd, string addElement){
        Node* checkNode;
        Node* returnNode;
        if(addElement>checkForAdd->getElement()){
            if(checkForAdd->getRight() != NULL){
                checkNode = checkForAdd->getRight();
                returnNode = insert(checkNode,addElement);
                return returnNode;
            }
            else{
                return checkForAdd;
            }
        }
        else{
            if(checkForAdd->getLeft() != NULL){
                checkNode = checkForAdd->getLeft();
                returnNode = insert(checkNode,addElement);
                return returnNode;
            }
            else{
                return checkForAdd;
            }
        }
    }
    //AVL rebalanceSon
    AVL::Node* AVL::rebalanceSon(AVL::Node* parentNode){
        if(parentNode == nullptr){
            return nullptr;
        }
        int leftheight,rightheight;
        if(parentNode->getLeft() != nullptr ){
            leftheight=parentNode->leftChildHeight();
        }
        else{
            leftheight =0;
        }
        if(parentNode->getRight() != nullptr ){
            rightheight=parentNode->rightChildHeight();
        }
        else{
            rightheight =0;
        }
        if( leftheight> rightheight){
            return parentNode->getLeft();
        }
        else{
            if(leftheight < rightheight){
                return parentNode->getRight();
            }
            else{
                if(parentNode->isLeft()){
                    return parentNode->getLeft();
                }
                else{if(parentNode->isRight()){
                        return parentNode->getRight();
                    }

                }
            }
        }
        return nullptr;
    }
    //AVL rebalance
    void AVL::rebalance(AVL::Node* NodeForBalance){
        if(NodeForBalance == nullptr){
            return ;
        }
        else{
            Node* v;
            Node* u;
            Node* w;
            v=NodeForBalance;
            while(v != nullptr){
                v->updateHeight();
                if(v->isBalanced() == false){
                    w = rebalanceSon(v);
                    u = rebalanceSon(w);
                    v = reconstruct(v,w,u);
                    v->getLeft()->updateHeight();
                    v->getRight()->updateHeight();
                    v->updateHeight();
                }
                v = v->getParent();
            }
        }
    }
    //AVL reconstruct
    AVL::Node* AVL::reconstruct(AVL::Node* v,AVL::Node* w,AVL::Node* u){
        bool isroot;
        //simple right rotation
        if(w->isLeft() && u->isLeft()){
            isroot =true;
            if(v->getParent() != nullptr){
                isroot = false;
                if(v->isLeft()){
                    v->getParent()->setLeft(w);
                }
                else{
                    v->getParent()->setRight(w);
                }
                w->setParent(v->getParent());
            }
            v->setLeft(w->getRight());
            if(w->getRight() != nullptr){
                w->getRight()->setParent(v);
            }
            w->setRight(v);
            v->setParent(w);
            if(isroot == true){
                w->setParent(nullptr);
                root = w;
            }
            return w;
        }

        //simple left rotation
        if(w->isRight() && u->isRight() ){
            isroot =true;
            if(v->getParent() != nullptr){
                isroot = false;
                if(v->isRight()){
                    v->getParent()->setRight(w);
                }
                else{
                    v->getParent()->setLeft(w);
                }
                w->setParent(v->getParent());
            }
            v->setRight(w->getLeft());
            if(w->getLeft() != nullptr){
                w->getLeft()->setParent(v);
            }
            w->setLeft(v);
            v->setParent(w);
            if(isroot == true){
                w->setParent(nullptr);
                root = w;
            }
            return w;
        }

        //double right rotation
        if(u->isLeft()){
            isroot = true;
            v->setRight(u->getLeft());
            if(u->getLeft() != nullptr){
                u->getLeft()->setParent(v);
            }
            w->setLeft(u->getRight());
            if(u->getRight() != nullptr){
                u->getRight()->setParent(w);
            }
            if(v->getParent() != nullptr){
                isroot = false;
                if(v->isRight()){
                    v->getParent()->setRight(u);
                }
                else{
                    v->getParent()->setLeft(u);
                }
                u->setParent(v->getParent());
            }
            v->setParent(u);
            w->setParent(u);
            u->setLeft(v);
            u->setRight(w);
            if(isroot == true){
                u->setParent(nullptr);
                root =u;
            }
            return u;
        }
        else{
            isroot = true;
            v->setLeft(u->getRight());
            if(u->getRight() != nullptr){
                u->getRight()->setParent(v);
            }
            w->setRight(u->getLeft());
            if(u->getLeft() != nullptr){
                u->getLeft()->setParent(w);
            }
            if(v->getParent() != nullptr){
                isroot =false;
                if(v->isLeft()){
                    v->getParent()->setLeft(u);
                }
                else{
                    v->getParent()->setRight(u);
                }
                u->setParent(v->getParent());
            }
            v->setParent(u);
            w->setParent(u);
            u->setLeft(w);
            u->setRight(v);
            if(isroot == true){
                u->setParent(nullptr);
                root=u;
            }
            return u;
        }
    }
    //AVL rmv
    bool AVL::rmv(string e){
        bool exist;
        exist = contains(e);
        Node* delnode;
        Node* nodeForRebalance;
        if(exist == true){
            delnode = findDelNode(e,root);
        //if he has not children at all
            if((delnode->getLeft() == nullptr)&&(delnode->getRight() == nullptr)){
                //if he is not the root
                if(delnode->getParent() != nullptr){
                    if(delnode->isLeft()){
                        delnode->getParent()->setLeft(nullptr);
                        nodeForRebalance = delnode->getParent();
                        delete(delnode);
                        rebalance(nodeForRebalance);
                        size--;
                        return true;
                    }
                    else{
                        delnode->getParent()->setRight(nullptr);
                        nodeForRebalance = delnode->getParent();
                        delete(delnode);
                        rebalance(nodeForRebalance);
                        size--;
                        return true;
                    }
                }
                else{
                    //is the root
                    delete(delnode);
                    size--;
                    root = nullptr;

                    return true;
                }
            }
           //if he has only left child
            if((delnode->getRight() == nullptr)&&(delnode->getLeft() !=nullptr)){
                if(delnode->getParent() != nullptr){
                    if(delnode->isLeft()){
                        delnode->getParent()->setLeft(delnode->getLeft());
                        delnode->getLeft()->setParent(delnode->getParent());
                        nodeForRebalance = delnode->getParent();
                        delete(delnode);
                        rebalance(nodeForRebalance);
                        size--;
                        return true;
                    }
                    else{
                        delnode->getParent()->setRight(delnode->getLeft());
                        delnode->getLeft()->setParent(delnode->getParent());
                        nodeForRebalance = delnode->getParent();
                        delete(delnode);
                        rebalance(nodeForRebalance);
                        size--;
                        return true;
                    }
                }
                else{
                    //is root
                    delnode->getLeft()->setParent(nullptr);
                    root = delnode->getLeft();
                    nodeForRebalance = root;
                    delete(delnode);
                    rebalance(nodeForRebalance);
                    size--;
                    return true;
                }
            }
            //if he has only right child
             if((delnode->getRight() != nullptr)&&(delnode->getLeft() ==nullptr)){
                //check if delnode is the root
                if(delnode->getParent() != nullptr){
                    if(delnode->isLeft()){
                        delnode->getParent()->setLeft(delnode->getRight());
                        delnode->getRight()->setParent(delnode->getParent());
                        nodeForRebalance = delnode->getParent();
                        delete(delnode);
                        rebalance(nodeForRebalance);
                        size--;
                        return true;
                    }
                    else{
                        delnode->getParent()->setRight(delnode->getRight());
                        delnode->getRight()->setParent(delnode->getParent());
                        nodeForRebalance = delnode->getParent();
                        delete(delnode);
                        rebalance(nodeForRebalance);
                        size--;
                        return true;
                    }
                }
                else{
                //is root
                    delnode->getRight()->setParent(nullptr);
                    root = delnode->getRight();
                    nodeForRebalance = root;
                    delete(delnode);
                    rebalance(nodeForRebalance);
                    size--;
                    return true;
                }

            }
            //if he has 2 children
            if((delnode->getRight() != nullptr)&&(delnode->getLeft() !=nullptr)){
                Node* moveNode= findTheMostLeftNode(delnode->getRight());
                delnode->setElement(moveNode->getElement());
                moveNode->setElement(e);
                if(moveNode->getRight()!= nullptr){
                  if(moveNode->getParent() != nullptr){
                    if(moveNode->isLeft()){
                        moveNode->getParent()->setLeft(moveNode->getRight());
                        moveNode->getRight()->setParent(moveNode->getParent());
                        nodeForRebalance = moveNode->getParent();
                        delete(moveNode);
                        rebalance(nodeForRebalance);
                        size--;
                        return true;
                    }
                    else{
                        moveNode->getParent()->setRight(moveNode->getRight());
                        moveNode->getRight()->setParent(moveNode->getParent());
                        nodeForRebalance = moveNode->getParent();
                        delete(moveNode);
                        rebalance(nodeForRebalance);
                        size--;
                        return true;
                    }
                    }
                    else{
                    //is root
                        moveNode->getRight()->setParent(nullptr);
                        root = moveNode->getRight();
                        nodeForRebalance = root;
                        delete(moveNode);
                        rebalance(nodeForRebalance);
                        size--;
                        return true;
                    }

                }
                else{
                    if(moveNode->isRight()){
                        moveNode->getParent()->setRight(nullptr);
                    }
                    else{
                        if(moveNode->isLeft()){
                            moveNode->getParent()->setLeft(nullptr);
                        }
                    }
                    nodeForRebalance = moveNode->getParent();
                    delete(moveNode);
                    rebalance(nodeForRebalance);
                    size--;
                    return true;
                }
            }
        }
           return false;
    }
    //AVL findDelNode
    AVL::Node* AVL::findDelNode(string element,AVL::Node* startNode){
        Node* delnode ;
        Node* checkNode = nullptr;
        if(element == startNode->getElement()){
            delnode = startNode;
            return delnode ;
        }
        else{
            if(element > startNode->getElement()){
                if(startNode->getRight() !=NULL){
                    checkNode = startNode->getRight();
                    delnode = findDelNode(element,checkNode);
                }
            }
            else{
                if(startNode->getLeft() != NULL){
                    checkNode = startNode->getLeft();
                    delnode = findDelNode(element,checkNode);
                }
            }
        }
        return delnode ;
    }
    //AVL findTheMostLeftNode
    AVL::Node* AVL::findTheMostLeftNode(AVL::Node* givenNode){
        Node* returnNode = givenNode;
        while (returnNode->getLeft() != nullptr){
            returnNode = returnNode->getLeft();
        }
        return returnNode;
    }
    //AVL print2DotFile
    void AVL::print2DotFile(char *filename){
        ofstream myfile (filename, ios::out | ios::trunc);
        if (!myfile.is_open()) {
            cout << "Unable to open file " << filename;
            exit(1);
        }
        int nullcounter=0;
        myfile << "digraph BST {"<<endl;
        for(Iterator it = begin(); it != end(); it++) {
            if (it.getCurrent()->getLeft()!= nullptr){
                myfile<<it.getCurrent()->getElement()<<" -> "<<it.getCurrent()->getLeft()->getElement()<<";"<<endl;
            }
            else{
                myfile<<"null"<<nullcounter <<" [shape=point];"<<endl;
                myfile<<it.getCurrent()->getElement()<<" -> null"<<nullcounter <<";"<<endl;
                nullcounter++;
            }
            if (it.getCurrent()->getRight()!= nullptr){
                myfile<<it.getCurrent()->getElement()<<" -> "<<it.getCurrent()->getRight()->getElement()<<";"<<endl;
            }
            else{
                myfile<<"null"<<nullcounter <<" [shape=point];"<<endl;
                myfile<<it.getCurrent()->getElement()<<" -> null"<<nullcounter <<";"<<endl;
                nullcounter++;
            }
        }
        myfile << "}" << endl;
        myfile.close();

    }
    //AVL pre_order
    void AVL::pre_order(std::ostream& out){
        for(Iterator it = begin(); it != end(); it++) {
          out << it.getCurrent()->getElement() ;
          out<<" ";
        }

    }
    //AVL operator<<
    ostream& operator<<(ostream& out, const AVL& tree){
        AVL avl;
        if(tree.size!=0){
            for(AVL::Iterator it = tree.begin(); it != tree.end(); it++) {
              out << it.getCurrent()->getElement() ;
              out<<" ";
            }

        }
        return out;
    }
    //Iterator constractor
    AVL::Iterator::Iterator(AVL::Node* rootnode){
        stack<Node*> reversePreorder;
        endnode = new (nothrow) Node("THIS_IS_THE_END",endnode,endnode,endnode);
        if(endnode == nullptr){
            cerr<<"error at new"<<endl;
            exit(1);
        }
        reversePreorder=preorderFunction(reversePreorder,rootnode);
        PreorderStack.push(endnode);
        while(!reversePreorder.empty()){
            PreorderStack.push(reversePreorder.top());
            reversePreorder.pop();
        }
        current = PreorderStack.top();
    }
    //Iterator copyconstractor
    AVL::Iterator::Iterator(const AVL::Iterator &it){
        endnode = new (nothrow) Node("THIS_IS_THE_END",endnode,endnode,endnode);
        if(endnode == nullptr){
            cerr<<"error at new"<<endl;
            exit(1);
        }
        this->current = it.getCurrent();
        this->PreorderStack = it.getStack();
    }
    //Iterator getEndnode
    AVL::Node* AVL::Iterator::getEndnode()const{
        return this->endnode;
    }
    //Iterator getCurrent
    AVL::Node* AVL::Iterator::getCurrent()const{
        return this->current;
    }
    //Iterator getStack
    stack<AVL::Node*> AVL::Iterator::getStack()const{
        return this->PreorderStack;
    }
    //Iterator setEndnode
    void AVL::Iterator::setEndnode(Node* node){
        this->endnode = node;
    }
    //Iterator setCurrent
    void AVL::Iterator::setCurrent(Node* node){
        this->current = node;
    }
    //Iterator preorderFunction
    stack<AVL::Node*>  AVL::Iterator::preorderFunction(stack<AVL::Node*> reversePreorder,AVL::Node* node){
        Node* nodeForPush;
        if(node== nullptr){
            return reversePreorder;
        }
        else{
            nodeForPush = node;
            reversePreorder.push(nodeForPush);
            if(node->getLeft()){
                nodeForPush=node->getLeft();
                reversePreorder = preorderFunction(reversePreorder,nodeForPush);
            }
            if(node->getRight()){
                nodeForPush=node->getRight();
                reversePreorder = preorderFunction(reversePreorder,nodeForPush);
            }
            return reversePreorder;
        }
    }
    //Iterator operator++
    AVL::Iterator& AVL::Iterator::operator++(){
        if(current!= endnode){
            PreorderStack.pop();
            current = PreorderStack.top();
        }
        return *this;
    }
    //Iterator operator++(inta)
    AVL::Iterator AVL::Iterator::operator++(int a){
        Iterator returnIterator(*this);
        if(this->current!= this->endnode){
            this->PreorderStack.pop();
            this->current = PreorderStack.top();
        }
        return returnIterator;
    }
    //Iterator operator*
    string AVL::Iterator::operator*(){
        return getCurrent()->getElement();
    }
    bool AVL::Iterator::operator!=(AVL::Iterator it){
        if(this->getCurrent()->getElement() == it.getCurrent()->getElement() ){

            return false;
        }
        else{
            return true;
        }

    }
    //Iterator operator==
    bool AVL::Iterator::operator==(AVL::Iterator it){
        if(this->getCurrent()->getElement() == it.getCurrent()->getElement()){
            return true;
        }
        else{
            return false;
        }
    }
    //AVL begin
     AVL::Iterator AVL::begin() const{
         Iterator startIterator(root);
         return startIterator;
     }
    //AVL end
     AVL::Iterator AVL::end() const{

         Iterator endIterator(root);
         endIterator.setCurrent(endIterator.getEndnode());
         return endIterator;
     }
    AVL& AVL::operator  =(const AVL& avl){
        while(size!=0){
            rmv(root->getElement());
        }
        for(Iterator it = avl.begin(); it != avl.end(); it++){
            add(it.getCurrent()->getElement());
        }

        return *this;

    }
    AVL  AVL::operator  +(const AVL& avl){
        AVL newAvl;
        newAvl = *this;
        for(Iterator it = avl.begin(); it != avl.end(); it++){
            newAvl.add(it.getCurrent()->getElement());
        }
        return newAvl;
    }
    AVL& AVL::operator +=(const AVL& avl){
        for(Iterator it = avl.begin(); it != avl.end(); it++){
            add(it.getCurrent()->getElement());
        }
        return *this;
    }
    AVL& AVL::operator +=(const string& e){
        add(e);
        return *this;
    }
    AVL& AVL::operator -=(const string& e){
        rmv(e);
        return *this;
    }
    AVL  AVL::operator  +(const string& e){
        AVL newAvl = AVL();
        newAvl = *this;
        newAvl.add(e);
        return newAvl;
    }
    AVL  AVL::operator  -(const string& e){
        AVL newAvl = AVL();
        newAvl = *this;
        newAvl.rmv(e);
        return newAvl;
    }
