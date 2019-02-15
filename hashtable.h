#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <iostream>
#include <string>
#include <stdexcept>
#include <iomanip>
#include "IndexInterface.h"
#include "list"
#include <fstream>

template <typename T,typename K> class HashTable;

template <typename T,typename K>
class HashTable : public IndexInterface<T,K>
{

public:
    /* HashTable is the default constructor of the sub class.
    * @return HashTable<T,k>
    */
    HashTable();
    /* HashTable is the copy constructor of the sub class
     * @param rightObj is the object we are copying to the initialized object.
    * @return HashTable<T,k>
    */
    HashTable(const HashTable<T,K> &rightObj);
    /* Destructor, deletes dynamic memory.
    * @return HashTable<T,K>
    */
    ~HashTable();
    /* operator= is the overloaded assignment operator
     * @param rightObj is the object we are copying to the object left of (=).
    * @return HashTable<T,k>&
    */
    HashTable<T,K>& operator=(const HashTable<T,K> &rightObj);
    /* operator[] will overload the subscript operator for the hashtable. Same implementation as find.
    * @param K is the key (string) by reference used to find value.
    * @return T&
    */
    virtual T& operator[](K& lookUpKey);
    /* getSize() it will return the number of elements inside each data structure.
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
    /* printInOrder will print out all of the keys in the order they stored.
    * @return void
    */
    virtual void printInOrder();
    /* printIndex() is virtual function, that will print out the from each data structure into a .txt file.
    * @return void
    */
    virtual void printIndex();
    /* makingTop50andTotalWordEachCase() will be used to calculate top 50 words per case.
    * @return void
    */
    void makingTop50andTotalWordEachCase();
    /*  top50Common() is pure virtual, it will return a map of ints and strings containing the top 50 common words.
    * @return std::map<int, std::string, std::greater<int>>&
    */
    virtual std::map<int, std::string, std::greater<int>>& top50Common();
    /* getTotalWordsEachCase() is pure virtual, it will return the number of words per case, that will be used for stats purposes.
    * @return std::map<std::string, int>&
    */
    virtual std::map<std::string, int>& getTotalWordsEachCase();

private:
    /*This function will return an index of where to store a certain element
    * based on the hash calculated by this function.
     @param K is the key (string) by reference used to insert value.
    * @return unsigned int
    */
    unsigned int getHashKeyIndex(K &k);

    //private variables
    int elementsInTable;
    const int sizeOfTable = 100000;
    std::map<std::string, int> IDandTotalWords;
    std::map<int, std::string, std::greater<int>> top50;
    std::list<std::pair<K,T>> *TableList;
};
//default constructor
template <typename T,typename K>
HashTable<T,K> :: HashTable()
{
    //initializing private variables
    elementsInTable = 0;
    TableList = new std::list<std::pair<K,T>>[sizeOfTable];
}

//copy constructor
template <typename T,typename K>
HashTable<T,K> :: HashTable(const HashTable<T,K> &rightObj)
{
    //copy private variables
    this->elementsInTable = rightObj.elementsInTable;
    //This first for loop is to acces each individual list in list array.
    for(int index = 0; index < sizeOfTable; ++index){
        //copy every single pair from right to left
        for(auto it = rightObj.TableList[index].begin(); it != rightObj.TableList[index].end(); ++it ){
            std::pair<K,T> tempPair;
            //first will contain the identifer of the object
            tempPair.first = it->first;
            //second will contain the object itself
            tempPair.second = it->second;
            //we insert pair into hashtable
            TableList[index].push_back(tempPair);
        }
    }
}

//destructor
template <typename T,typename K>
HashTable<T,K> :: ~HashTable()
{
    //delete dynamic memory
    delete [] TableList;
}

//overloaded assignment operator
template <typename T,typename K>
HashTable<T,K>& HashTable<T,K> :: operator=(const HashTable<T,K> &rightObj)
{
    //first we delete evertyhing on left hand side
    delete [] TableList;
    //allocate a new table
    TableList = new std::list<std::pair<K,T>>[sizeOfTable];
    //copy private variables
    this->elementsInTable = rightObj.elementsInTable;
    //This first for loop is to acces each individual list in list array.
    for(int index = 0; index < sizeOfTable; ++index){
        //copy every single pair from right to left
        for(auto it = rightObj.TableList[index].begin(); it != rightObj.TableList[index].end(); ++it ){
            std::pair<K,T> tempPair;
            //first will contain the identifer of the object
            tempPair.first = it->first;
            //second will contain the object itself
            tempPair.second = it->second;
            //we insert pair into hashtable
            TableList[index].push_back(tempPair);
        }
    }
    //return rhs obj
    return *this;
}

/* WARNING MUST USE A TRY AND CATCH FOR THIS FUNCTION!!!!!
 * We can use this to return the object by using its key to look for it.
 * Example to use this function:
 * Hashtable["law"].getFileAndCount();
*/
template <typename T,typename K>
T& HashTable<T,K> ::  operator[](K& lookUpKey)
{
    //we pass in the key to get hash function, which index will store the index where this key should be stored
    unsigned int index = getHashKeyIndex(lookUpKey);
    //if the list at this current index is empty it means we didn't find the object
    if(TableList[index].empty())
        throw std::out_of_range("Error object not found in HashTable. Inside [] operator.");

    for(auto it = TableList[index].begin(); it != TableList[index].end(); ++it ){
        //look for the object by comparing keys in the list
        if(it->first == lookUpKey)
            return it->second;
    }
    //if the object was not found with the iterator we will throw an exception.
    throw std::out_of_range("Error object not found in HashTable. Inside [] operator.");
}

