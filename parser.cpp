#include "parser.h"

//stop words
std::string strArray[155] = {"a", "about", "above", "after", "again", "against", "all", "am", "an",
                             "and", "any", "are", "as", "at", "be", "because", "been", "before", "being",
                             "below", "between", "both", "but", "by", "could", "did", "do", "does", "doing",
                             "down", "during", "each", "few", "for", "from", "further", "had", "has", "have",
                             "having", "he", "he'd", "he'll", "he's", "her", "here", "here's", "hers",
                             "herself", "him", "himself", "his", "how", "how's", "i", "i'd", "i'll", "i'm",
                             "i've", "if", "in", "into", "is", "it", "it's", "its", "itself", "let's", "me",
                             "more", "most", "my", "myself", "no", "not", "nor", "of", "on", "once", "only", "or", "other",
                             "ought", "our", "ours", "ourselves", "out", "over", "own", "same", "she", "she'd",
                             "she'll", "she's", "should", "so", "some", "such", "than", "that", "that's", "the",
                             "their", "theirs", "them", "themselves", "then", "there", "there's", "these", "they",
                             "they'd", "they'll", "they're", "they've", "this", "those", "through", "to",
                             "too", "under", "until", "up", "very", "was", "we", "we'd", "we'll", "we're",
                             "we've", "were", "what", "what's", "when", "when's", "where", "where's", "which",
                             "while", "who", "who's", "whom", "why", "why's", "with", "would", "you", "you'd",
                             "you'll", "you're", "you've", "your", "yours", "yourself", "yourselves"};


Parser::Parser()
{
    OverallWordTotal = 0;
    reg = ("<.*?>");
    for(int i = 0; i < 155; i++)//155 is number of stop words
        stopWords.insert(strArray[i]);
}

std::vector<std::string> Parser::getFiles(std::string path, std:: string extn)
{
    /* adapted from:
     * https://stackoverflow.com/questions/612097
     * /how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c
     */
    std::vector<std::string> result;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(path.c_str())) != nullptr) {
        while ((ent = readdir(dir)) != nullptr) {
            int len = strlen(ent->d_name);
            if (ent->d_type == DT_REG &&
                len > extn.length() &&
                strcmp(ent->d_name + len - extn.length(), extn.c_str()) == 0)
                result.push_back(ent->d_name);
        }
        closedir(dir);
    }
    else
        throw std::invalid_argument("Provided path could not be opened");

    fileNames = result;
    return result; //If want to send vector
}

bool Parser::isStopWord(std::string s)
{
    std::set<std::string>::iterator it;
    it = stopWords.find(s);
    if(it == stopWords.end())
        return false;
    return true;
}

void Parser::parse(std::string filePath, std::string fileNum, IndexInterface<Word,std::string>& index)
{
    //This is the map of all the words and number of times it appear
    std::map<std::string,std::map<std::string,int>> wordtoCases;
    std::map<std::string,int> wordCase;
    std::regex line("\n");

    std::ifstream firstFile(filePath);
    if(!firstFile){
        std::cerr << "File could not be read." << std::endl;
        exit(EXIT_FAILURE);
    }
    //Gets the Json file and Parses it
    std::stringstream sstr;
    sstr << firstFile.rdbuf();
    std::string jstring = sstr.str();
    firstFile.close();
    std::transform(jstring.begin(), jstring.end(), jstring.begin(), ::tolower);//lowercase
    const char* json = jstring.c_str(); //String to cstring
    rapidjson::Document cases;
    cases.Parse(json);
    //Keep track of already stemmed words
    //Can be substitute by using a 'dictionary' text file
    //First if is testing html
    if(!cases["html"].IsNull() && (strcmp(cases["html"].GetString(), "") != 0))
    {

        //Can Switch regex or not by commeting this line and remove comment on next
        //std::istringstream ss{std::regex_replace(cases["html"].GetString(),reg, " ")};
        std::istringstream ss{(cases["html"].GetString())};
        parseCase(wordCase, ss);
    }
    //This else then checks plain text
    else
    {
        std::istringstream ss{(cases["plain_text"].GetString())};
        parseCase(wordCase, ss);
    }
    //One Case is done (creating an indivual case)
    for(auto iter: wordCase )
    {
        Word caseWord;
        caseWord.setWord(iter.first);
        caseWord.upDateFileAndCount(fileNum, iter.second);

        try{
            //check if object exists and update that word object
            index.find(caseWord.getWord()).upDateFileAndCount( fileNum, iter.second);}
        catch (std::exception &e){
            //object doesnt exitst so we insert in avl tree
            index.insert( caseWord, caseWord.getWord() );}
    }
}

