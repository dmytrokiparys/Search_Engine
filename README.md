# Search Engine
Application for search text in files.
## Content
* [Description](#description)
* [Methods](#methods)
* [Starting](#starting)
* [Build](#build)

### Description

Search engine for local files. The search is performed according to the specified parameters (config.json) and outputs the result to the final file "answers.json".

The principle of the search movement should be heard:
1. In the configuration file, before starting the application, the names are set
files, the search engine will be configured by subject.
2. The search engine must independently bypass all files and
index them so that later on any search query to find the most
relevant documents.
3. The user sets the request via the requests.json JSON file. The request is
a set of words to search for documents.
4. The query is transformed into a list of words.
5. The index looks for those documents that contain all these words.
6. Search results are ranked, sorted and given to the user,
the maximum number of possible documents in the response is set in
configuration file.
7. At the end, the program generates the answers.json file, in which it writes
searching results.

### Methods

#### ConverterJSON
- std::vector<std::string> GetTextDocuments() - opens the specified file as a settings file for the application. If the file has not been opened, the application will throw an ConfigFileMissing exception. Returns a list of words, specified in files.
- int getResponsesLimit() const - returns the maximum number of responses for a single request.
- std::vector<std::string> getRequests() const - returns a list of requests received from file "request.json".
- struct RelativeIndex - contains document id and rank of another answer to the request.
- void putAnswers(std::vector<std::vector<RelativeIndex>> answers) const - puts the search results, received from vector "answers" in the file "answers.json".
- std::string CheckFile(nlohmann::json &cur_file) - checks each file from the list for the number of words and correctness of the entered data.
- bool IsCorrectRequest(std::string &current_request, int &words_counter, std::vector<std::string> &requests) const - checks each request from the list for the number of words and correctness of the entered data.

#### InvertedIndex
- void updateDocumentBase(std::vector<std::string> input_docs) - counts number of words, received from input list.
- struct Entry - contains document id and rank of another word from list.
- std::vector<Entry> getWordCount(const std::string& word) const - returns a list of indexes for the specified word, regardless of the document.
- std::map<std::string, std::vector<Entry>> freq_dictionary_return - returns a list of all indexed words.

#### SearchServer
- std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input, std::map<std::string, std::vector<Entry>> _freq_dictionary, int response_limit) - returns the result of a search by words, taking into account their relativity according to the internal formula.
- std::vector<RelativeIndex> relative(std::string queries_input, std::map<std::string, std::vector<Entry>> &_freq_dictionary) const - calculates the relevancy index for each query.
- std::vector<std::string> _unique_words(std::string queries_input, std::map<std::string, std::vector<Entry>> &_freq_dictionary) const - defines unique query words.

### Starting

To start using the application, create two files "config.json" and "reguests.json". These files must be located in the folder "config_files" which is in the directory of the executable file. For example config.json, request.json and files to search already created.

**config.json example**
	
	{
      "config": {
        "name": "Search_engine",
        "version": "1.0",
        "max_responses": 5
      },
      "files": [
        "../file1.txt",
        "../file2.txt",
        "../file3.txt",
        "../file4.txt",        
      ]
    }

**requests.json example**

    {
      "requests": [
        "milk water milk",
        "latte",
        "sugar",
        "coffee"
      ]
    }

Then run SearchEngine, the result of the application will be the file "answers.json" containing a list of found words and their relativity.
	
Google tests are also connected to this program. Possible to start them from "tests/SearchEngine_test.cpp".

### Build

Step 1. Create build folder in project folder:

****
mkdir build && cd build
****

Step 2. Build project with CMake:

****
cmake .. && cmake --build . --config Release --target search_engine
****
As a result, the project file will be placed in the Release folder in the root directory. The test file will be placed in the \tests subdirectory.

Step 3. Start application:

****
.\Release\search_engine
****