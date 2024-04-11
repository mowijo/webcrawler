#ifndef URL_H
#define URL_H
#include <string>

#include <memory>

typedef std::shared_ptr<class Url> UrlP ;

/** Class Url wraps functionality of Boost's url class and adds convenience functions and
 *  methods to detect schema and construct an absolute URL from a base URL and a path.
 *
 *  See README.for more details on that*/
class Url
{
public:

    enum Schema
    {
        OTHER = 0,

        // Relative is needed to mark an extracted HREF as relative to the base url if no
        // schema is specified. This is different from OTHER that are unsupported schemas such
        //  as mailto: and tel:
        RELATIVE,
        HTTP,
        HTTPS,
    };


    Url() = default;
    Url(const char *constData);
    Url(const std::string &constString);


    /** Named constructor.
     *  Creates a Url isntalce from a string. */
    static Url parse(const std::string url);
    bool isValid() const;

    uint16_t port;
    std::string host;
    std::string path;
    std::string query;
    Schema schema;

    std::string  schemaAsString() const;
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
