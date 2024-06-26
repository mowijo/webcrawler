#include <webcrawler/HttpFetcher.h>
#include <webcrawler/Url.h>
#include <gtest/gtest.h>



#define ASSERT_EQ_W_LINENO(a, b, lineno) \
if( ! (a == b) ) { printf("Comparision failed with testcase from line %d\n", lineno); }\
ASSERT_EQ(a,b);

#define ASSERT_TRUE_W_LINENO(e, lineno) \
if( ! e ) { printf("Comparision failed with testcase from line %d\n", lineno); }\
ASSERT_TRUE(e);

#define ASSERT_FALSE_W_LINENO(e, lineno) \
if(  e ) { printf("Comparision failed with testcase from line %d\n", lineno); }\
ASSERT_FALSE(e);


#define ADD_CASE( url , expectedStatus)  cases.push_back({url, FetchResult:: expectedStatus, __LINE__});


TEST(HttpFetcher, verifyFetchGeneratesProperResultCodes)
{
    std::vector<std::tuple<std::string, FetchResult::Status, int>> cases;
    ADD_CASE( "http://google.com/" , Success) ;
    ADD_CASE( "http://domain.does.not.exist.comma.probably.comma.that.is.of.course/" , DidNotResolve) ;
    ADD_CASE( "http://localhost/" , HostUnreachable) ;


    for( const auto &[input, expected, lineno]: cases)
    {
        HttpFetcher f;

        auto result = f.fetch(std::make_shared<Url>(input));

        ASSERT_EQ_W_LINENO( result->status(), expected, lineno );
    }
}

#undef ADD_CASE



TEST(HttpFetcher, verifyGETcontent)
{
    HttpFetcher f;
    const auto result = f.fetch(std::make_shared<Url>("http://google.com/"));
    const std::size_t found = result->response().find("<A HREF=\"http://www.google.com/\">here</A>");
    ASSERT_TRUE (found!=std::string::npos);
}


TEST(HttpFetcher, verifyGETWithNoPathProvided)
{
    HttpFetcher f;
    const auto result = f.fetch(std::make_shared<Url>("http://google.com"));
    const std::size_t found = result->response().find("<A HREF=\"http://www.google.com/\">here</A>");
    ASSERT_TRUE (found!=std::string::npos);
}



