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
            "milk water milk",
            "latte",
            "sugar",
            "coffee",
            "water coffee"
    };
    std::vector<std::string> docs =
            {
                    "milk milk milk milk water water water coffee",
                    "milk water water sugar sugar",
                    "milk milk milk milk milk water water water water water milk coffee",
                    "americano cappuccino water sugar"
            };

    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    SearchServer srv;
    ConverterJSON conv;
    auto result = srv.Search(requests, idx.FreqDictionaryReturn(), conv.GetResponsesLimit());
    std::vector<std::vector<RelativeIndex>> excepted{
            {
                    {2, 1.0},
                    {0, 0.6363636255264282},
                    {1, 0.27272728085517883}
            },
            {

            },
            {
                    {1, 1.0},
                    {3, 0.5}
            },
            {
                    {0, 1.0},
                    {2, 1.0}
            },
            {
                    {2, 1.0},
                    {0, 0.800000011920929},
                    {1, 0.4000000059604645},
                    {3, 0.20000000298023224}
            }

    };
    EXPECT_EQ(result, excepted);
}





