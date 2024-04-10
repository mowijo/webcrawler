#include <webcrawler/Url.h>

#include <gtest/gtest.h>

#define ADD_CASE( url)  cases.push_back({url, __LINE__});

TEST(UrlTest, verifyShouldFollow)
{
    std::vector<std::tuple<std::string, int>> cases;
    ADD_CASE( "http://google.com/") ;
    ADD_CASE( "https://google.com/") ;
    ADD_CASE( "https://google.com") ;
    ADD_CASE( "http://google.com") ;

    ADD_CASE( "google.com/") ;  // Yes. This is a relative path to whatever we are parsing right now.

    for( const auto &[input, lineno]: cases)
    {
        Url u = Url::parse(input);
        const bool actual = shouldFollow(u);
        if( ! actual  )
        {
            printf("Failure in case in case '%s'from line %d\n", input.c_str(), lineno);
        }
        ASSERT_TRUE( actual );
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
        if(  actual  )
        {
            printf("Failure in case in case '%s'from line %d\n", input.c_str(), lineno);

            std::cout << u.schemaAsString() << std::endl;
            std::cout << u.host << std::endl;
            std::cout << u.port << std::endl;
            std::cout << u.path << std::endl;
            std::cout << u.query << std::endl;

        }
        ASSERT_FALSE( actual );
    }

}



#undef ADD_CASE
