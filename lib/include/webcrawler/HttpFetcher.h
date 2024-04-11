#ifndef HTTPFETCHER_H
#define HTTPFETCHER_H

#include <webcrawler/Fetcher.h>

/** Implements fetching of HTTPS resources.*/
class HttpFetcher : public Fetcher
{
public:
    HttpFetcher();
    ~HttpFetcher() = default;

private:
    std::shared_ptr<FetchResult> fetchImplementation(std::shared_ptr<Url> url) override;
};



#endif // HTTPFETCHER_H