unsigned long Parser :: getOverallWordTotal()
{
    return OverallWordTotal;
}


void Parser::parseCase(std::map<std::string,int>& wordCase, std::istringstream& textType)
{
    std::string temp; //A single word
    int count = 0;//keep track of number of wordToFind appears in document
    while(textType >> temp)
    {
        temp.erase (std::remove_if (temp.begin (), temp.end (), ispunct), temp.end ());
        if(isStopWord(temp))
            temp = "";
        else
        {
            if(keepTrack.find(temp) != keepTrack.end())
            {
                ++wordCase[keepTrack.find(temp)->second];
                OverallWordTotal++;//counting every single word parsed excluding stop words.
            }
            else
            {
                std::string temp2 = temp;
                Porter2Stemmer::stem(temp);
                keepTrack.insert(std::pair<std::string, std::string>(temp2, temp));
                ++wordCase[temp];
                OverallWordTotal++;         //counting every single word parsed excluding stop words.
            }
        }
    }
}

void Parser::parseOneFile(std::vector<std::string> &tempfilePathVec, std::string fileNum)
{
    std::ifstream firstFile = readInFiles(tempfilePathVec,fileNum);

    //Gets the Json file and Parses it
    std::stringstream sstr;
    sstr << firstFile.rdbuf();
    std::string jstring = sstr.str();
    firstFile.close();
    //std::transform(jstring.begin(), jstring.end(), jstring.begin(), ::tolower);//lowercase
    const char* json = jstring.c_str(); //String to cstring
    rapidjson::Document cases;
    cases.Parse(json);
    //Keep track of already stemmed words
    //Can be substitute by using a 'dictionary' text file
    //First if is testing html
    if(!cases["html"].IsNull() && (strcmp(cases["html"].GetString(), "") != 0))
    {
        //Can Switch regex or not by commeting this line and remove comment on next
        //std::istringstream ss{std::regex_replace(cases["html"].GetString(),reg, " ")};
        std::istringstream ss{std::regex_replace(cases["html"].GetString(),reg, " ")};
        //std::istringstream ss{(cases["html"].GetString())};
        int count = 0;
        std::string temp;
        while (count < 300 && !ss.eof()) {
            ss >> temp;
            std::cout << temp << " ";
            ++count;
        }
    }
    //This else then checks plain text
    else
    {
        std::istringstream ss{(cases["plain_text"].GetString())};
        int count = 0;
        std::string temp;
        while (count < 300 && !ss.eof()) {
            ss >> temp;
            std::cout << temp << " ";
            ++count;
        }
    }
}

