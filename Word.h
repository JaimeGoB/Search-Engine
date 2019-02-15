#ifndef WORD_H
#define WORD_H
#include <iostream>
#include <string>
#include <map>

class Word
{
public:
    /* default constructor.
    * @return Word
    */
    Word();
    /* Overloaded < operator by comparing the left string caseWord with right string caseWord
     * @param rightObj is the object we are comparing to this object.
    * @return bool
    */
    bool operator<(const Word& rightObj);
    /* Overloaded > operator by comparing the left string caseWord with right string caseWord
     * @param rightObj is the object we are comparing to this object.
    * @return bool
    */
    bool operator>(const Word& rightObj);
    /* Overloaded == operator by comparing the left string caseWord with right string caseWord
     * @param rightObj is the object we are comparing to this object.
    * @return bool
    */
    bool operator==(const Word& rightObj);
    /* Overloaded ostream operator to output map in word object
     * @param rightObj is the object we are going to output.
    * @return std::ostream&
    */
    friend std::ostream & operator<<(std::ostream &out, const Word& w);
    /* setter for word
     * @param w is the what caseword will be set to.
    * @return void
    */
    void setWord(std::string w);
    /* getter for word
    * @return std::string&
    */
    std::string& getWord();
    /* setter for map inside of word
     * @param f is the file name
     * @param c is the number of times the case word appears in that file.
    * @return void
    */
    void upDateFileAndCount(std::string f, int c);
    /* getter for map inside of word
    * @return std::map<std::string, int>&
    */
    std::map<std::string, int>& getFileAndCount();
private:
    std::string caseWord;
    /*first element in map will be the file identifier
    second element in map will be the apprerances in that file*/
    std::map<std::string, int> FileIdAndCount;
};

#endif // WORD_H
