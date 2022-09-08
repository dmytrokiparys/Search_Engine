#include "include/SearchServer.h"




int main() {
ConverterJSON converter;
InvertedIndex index;
SearchServer search_server;

try
{
    index.UpdateDocumentBase(converter.GetTextDocuments());
    converter.putAnswers(search_server.search(converter.GetRequests(), index.freq_dictionary_return(), converter.GetResponsesLimit()));
}

catch (const std::exception& x)
{
    std::cerr<<"Caught exception: "<<x.what()<<std::endl;
}

}
