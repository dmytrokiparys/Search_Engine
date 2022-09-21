#include "SearchServer.h"

int main() {

    ConverterJSON converter;
    InvertedIndex index;
    SearchServer search_server;

    try {
        index.UpdateDocumentBase(converter.GetTextDocuments());
        converter.PutAnswers(search_server.Search(converter.GetRequests(), index.FreqDictionaryReturn(),converter.GetResponsesLimit()));
    }
    catch (const std::exception &x) {
        std::cerr << "Caught exception: " << x.what() << std::endl;
    }
}
