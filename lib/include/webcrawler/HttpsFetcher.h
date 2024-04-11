#ifndef HTTPS_FETCHER_H
#define HTTPS_FETCHER_H

#include <webcrawler/Fetcher.h>

/** Implements fetching of HTTPS resources.

Note, that this class has a limited set of root certificates bundled and you may end up trying to
fetch a resource signed at toplevel using a certificate not present here.
*/
class HttpsFetcher : public Fetcher
{
public:
    HttpsFetcher();
    ~HttpsFetcher() = default;

private:
    std::shared_ptr<FetchResult> fetchImplementation(std::shared_ptr<Url> url) override;
};

#endif // HTTPS_FETCHER_H
