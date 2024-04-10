#ifndef HTTPS_FETCHER_H
#define HTTPS_FETCHER_H

#include <webcrawler/Fetcher.h>

class HttpsFetcher : public Fetcher
{
public:
    HttpsFetcher();
    ~HttpsFetcher() = default;

private:
    std::shared_ptr<FetchResult> fetchImplementation(std::shared_ptr<Url> url) override;
};

#endif // HTTPS_FETCHER_H
