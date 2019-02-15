#include "Word.h"
#include <iostream>
#include <string>
#include <map>

/*getters and setters for private variables for each word object*/
Word :: Word()
{
    caseWord = "\0";
}

bool Word :: operator<(const Word& rightObj)
{
    return this->caseWord < rightObj.caseWord;
}

bool Word :: operator>(const Word& rightObj)
{
    return this->caseWord > rightObj.caseWord;
}

bool Word :: operator==(const Word& rightObj)
{
    return this->caseWord == rightObj.caseWord;
}

void Word :: setWord(std::string w)
{
    caseWord = w;
}

std::string& Word :: getWord()
{
    return caseWord;
}

void Word :: upDateFileAndCount(std::string fileId, int countOfWord)
{
    FileIdAndCount.emplace(fileId, countOfWord);
}

std::map<std::string, int>& Word :: getFileAndCount()
{
    return FileIdAndCount;
}

std::ostream & operator<<(std::ostream &out, const Word& w)
{
    out << w.caseWord << " ";
    for(const auto &word: w.FileIdAndCount)
    {
        std::string moreTemp = std::to_string(word.second);
        std::string temp = word.first;
        out << temp + " ";
        out << moreTemp <<  " ";
        //std::cout << word.first;
        //out << word.first << "; ";
        //out << std::to_string(word.second);
        //std::cout << word.second;

    }
    //out << w.caseWord;
    return out;
}
