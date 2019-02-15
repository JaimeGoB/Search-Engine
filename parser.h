/*This class is sent individual files and returns
 * a parsed verison of the file along with
 * any information that the user requests of that
 * file
 *
 * Terry Wang
 *
 * Along with Skyler and Jaime for the other parts
 * but parser is mine
 *
 * Dec 2018
 */

#ifndef PARSER_H
#define PARSER_H
#include <iostream>
#include <cstring>
#include <stdio.h>
#include <string>
#include <sstream>
#include <istream>
#include <vector>
#include <map>
#include <stdexcept>
#include <fstream>
#include <chrono>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <regex>
#include <porter2_stemmer.h>
#include <set>
#include "Word.h"
#include "dirent.h"
#include "IndexInterface.h"
#include <vector>

/*Code for reading in files adapted
 * from https://stackoverflow.com/questions/116038/what-is-the-best-way-to-read-an-entire-file-into-a-stdstring-in-c
 */

class Parser
{
public:
    Parser();
    /**
     * getFiles gets the filepath and stores all the filenames
     * into a vector
     *
     * @param path is the filepath
     * @param extn is.json or any other extension
     * @return the vector with all the fileNames
     */
    std::vector<std::string> getFiles(std::string,std::string);
    /**
     * The Parse function parses all the words for a set number of cases
     * using regex
     *
     * @param filePath the filePath
     * @param fileNum the actual id of the file
     * @param index the data structure
     */
    void parse(std::string, std::string,  IndexInterface<Word,std::string>&);
    /**
     * isStopWord will return if the word is a stopWord
     * @return true if stopword, false if not
     */
    bool isStopWord(std::string); //check if a word is a stop word, return true if it is
    /**
     * getOverallWordTotal returns the total number of works
     * we have parsed
     * @return a long with the total number of words
     */
    unsigned long getOverallWordTotal();
    /**
     * Parser::parseOneFile parses one file for its text
     * and prints out the first 300 words of the text
     *
     * @param tempfilePathVec the vector of file paths
     * @param fileNum the fileName
     */
    void parseOneFile(std::vector<std::string>&, std::string);
    /**
     * ParseIndex will convert the index file into a
     * readable file.
     *
     * @param filePath the path of the File where index is stored(Hardcoded)
     * @param index the data structure
     */
    void parseIndex(std::string, IndexInterface<Word,std::string>&);
    /**
     * Parser::returnTextofFile returns the entire text of the document
     *
     * @param tempfilePathVec the vector with the filePath
     * @param fileNum the id of the file
     * @return a string of the entire file text
     */
    std::string returnTextofFile(std::vector<std::string>&, std::string);
    /**
     * NumberofTerms returns the number of a certain term in a doucment
     *
     * @param tempFilePathVec the vector for the filePaths
     * @param fileNum the actual file
     * @param word The word that is used
     * @return the number of terms in a doucment
     */
    int numberofTerms(std::vector<std::string>&, std::string, std::string, IndexInterface<Word,std::string>& );
    /**
     * Parser::parseResultInfo
     * @param tempfilePathVec vector of file path
     * @param fileNum the fileName
     */
    void parseResultInfo(std::vector<std::string>&, std::string);
private:
    std::set<std::string> stopWords;
    std::vector<std::string> fileNames;
    unsigned long OverallWordTotal;
    std::map<std::string, std::string> keepTrack;
    /**
     * The Private Function that parses the words for a case
     * and stores them inside the temporary map for that case of
     * unique words and number of types the appear
     *
     * @param wordCase the map
     * @param textType html or plaintext
     */
    void parseCase(std::map<std::string,int>&, std::istringstream&);
    /**
     * readInFiles is a private function to
     * create the Istream object to read in files
     *
     * @param tempfilePathVec& is the vector of filepaths
     * @param fileNum is the actual file name
     * @return a completed istream object
     */
    std::ifstream readInFiles(std::vector<std::string>&, std::string);
    std::regex reg;
};
#endif // PARSER_H
