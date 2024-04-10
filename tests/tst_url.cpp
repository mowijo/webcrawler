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


#define ADD_CASE( url)  cases.push_back({url, __LINE__});

TEST(UrlTest, verifyShouldFollow)
{
    std::vector<std::tuple<std::string, int>> cases;
    ADD_CASE( "http://google.com/") ;
    ADD_CASE( "https://google.com/") ;
    ADD_CASE( "https://google.com") ;
    ADD_CASE( "http://google.com") ;

    ADD_CASE( "google.com/") ;  // Yes. This is a relative path to whatever we are processing right now.

    for( const auto &[input, lineno]: cases)
    {
        Url u = Url::parse(input);
        const bool actual = shouldFollow(u);
        ASSERT_TRUE_W_LINENO( actual , lineno );
    }

}

TEST(UrlTest, verifyShouldNotFollow)
{
    std::vector<std::tuple<std::string, int>> cases;
    ADD_CASE( "ftp://google.com/") ;


    for( const auto &[input, lineno]: cases)
    {
        Url u = Url::parse(input);
        const bool actual = shouldFollow(u);
        ASSERT_FALSE_W_LINENO( actual , lineno );
    }

}

#undef ADD_CASE


#define ADD_CASE( absolute, relative, expected )  cases.push_back({absolute, relative, expected , __LINE__});

TEST(UrlTest, verifyMakeRelativeToAnAbsoluteAbsolute)
{
    std::vector<std::tuple<std::string, std::string, std::string, int>> cases;

    ADD_CASE("http://foo.bar/asdf", "qwer", "http://foo.bar/asdf/qwer");    //Here, qwer is realtive to asdf.


    for( const auto &[absolute, relative, expected, lineno]: cases)
    {
        Url a = Url::parse(absolute);
        Url r = Url::parse(relative);
        Url e = Url::parse(expected);

        makeAbsoluteFromAbsoluteAndRelative(a, &r);

        ASSERT_EQ_W_LINENO(r, e, lineno);
    }
}

#undef ADD_CASE
