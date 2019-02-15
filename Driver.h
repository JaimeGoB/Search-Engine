#ifndef DRIVER_H
#define DRIVER_H
#include <iostream>
#include <avltree.h>
#include <hashtable.h>
#include <parser.h>
#include <exception>
#include <IndexInterface.h>
#include <random>
#include <thread>
#include <stdexcept>
#include "hashtable.h"
#include <porter2_stemmer.h>
#include <vector>

class Driver
{
public:
    /**
     * Constructor for Driver class
     * @param string: initial folder path
     */
    Driver(std::string);
    /**
     * Destructor
     * @param None
     */
    ~Driver();
    /**
     * User Interface for the program, take user's command and operate
     * @param None
     * @return Void
     */
    void userInterface();
    /**
     * Using AVLTree as data structure to operate user's commands
     * @param None
     * @return Void
     */
    void TestingWithAVLTree();
    /**
     * Using HashTable as data structure to operate user's commands
     * @param None
     * @return Void
     */
    void TestingWithHashTable();
    /**
     * Build AVLTree or HashTable storage as user's choice
     * @param DataStructure Of user's choice: AVLTree or HashTable
     * @return Void
     */
    void makingStorage(IndexInterface<Word, std::string>*& dataStructure);
    /**
     * Query with Operand "AND" at the beginning of query, display results and proceed with user's command
     * @param stringstream
     * @param DataStructure Of user's choice: AVLTree or HashTable
     * @return Void
     */
    void andQuery(std::stringstream&, IndexInterface<Word, std::string>*& dataStructure);
    /**
     * Query without either "AND" or "OR" at the beginning of query, display results and proceed with user's command
     * @param stringstream
     * @param DataStructure Of user's choice: AVLTree or HashTable
     * @return Void
     */
    void notQuery(std::stringstream&, IndexInterface<Word, std::string>*& dataStructure);
    /**
     * Query with Operand "OR" at the beginning of query, display results and proceed with user's command
     * @param stringstream
     * @param DataStructure Of user's choice: AVLTree or HashTable
     * @return Void
     */
    void orQuery(std::stringstream&, IndexInterface<Word, std::string>*& dataStructure);
    /**
     * Show Basic statistics
     * @param DataStructure Of user's choice: AVLTree or HashTable
     * @return Void
     */
    void basicStat();
    /**
     * Broken down statistics to show Basic statistics
     * @param DataStructure Of user's choice: AVLTree or HashTable
     * @return Void
     */
    void stat(IndexInterface<Word, std::string>*& dataStructure);

private:
    std::string file;
    int filesToIndex;
    std::string wordToFind;
    unsigned long totalWords;
    IndexInterface<Word, std::string>* Tree = new AVLTree<Word, std::string>();
    IndexInterface<Word, std::string>* Table = new HashTable<Word, std::string>();
    std:: vector<std::string> filePathsVec;
};

#endif // DRIVER_H
