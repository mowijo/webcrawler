#include <webcrawler/Url.h>

#include <boost/url.hpp>

// libboost-url1.81-dev
using namespace std;
#include <iostream>

Url Url::parse(const std::string url_s)
{
    Url u;

    u.host = "";
    u.port = 0;
    u.path = "";
    u.query = "";
    u.schema = OTHER;

    boost::url boost_url(url_s);
    auto const authority = boost_url.authority();

    u.host = authority.host_name();
    if(  boost_url.scheme() == "http" )
    {
        u.schema = HTTP;
        u.port = 80;
    }
    else if (  boost_url.scheme() == "https" )
    {
        u.schema = HTTPS;
        u.port = 443;
    }
    else
    {
        u.schema = OTHER;
    }

    if( authority.has_port() )
    {
        u.port = authority.port_number();
    }

    u.path = boost_url.path();

    return u;
}



bool Url::isFullyQualified() const
{
    if(host != "")
    {
        return true;
    }
    return false;
}

bool Url::isPathAbsolute() const
{
    return path[0] == '/';
}

bool shouldFollow(const Url &u)
{
    if ( ! u.isPathAbsolute() )
    {
        return true;
    }

    if(u.schema == Url::OTHER)
    {
        return false;
    }

    if(u.host == "")
    {
        return false;
    }

    if(u.path == "")
    {
        return false;
    }

    return true;

}
