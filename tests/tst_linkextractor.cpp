#include <webcrawler/HttpFetcher.h>

#include <gtest/gtest.h>

TEST(CalculatorTest, verify_addition_works) {



    HttpFetcher hf;
    hf.fetch();
    EXPECT_EQ(1, 1);
}
