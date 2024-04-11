#include <webcrawler/Url.h>

#include <boost/url.hpp>

using namespace std;
#include <iostream>


Url::Url(const char *constData)
{
    *this = Url::parse(constData);
}

Url::Url(const std::string &constString)
{
    *this = Url::parse(constString);
}

bool Url::isValid() const
{
    return is_valid;
}


std::string Url::schemaAsString() const
{
    switch(schema)
    {
    case HTTPS: return "https";
    case HTTP: return "http";
    case RELATIVE: return "relative";
    default:
        break;
       }
    return "other";
}

Url Url::parse(const std::string url_s)
{
    Url u;

    u.host = "";
    u.port = 0;
    u.path = "";
    u.query = "";
    u.schema = OTHER;
    u.is_valid = false;

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
    else if (  boost_url.scheme() == "" )
    {
        u.schema = RELATIVE;
    }

    if( authority.has_port() )
    {
        u.port = authority.port_number();
    }

    u.path = boost_url.path();

    if( u.path == "")
    {
        u.path = "/";
    }

    u.is_valid = true;
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


bool Url::operator==(const Url &other) const
{
    return (
        (this->schema == other.schema )
        &&
        (this->host == other.host )
        &&
        (this->port == other.port )
        &&
        (this->path == other.path )
    );
}


std::string Url::asString() const
{
    return schemaAsString() + "://" + host + ":" + std::to_string(port) + path;
}
void makeAbsoluteFromAbsoluteAndRelative(const Url &absolute, Url *relativeThatWillBeAbsolute)
{
    relativeThatWillBeAbsolute->port = absolute.port;
    relativeThatWillBeAbsolute->schema = absolute.schema;
    relativeThatWillBeAbsolute->host = absolute.host;
    std::string newPath = absolute.path + "/" + relativeThatWillBeAbsolute->path;
    relativeThatWillBeAbsolute->path = newPath;
}


std::ostream& operator<<(std::ostream& stream, Url const& url)
{
    return stream << url.asString();
}