void Parser::parseIndex(std::string filePath, IndexInterface<Word,std::string>& index)
{
    std::map<std::string,int> wordCase;
    std::ifstream firstFile(filePath);
    if(!firstFile){
        std::cerr << "File could not be read." << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string tempText;
    std::string word;
    std::string fileName; //Name of the case
    int num; //How many times that case appears for a word
    //std::stringstream str;

    while(std::getline(firstFile, tempText))
    {
        Word wordNode;
        std::stringstream str(tempText);
        //++wordCase[]
        str >> word;
        wordNode.setWord(word); //Sets the word into wordNode
        while(str >> fileName >> num )
        {
            OverallWordTotal += num;
            //Updates the map of the wor
            wordNode.upDateFileAndCount(fileName, num);
        }
        //Puts the final word into the map
        index.insert(wordNode, wordNode.getWord());
    }
    firstFile.close();
}


std::string Parser::returnTextofFile(std::vector<std::string> &tempfilePathVec, std::string fileNum)
{
    std::ifstream firstFile = readInFiles(tempfilePathVec,fileNum);
    //Gets the Json file and Parses it
    std::stringstream sstr;
    sstr << firstFile.rdbuf();
    std::string jstring = sstr.str();

    firstFile.close();

    //std::transform(jstring.begin(), jstring.end(), jstring.begin(), ::tolower);//lowercase
    const char* json = jstring.c_str(); //String to cstring
    rapidjson::Document cases;
    cases.Parse(json);

    //Keep track of already stemmed words
    //Can be substitute by using a 'dictionary' text file
    //First if is testing html
    if(!cases["html"].IsNull() && (strcmp(cases["html"].GetString(), "") != 0))
    {

        //Can Switch regex or not by commeting this line and remove comment on next
        //std::istringstream ss{std::regex_replace(cases["html"].GetString(),reg, " ")};
        std::istringstream ss{std::regex_replace(cases["html"].GetString(),reg, " ")};
        //std::istringstream ss{(cases["html"].GetString())};
        return ss.str();
    }
    //This else then checks plain text
    else
    {
        std::istringstream ss{(cases["plain_text"].GetString())};
        return ss.str();
    }
}

int Parser::numberofTerms(std::vector<std::string> &tempfilePathVec, std::string fileNum, std::string word, IndexInterface<Word,std::string>& index)
{
    std::map<std::string,int> wordMap = index.find(word).getFileAndCount();
    return wordMap.find(fileNum)->second;
}

void Parser::parseResultInfo(std::vector<std::string> &tempfilePathVec, std::string fileNum)
{
    std::ifstream firstFile = readInFiles(tempfilePathVec,fileNum);

    //Gets the Json file and Parses it
    std::stringstream sstr;
    sstr << firstFile.rdbuf();
    std::string jstring = sstr.str();
    firstFile.close();
    //std::transform(jstring.begin(), jstring.end(), jstring.begin(), ::tolower);//lowercase
    const char* json = jstring.c_str(); //String to cstring
    rapidjson::Document cases;
    cases.Parse(json);

    std::string author;
    std::string date;
    std::cout << fileNum << '\n';


    if(!cases["author"].IsNull() && (strcmp(cases["author"].GetString(), "") != 0))
    {
        std::istringstream ss{(cases["author"].GetString())};
        author = ss.str();
    }

    else
    {
        author = "N/A";
    }

        //std::string temp = cases["author"].GetString();

    if(!cases["date_created"].IsNull() && (strcmp(cases["date_created"].GetString(), "") != 0))
    {
        std::istringstream ss{(cases["date_created"].GetString())};
        date = ss.str();
    }
    else{
        date = "N/A";
    }
    std::cout << "Document satisfying condition is " << fileNum << '\n' <<
              "Author is :" << author << '\n'
              << "Date:" << date << '\n';

}

std::ifstream Parser::readInFiles(std::vector<std::string> &tempfilePathVec, std::string fileNum)
{
    unsigned long filePathPosition = 0;
    //This is the map of all the words and number of times it appear
    std::string filePath = tempfilePathVec[filePathPosition] + "/" + fileNum;
    std::ifstream firstFile( filePath);
    //This while goes through the filepath and checks if its valid
    //Bascially goes through every filePath till it reaches the right one
    while(!firstFile)
    {
        //std::cerr << "Hi" << '\n';
        filePathPosition++;
        filePath = tempfilePathVec[filePathPosition] + "/" + fileNum;
        //filePath = "/home/student/Desktop/scotus";
        filePath = filePath + "/" + fileNum;
        firstFile.close();
        firstFile.open(filePath);
        std::cout << tempfilePathVec.size();
        if(filePathPosition > tempfilePathVec.size())
        {
            std::cerr << "File could not be read." << std::endl;
            //return "You done messed up and the filePath doesnt exist"; //Exit if filepath couldn't be found for somereason
            //SO we don't just fail if filepath isn't found
        }
    }
    return firstFile;
}