//getting the number of elements in the hashtable
template <typename T,typename K>
int HashTable<T,K> ::  getSize()
{
    return elementsInTable;
}

template <typename T,typename K>
void HashTable<T,K> ::  insert( T &data, K &key)
{
    //get index of key
    unsigned int index = getHashKeyIndex(key);
    //adding first key and value in list
    std::pair<K,T> KeyValPair;
    //first will contain the identifer of the object
    KeyValPair.first = key;
    //second will contain the object itself
    KeyValPair.second = data;
    //we insert pair into hashtable
    TableList[index].push_back(KeyValPair);
    //increment counter
    ++elementsInTable;
}


/* find function
 *WARNING MUST USE A TRY AND CATCH FOR THIS FUNCTION!!!!!
*/
template <typename T,typename K>
T&  HashTable<T,K> :: find( K& lookUpKey )
{

    //getting index of key
    unsigned int index = getHashKeyIndex(lookUpKey);

    //if the list at that index is empty throw an exception
    if( TableList[index].empty() )
        throw std::out_of_range("Error, object does not exist on hashtable.");

    //iterative through list to find the key value pair.
    for(auto pairIterator = TableList[index].begin(); pairIterator != TableList[index].end(); ++pairIterator ){
        //key to the object is found
        if(pairIterator->first == lookUpKey){
            //return the word object
            return pairIterator->second;
        }
    }
    //throw excepetion if object not found after iterating a non-empty list
    throw std::out_of_range("Error, object does not exist on hashtable.");
}

template <typename T,typename K>
void  HashTable<T,K> :: printInOrder()
{
    //This first for loop is to acces each individual list in list array.
    for(int i = 0; i < sizeOfTable; ++i){
        //SKYLER SEE THIS PLEASE!!! UNCOMMENT THE NEXT LINE,YOU CAN SEE ITS AN ARRAY OF LISTS
        //Skyler has been here!!
        //std::cout<< "Size of index "<< i << " is: " << TableList->size() <<"\n";
        //this second for loop is to iterate through each list that contains a pair
        for(auto it = TableList[i].begin(); it != TableList[i].end(); ++it ){
            //we only output the key (first), second is the word object itself.
            std::cout<<it->first << ' ';
        }
    }
}

template<typename T, typename K>
void HashTable<T,K> :: printIndex()
{
    std::ofstream output;
    output.open("output.txt");
    //This first for loop is to acces each individual list in list array.
    for(int i = 0; i < sizeOfTable; ++i){
        //SKYLER SEE THIS PLEASE!!! UNCOMMENT THE NEXT LINE,YOU CAN SEE ITS AN ARRAY OF LISTS
        //std::cout<< "Size of index "<< i << " is: " << TableList->size() <<"\n";
        //this second for loop is to iterate through each list that contains a pair
        for(auto it = TableList[i].begin(); it != TableList[i].end(); ++it ){
            //we only output the key (first), second is the word object itself.
            //std::cout<<it->first << ' ';
            output << it->second << '\n';
        }
    }
    output.close();
}

template <typename T,typename K>
unsigned int HashTable<T,K> ::  getHashKeyIndex( K& key)
{
    //std hash function,definition
    std::hash<std::string> computeHash;
    /*this temp variable will hold the index
    *where this string belongs too in the array of
    * containers. */
    unsigned int hashedKeyIndex = computeHash(key);
    /*this is used to compute boundries which will
     *  go from 0 - 100,000 */
    hashedKeyIndex = hashedKeyIndex % sizeOfTable;
    //return index
    return hashedKeyIndex;
}

template <typename T,typename K>
void HashTable<T,K>::makingTop50andTotalWordEachCase()
{
    int count = 0;
    int countforID = 0;
    //This first for loop is to acces each individual list in list array.
    for(int i = 0; i < sizeOfTable; ++i)
    {
        //this second for loop is to iterate through each list that contains a pair
        for(auto it = TableList[i].begin(); it != TableList[i].end(); ++it )
        {
            count = 0;
            countforID = 0;
            //the key it.(first), it.second is the word object itself.
            for(auto inner : it->second.getFileAndCount())
            {
                count += inner.second;
                auto itFind = IDandTotalWords.find(inner.first);
                if(itFind == IDandTotalWords.end())
                    IDandTotalWords.insert(make_pair(inner.first, 0));
                else
                {
                    countforID += inner.second;
                    itFind->second = countforID;
                }
            }
            top50.insert(make_pair(count, it->first));
        }
    }
}

template <typename T,typename K>
std::map<int, std::string, std::greater<int>>& HashTable<T,K> :: top50Common()
{
    makingTop50andTotalWordEachCase();
    return top50;
}

template <typename T,typename K>
std::map<std::string, int>& HashTable<T,K>::getTotalWordsEachCase()
{
    makingTop50andTotalWordEachCase();
    return IDandTotalWords;
}

/*Citations:
 * The hash function was used from an example in StackOverflow:
*https://stackoverflow.com/questions/33809770/hash-function-that-can-return-a-integer-range-based-on-string
*/
#endif // HASHTABLE_H
