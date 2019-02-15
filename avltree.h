#ifndef AVLTREE_H
#define AVLTREE_H
#include <iostream>
#include <string>
#include <stdexcept>
#include <iomanip>
#include <fstream>
#include "IndexInterface.h"

template <typename T,typename K> class Node;
template <typename T,typename K> class AVLTree;

template <typename T, typename K>
class Node{

friend class AVLTree<T,K>;

private:
    T data;
    Node<T,K>* left;
    Node<T,K>* right;
    int height;
    K key;
public:
    /* Node is the default constructor of the friend class.
    * @return Node<T,k>
    */
    Node(T d, K k);
};

//constructor for node
template <typename T,typename K>
Node<T,K>::Node(T d, K k)
{
    data = d;
    left = nullptr;
    right = nullptr;
    height = 0;
    key = k;
}

template <typename T,typename K>
class AVLTree: public IndexInterface<T,K> {

public:
    /* AVLTree is the default constructor of the subclass class.
    * @return AVLTree<T,k>
    */
    AVLTree();
    /* AVLTree is the copy constructor of the sub class
     * @param rightObj is the object we are copying to the initialized object.
    * @return AVLTree<T,k>
    */
    AVLTree(const AVLTree<T,K>& rightObject);
    /* Destructor, deletes dynamic memory.
    * @return AVLTree<T,K>
    */
    virtual ~AVLTree();
    /* operator= is the overloaded assignment operator
     * @param rightObj is the object we are copying to the object left of (=).
    * @return AVLTree<T,k>&
    */
    AVLTree<T,K>& operator=(const AVLTree<T,K>& rightObject);
    /* getSize() it will return the number of elements inside this data structure.
    * @return int
    */
    virtual int getSize();
    /* insert(T &d, K &k) allow insertion of elements using a value (T &d) and key (K &k).
    * @param T is the object (value) by reference
    * @param K is the key (string) by reference used to insert value.
    * @return Void
    */
    virtual void insert( T &d, K &k);
    /* find(K &k) allow finding of elements using a key (K &k).
    * @param K is the key (string) by reference used to find value.
    * @return T&
    */
    virtual T& find( K& k );
    /* printInOrder() will print out all of the keys in the order they stored.
    * @return void
    */
    virtual void printInOrder();
    /* printIndex() is virtual function, that will print out the from each data structure into a .txt file.
    * @return void
    */
    virtual void printIndex();
    /*  top50Common() it will return a map of ints and strings containing the top 50 common words.
    * @return std::map<int, std::string, std::greater<int>>&
    */
    virtual std::map<int, std::string, std::greater<int>>& top50Common();
    /*  getWordObject() wil call getWordObject(Node<T,K>*)
    * @return void
    */
    void getWordObject();
    /*   getWordObject(Node<T,K>*) will be used along side of top50Common() to help calculate top 50 words.
    * @return void
    */
    void getWordObject(Node<T,K>* ptrAtThisNode);
    /*  getTotalWordsEachCase() will return a map of strings and ints getting the total number of words per case.
    * @return std::map<std::string, int>&
    */
    std::map<std::string, int>& getTotalWordsEachCase();

private:
    Node<T,K>* root;
    int treeNodes;
    std::map<int, std::string, std::greater<int>> top50;
    std::map<std::string, int> IDandTotalWords;
    void copy(Node<T,K>* rightObjNodePtr);//used for copy constructor
    void clear(Node<T,K>* &rootPtr);//used for destructor
    void insert( T &d, K &k, Node<T,K>* &r);//passing root by reference
    void printInOrder(Node<T,K>* r);
    void printIndex(Node<T,K>* r, std::ofstream &output);
    int getHeight(Node<T,K>* n);
    int max(int l, int r);
    void rotateWithLeftChild(Node<T,K>* &r);//passing root by reference
    void doubleWithLeftChild(Node<T,K>* &r);//passing root by reference
    void doubleWithRightChild(Node<T,K>* &r);//passing root by reference
    void rotateWithRightChild(Node<T,K>* &r);//passing root by reference
};

//default constructor
template <typename T,typename K>
AVLTree<T,K> :: AVLTree()
{
    //initializing private variables
    root = nullptr;
    treeNodes = 0;
}

//copy constructor
template <typename T,typename K>
AVLTree<T,K> :: AVLTree(const AVLTree<T,K>& rightObject)
{
    this->root = nullptr;
    this->treeNodes = 0;
    //call copy function passing in the object to be copied.
    copy(rightObject.root);
}

//deconstructor
template <typename T,typename K>
AVLTree<T,K> :: ~AVLTree()
{
    //calling clear function to delete everything in tree.
    clear(this->root);
}

//overloaded assignment operator
template <typename T,typename K>
AVLTree<T,K>& AVLTree<T,K> :: operator=(const AVLTree<T,K>& rightObject)
{
    //checking if we are copying the same object
    if(this == &rightObject){
        return *this;
    }

    //calling clear function to delete everything in tree.
    clear(this->root);
    //call copy function passing in the object to be copied.
    copy(rightObject.root);

    return *this;
}

