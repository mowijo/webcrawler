#include <webcrawler/HttpsFetcher.h>
#include<webcrawler/Url.h>
int main(int argc, char *argv[])
{

    std::shared_ptr<Fetcher> fetcher = std::make_shared<HttpsFetcher>();
    fetcher->fetch(std::make_shared<Url>("https://version2.dk/"));
    return 1;
}
