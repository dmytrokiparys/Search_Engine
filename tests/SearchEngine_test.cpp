#include "SearchServer.h"
#include "gtest/gtest.h"

TEST(SearchEngine, search1) {
    const std::vector<std::string> docs = {
            "milk milk milk milk water water water",
            "milk water water",
            "milk milk milk milk milk water water water water water",
            "americano cappuccino"
    };
    const std::vector<std::string> request = {"milk water", "sugar"};
    const std::vector<std::vector<RelativeIndex>> expected = {
            {
                    {2, 1.0},
                    {0, 0.699999988079071},
                    {1, 0.30000001192092896}
            },
            {

            }
    };
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    SearchServer srv;
    ConverterJSON conv;
    std::vector<std::vector<RelativeIndex>> result = srv.Search(request, idx.FreqDictionaryReturn(),conv.GetResponsesLimit());
    ASSERT_EQ(result, expected);
}

TEST(SearchEngine, search2) {
    std::vector<std::string> requests{
            "london",
            "the"
    };
    std::vector<std::string> docs =
            {
                    "london is the capital of great britain",
                    "big ben is the nickname for the great bell of the striking clock"
            };

    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    SearchServer srv;
    ConverterJSON conv;
    auto result = srv.Search(requests, idx.FreqDictionaryReturn(), conv.GetResponsesLimit());
    std::vector<std::vector<RelativeIndex>> excepted{
            {
                    {0, 1.0}
            },
            {
                    {1, 1.0},
                    {0, 0.3333333432674408}
            }
    };

    EXPECT_EQ(result, excepted);
}

TEST(SearchEngine, search3) {
    std::vector<std::string> requests{
            "kiev is the capital of ukraine",
            "other",
    };
    std::vector<std::string> docs =
            {
                    "london is the capital of great britain",
                    "moscow is the capital of russia",
                    "kiev is the capital of ukraine",
                    "welcome to the kiev the capital of ukraine",
                    "amsterdam is the capital of netherlands",
            };

    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    SearchServer srv;
    ConverterJSON conv;
    auto result = srv.Search(requests, idx.FreqDictionaryReturn(), conv.GetResponsesLimit());
    std::vector<std::vector<RelativeIndex>> excepted{
            {
                    {2, 1},
                    {3, 1},
                    {0, 0.666666687},
                    {1, 0.666666687},
                    {4, 0.666666687}
            },
            {

            }
    };
    EXPECT_EQ(result, excepted);
}