/*
This function will copy values, form the right hand side object.
This object is currently empty however, rightObject may not be empty.
copy function is used in copy constructor and assignment operator.
*/
template <typename T,typename K>
void AVLTree<T,K> :: copy(Node<T,K>* rightObjNodePtr)
{
    if(rightObjNodePtr){
        //add a new element from the right object to the left object
        insert(rightObjNodePtr->data, rightObjNodePtr->key);
        //copy recursively in order
        copy(rightObjNodePtr->left);
        copy(rightObjNodePtr->right);
    }
}

//function that frees dynamic memory.
template <typename T,typename K>
void AVLTree<T,K> :: clear(Node<T,K>* &rootPtr)
{
    //checking if tree is empty
    if(rootPtr != nullptr){
        //delete recursively on both left and right
        clear(rootPtr->left);
        clear(rootPtr->right);
        delete rootPtr;
        //make ptr null
        --treeNodes;
    }
    rootPtr = nullptr;
}

//function that gets the size of tree
template <typename T,typename K>
int AVLTree<T,K> :: getSize()
{
    return treeNodes;
}

//public insert function that will call private
template <typename T,typename K>
void AVLTree<T,K> :: insert( T &t, K &k)
{
    //passing in both values by reference
    insert(t, k, root);
}

//private insert function
template <typename T,typename K>
void AVLTree<T,K> :: insert( T &t, K &k, Node<T,K>* &ptrAtThisNode)
{
    //check if the tree is empty
    if(ptrAtThisNode == nullptr){
        //creating the root of the tree
        ptrAtThisNode = new Node<T,K>(t,k);
        ++treeNodes;
    }
    //checking if next value of the tree goes to left
    else if(k < ptrAtThisNode->key)
    {
        //recursive call to insert on left side
        insert(t, k,ptrAtThisNode->left);
        //check for balance factor WHEN INSERTING IN THE LEFT, if true then its not balanced.
        if(getHeight(ptrAtThisNode->left) - getHeight(ptrAtThisNode->right) ==  2)
        {
            //checking for the imabalance factor, checking values inside node to do case 1 or 2
            if( k < ptrAtThisNode->left->key )//case 1 rotation
                rotateWithLeftChild(ptrAtThisNode);
            else //case 2 rotation
                doubleWithLeftChild(ptrAtThisNode);
        }
    }
    else if(k > ptrAtThisNode->key)
    {
        //recursive call to insert on right side
        insert(t, k,ptrAtThisNode->right);
        //check for balance factor WHEN INSERTING IN THE RIGHT, if true then its not balanced.
        if(getHeight(ptrAtThisNode->right) - getHeight(ptrAtThisNode->left) == 2){
            //checking for the imabalance factor, checking values inside node to do case 3 or 4
            if(ptrAtThisNode->right->key < k)//case 4 rotation
                rotateWithRightChild(ptrAtThisNode);
            else//case 3 rotation
                doubleWithRightChild(ptrAtThisNode);
        }
    }
    else{}//value already existsin map, SPECIAL CASE

    //update height
    ptrAtThisNode->height = 1 + max( getHeight(ptrAtThisNode->left), getHeight(ptrAtThisNode->right) );
}

//displaying values inside of the avl tree
template <typename T,typename K>
void AVLTree<T,K> :: printInOrder()
{
    //checking if tree is empty
    if(root == nullptr)
        throw std::out_of_range("Error trying to print empty AVL tree. Inside printInOrder.");
    //calling private printInOrder method
    printInOrder(root);
}

//displaying values inside of the avl tree
template <typename T,typename K>
void AVLTree<T,K> :: printInOrder(Node<T,K>* ptrAtThisNode)
{
    //recursive call until null
    if(ptrAtThisNode != nullptr)
    {
        //1st. Getting everything to left
        printInOrder(ptrAtThisNode->left);
        //2nd. Outputting values. //setw(ptrAtThisNode->height *4) to print in tree format
        std::cout << ptrAtThisNode->data << std::endl;
        //3rd. Getting evertyhing to the right.
        printInOrder(ptrAtThisNode->right);
    }
}

template <typename T,typename K>
void AVLTree<T,K> :: printIndex()
{
    std::ofstream output;
    output.open("output.txt");
    //checking if tree is empty
    if(root == nullptr)
        throw std::out_of_range("Error trying to print empty AVL tree. Inside printInOrder.");
    //calling private printInOrder method
    printIndex(root,output);
    output.close();
}



/**PrintIndex prints the contents of the data structure
 * into an output file
 * @param
 */
//Prints to file inside the tree
template <typename T,typename K>
void AVLTree<T,K> :: printIndex(Node<T,K>* ptrAtThisNode, std::ofstream &output)
{

    //recursive call until null
    if(ptrAtThisNode != nullptr)
    {
        //1st. Getting everything to left
        printIndex(ptrAtThisNode->left, output);
        //2nd. Outputting values. //setw(ptrAtThisNode->height *4) to print in tree format
        //std::cout << ptrAtThisNode->data;
        output << ptrAtThisNode->data << '\n';
        //3rd. Getting evertyhing to the right.
        printIndex(ptrAtThisNode->right, output);
    }
}

