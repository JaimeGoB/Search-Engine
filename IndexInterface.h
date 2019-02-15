#ifndef INDEXINTERFACE_H
#define INDEXINTERFACE_H
#include "Word.h"
#include "vector"

template<typename T,typename K>
class IndexInterface
{
public:
    /* IndexInterface is the default constructor of the abstract class.
    * @return IndexInterface<T,k>
    */
    IndexInterface();
    /* pure virtual Destructor, must be overwritten by subclasses.
    * @return IndexInterface<T,K>
    */
    virtual ~IndexInterface() = 0;
    /* getSize() is pure virtual, it will return the number of elements inside each data structure.
    * @return int
    */
    virtual int getSize() = 0;
    /* insert(T &d, K &k) is pure virtual, it has to be overwritten by the subclass to allow insertion of elements using a value (T &d) and key (K &k).
    * @param T is the object (value) by reference
    * @param K is the key (string) by reference used to insert value.
    * @return Void
    */
    virtual void insert( T &d,K &k ) = 0;
    /* find(K &k) is pure virtual, it has to be overwritten by the subclass to allow finding of elements using a key (K &k).
    * @param K is the key (string) by reference used to insert value.
    * @return T&
    */
    virtual T& find( K& data ) = 0;
    /* printInOrder() is virtual function, that will print keys from each data structure.
    * @return void
    */
    virtual void printInOrder();
    /* printIndex() is virtual function, that will print out the from each data structure into a .txt file.
    * @return void
    */
    virtual void printIndex();
    /* operator[] is virtual function, that will overload the subscript operator for the hashtable. Same implementation as find.
    * @param K is the key (string) by reference used to insert value.
    * @return void
    */
    virtual T& operator[](K& lookUpKey);
    /*  top50Common() is pure virtual, it will return a map of ints and strings containing the top 50 common words.
    * @return std::map<int, std::string, std::greater<int>>&
    */
    virtual std::map<int, std::string, std::greater<int>>& top50Common() = 0;
    /* getTotalWordsEachCase() is pure virtual, it will return the number of words per case, that will be used for stats purposes.
    * @return std::map<std::string, int>&
    */
    virtual std::map<std::string, int>& getTotalWordsEachCase() = 0;
private:

};
template<typename T,typename K>
inline IndexInterface<T,K>::IndexInterface(){}
template<typename T,typename K>
inline IndexInterface<T,K>::~IndexInterface(){}
template<typename T,typename K>
//virtual functions
inline void IndexInterface<T,K> :: printInOrder(){}
template<typename T, typename K>
inline void IndexInterface<T,K> :: printIndex(){}
template<typename T,typename K>
inline T& IndexInterface<T,K> :: operator[](K& lookUpKey){}
template<typename T,typename K>
std::map<int, std::string, std::greater<int>>& top50Common(){}
template<typename T,typename K>
std::map<std::string, int>& getTotalWordsEachCase(){}
#endif // INDEXINTERFACE_H
