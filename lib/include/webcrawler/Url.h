#ifndef URL_H
#define URL_H
#include <string>

class Url
{
public:

    enum Schema
    {
        OTHER = 0,
        HTTP,
        HTTPS,
    };

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
        default:
            break;
           }
        return "other";
    }

    bool isFullyQualified() const;
    bool isPathAbsolute() const;


private:
    bool is_valid;


};

bool shouldFollow(const Url &u);

#endif // URL_H
