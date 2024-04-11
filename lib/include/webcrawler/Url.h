#ifndef URL_H
#define URL_H
#include <string>

#include <memory>

typedef std::shared_ptr<class Url> UrlP ;
class Url
{
public:

    enum Schema
    {
        OTHER = 0,
        RELATIVE,
        HTTP,
        HTTPS,
    };


    Url() = default;
    Url(const char *constData);
    Url(const std::string &constString);


    static Url parse(const std::string url);
    bool isValid() const;

    uint16_t port;
    std::string host;
    std::string path;
    std::string query;
    Schema schema;

    std::string  schemaAsString() const
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

    std::string asString() const;
    bool isFullyQualified() const;
    bool isPathAbsolute() const;


    bool operator==(const Url &other) const;


private:
    bool is_valid;
};

std::ostream& operator<<(std::ostream& stream, Url const& url);

bool shouldFollow(const Url &u);
void makeAbsoluteFromAbsoluteAndRelative(const Url &absolute, Url *relativeThatWillBeAbsolute);

#endif // URL_H
