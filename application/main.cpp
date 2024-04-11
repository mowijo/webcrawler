#include <iostream>
#include <future>
#include <vector>
#include <chrono>
#include <unordered_set>

#include <webcrawler/Url.h>
#include <webcrawler/HttpFetcher.h>
#include <webcrawler/HttpsFetcher.h>
#include <webcrawler/HrefExtractor.h>

typedef std::string webaddress;

class Crawler
{
public:
    void crawl(webaddress url, int maxDepth );
private:

    struct VisitResult
    {
        webaddress addressVisited;
        bool wasVisisted;
        std::unordered_set<webaddress> nextUrls;
        int depth;

    };


    std::shared_ptr<VisitResult> visitBlocking( webaddress i, int currentDepth);
    void perhapsQueue(webaddress u, int depth);

    int maximumConcurrentVisits() const;
    int maxDepth ;

    std::vector<std::future<std::shared_ptr<VisitResult> >> futures;
    std::vector<std::tuple<webaddress, int>> queue;
    std::unordered_set<webaddress> alreadyVisited;
    std::unordered_set<webaddress> inProgressBeingVisited;



};

int Crawler::maximumConcurrentVisits() const
{
    // Issue2
    return 3;
}


std::shared_ptr<Crawler::VisitResult> Crawler::visitBlocking(webaddress u, int currentDepth)
{
    std::shared_ptr<Url> currentUrl = std::make_shared<Url>(u);

    auto result = std::make_shared<VisitResult>();
    result->wasVisisted = false;

    if( ! currentUrl->isValid() )
    {
        return result;
    }
    std::shared_ptr<Fetcher> fetcher;
    if(currentUrl->schema == Url::HTTPS)
    {
        fetcher = std::make_shared<HttpsFetcher>();
    }
    else if(currentUrl->schema == Url::HTTP)
    {
        fetcher = std::make_shared<HttpFetcher>();
    }
    else
    {
        return result;
    }
    std::cout << "Fetches " << currentUrl->asString() << std::endl;
    auto fetchResult = fetcher->fetch(currentUrl);
    std::cout << "Done fetching " << currentUrl->asString() << std::endl;
    if(fetchResult->status() != FetchResult::Success)
    {
        return result;
    }

    const std::string document = fetchResult->response();
    std::set<std::string> nextUrls = extractHrefs(document);

    for(auto UUUU: nextUrls)
    {
        std::shared_ptr<Url> nextUrl = std::make_shared<Url>(UUUU);
        if( nextUrl->schema == Url::RELATIVE)
        {
            makeAbsoluteFromAbsoluteAndRelative(*currentUrl.get(), nextUrl.get());
        }

        if( nextUrl->schema == Url::OTHER)
        {
            // Ignore
        }
        else
        {
            result->nextUrls.insert(nextUrl->asString());
        }
    }

    result->depth = currentDepth+1;
    result->addressVisited = u;
    result->wasVisisted = true;
    return result;
}

void Crawler::perhapsQueue(webaddress u, int depth)
{
    if ( depth > maxDepth )
    {
        return ;
    }

    if ( alreadyVisited.count(u) > 0)
    {
        return;
    }

    if(inProgressBeingVisited.count(u) > 0)
    {
        return ;
    }

    inProgressBeingVisited.insert(u);
    queue.push_back({u, depth});
}

void Crawler::crawl(webaddress url, int maxDepth )
{

    this->maxDepth = maxDepth;

    perhapsQueue(url, 0);

    while(!queue.empty() || !futures.empty())
    {

        while (!futures.empty())
        {
            for (auto it = futures.begin(); it != futures.end();)
            {
                auto status = it->wait_for(std::chrono::seconds(0)); // Check if the future is ready

                if (status == std::future_status::ready)
                {

                    auto result = it->get();
                    if(result->wasVisisted)
                    {
                        std::cout << "Visited  " << result->addressVisited << std::endl;
                        for(auto u : result->nextUrls)
                        {
                            perhapsQueue(u, result->depth);
                        }
                    }

                    inProgressBeingVisited.erase(result->addressVisited);
                    alreadyVisited.insert(result->addressVisited);
                    it = futures.erase(it); // Remove the ready future
                }
                else if (status == std::future_status::timeout)
                {
                    ++it; // Future not ready yet, move to the next one
                }
                else
                {
                    // Error occurred while waiting for future
                    it = futures.erase(it);
                }
            }
        }

        while( futures.size() <= maximumConcurrentVisits() && queue.size() > 0)
        {
            auto newVisit = queue.back();
            queue.pop_back();

            const int visitDepth = std::get<1>(newVisit);
            const webaddress url = std::get<0>(newVisit);

            futures.push_back(std::async(std::launch::async, [=]()
            {
                return this->visitBlocking(url, visitDepth);
            }));

        }

    }


}


int main()
{
    srand (time(NULL));

    Crawler crawler;
    crawler.crawl("https://www.pixop.com/", 3);

    return 0;
}
