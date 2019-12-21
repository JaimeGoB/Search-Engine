# Search-Engine
SCOTUS search engine containing 70k .json files. This search engine uses a hash table and a AVL tree to store words in the documents.


Search engine for a large collection of Supreme Court of the US (SCOTUS).  
Users will be able to enter a search query and the program will return result based on a formula. The result will output the 15 cases containing the word. Then the user can pick whichever case to further read. Once the user picked a correct case it will output the whole case for the user to read.

The formula used to determine the first 15 files are based on:
Total number of authors.
Date the article was published.
Number of times the word appears in article divided by the number of words in article. 
Number of Judges in case. 
Last Date the article was modified.


Here is the overall architecture of search engine:
![alt text](https://github.com/JaimeGoB/Search-Engine/blob/master/searchEngineArchitecture.png)

Here is a UML diagram of the solution:
![alt text](https://github.com/JaimeGoB/Search-Engine/blob/master/ProjectDesign.png)

Here is an example of a file that contains one court case:
![alt text](https://github.com/JaimeGoB/Search-Engine/blob/master/jsonFiles.png)