/*This function will use to measure the height of each child.*/
template <typename T,typename K>
int AVLTree<T,K> :: getHeight(Node<T,K>* nPtr)
{
    //if no children return 0
    if(nPtr== nullptr)
        return 0;
    //returing height of the child.
    return nPtr->height;
}

/*This function will use to measure the height of each child.*/
template <typename T,typename K>
int AVLTree<T,K> :: max(int leftChild, int rightChild)
{
    //returing biggest between left and right
    return leftChild > rightChild ? leftChild : rightChild;
}

//case 1 rotation
template <typename T,typename K>
void AVLTree<T,K> :: doubleWithLeftChild(Node<T,K>* &K2)
{
    //converting case2 to a case1 by rotation
    rotateWithRightChild(K2->left);
    //balancing a tree using case1
    rotateWithLeftChild(K2);
}

/*
This is a case two rotation:
-left child of right substree.
*/
template <typename T,typename K>
void AVLTree<T,K> :: rotateWithLeftChild(Node<T,K>* &K2)
{
    //1) Create a pointer to the node left of the value passed in the function
    Node<T,K>* K1 = K2->left;
    //2) Make unbalanced left pointer point to the right of its left
    K2->left = K1->right;
    //3) Make the the right ptr of K1 point to K2
    K1->right = K2;
    //4) Update heights of each node and rotate nodes and switch imbalance node withs its left
    K2->height = 1+ max( getHeight(K2->left), getHeight(K2->right) );
    K1->height = 1+ max( getHeight(K1->left), getHeight(K2) );
    K2 = K1;
}

//case 4 rotation
template <typename T,typename K>
void AVLTree<T,K> :: doubleWithRightChild(Node<T,K>* &K2)
{
    //converting case3 to a case1 by rotation
    rotateWithLeftChild(K2->right);
    //balancing a tree using case4
    rotateWithRightChild(K2);
}

/*
This is a case three rotation:
-rightt child of left substree.
*/
template <typename T,typename K>
void AVLTree<T,K> :: rotateWithRightChild(Node<T,K>* &K2)
{
    //1) Create a pointer to the node left of the value passed in the function
    Node<T,K>* K1 = K2->right;
    //2) Make unbalanced right pointer point to the () of its ()
    K2->right = K1->left;
    //3) Make the the left ptr of K1 point to K2
    K1->left = K2;
    //4)The height of k2 will depend if there was a node added or not from k1.
    K2->height = 1+ max( getHeight(K2->left), getHeight(K2->right) );
    //The height of K1 will be the height of its own right + the height of its left or K2
    K1->height = 1+ max( getHeight(K1->right), getHeight(K2) );
    K2 = K1;

}

/*NOTE THIS FUNCTION HAS NEEDS A TRY AND CATCH TO BE IMPLEMENTED.
    IF THE OBJECTS EXISTS IN TREE THEN IT WILL RETURN IT,
    IF NOT IT WILL THROW AN EXCEPTION.
*/
template <typename T,typename K>
T& AVLTree<T,K> :: find( K& data )// FIND BY KEY
{
    Node<T,K>* temp = root;
    while (temp != nullptr){
      if (temp->key == data)
          return temp->data;//return object
      if (temp->key < data)
          temp = temp->right;
      else
          temp = temp->left;
    }

    throw std::out_of_range("Error element not found in Tree. Inside find.");
}

template <typename T,typename K>
void AVLTree<T,K> :: getWordObject()
{
    //checking if tree is empty
    if(root == nullptr)
        throw std::out_of_range("Error trying to print empty AVL tree. Inside printInOrder.");
    //calling private printInOrder method
    getWordObject(root);
}

//displaying values inside of the avl tree
template <typename T,typename K>
void AVLTree<T,K> :: getWordObject(Node<T,K>* ptrAtThisNode)
{
    //recursive call until null
    if(ptrAtThisNode != nullptr)
    {
        //1st. Getting everything to left
        getWordObject(ptrAtThisNode->left);
        //2nd. Outputting values. //setw(ptrAtThisNode->height *4) to print in tree format
        int count = 0;
        int countForID = 0;
        for(auto it : ptrAtThisNode->data.getFileAndCount())
        {
            count += it.second;
            auto itFind = IDandTotalWords.find(it.first);
            if(itFind == IDandTotalWords.end())
                IDandTotalWords.insert(make_pair(it.first, 0));
            else
            {
                countForID += it.second;
                itFind->second = countForID;
            }
        }
        top50.insert(make_pair(count, ptrAtThisNode->key));
        //3rd. Getting evertyhing to the right.
        getWordObject(ptrAtThisNode->right);
    }
}

//Function to get a map of every word and its commoness
template <typename T,typename K>
std::map<int, std::string, std::greater<int>>& AVLTree<T,K>::top50Common()
{
    getWordObject();
    return top50;
}

template <typename T,typename K>
std::map<std::string, int>& AVLTree<T,K>::getTotalWordsEachCase()
{
    getWordObject();
    return IDandTotalWords;
}



#endif
