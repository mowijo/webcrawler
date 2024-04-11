#include <iostream>
#include <future>
#include <vector>
#include <chrono>
#include <unordered_set>
typedef std::string Url;



class Crawler
{
public:
    void crawl(Url url, int maxDepth );
private:

    struct VisitResult
    {
        Url visited;
        std::unordered_set<Url> nextUrls;
        int depth;

    };


    std::shared_ptr<VisitResult> visitBlocking( Url i, int currentDepth);
    void perhapsQueue(Url u, int depth);

    int maximumConcurrentVisits() const;
    int maxDepth ;

    std::vector<std::future<std::shared_ptr<VisitResult> >> futures;
    std::vector<std::tuple<Url, int>> queue;
    std::unordered_set<Url> alreadyVisited;
    std::unordered_set<Url> inProgressBeingVisited;



};

int Crawler::maximumConcurrentVisits() const
{
    return 3;
}


std::shared_ptr<Crawler::VisitResult> Crawler::visitBlocking(Url u, int currentDepth)
{
    std::this_thread::sleep_for(std::chrono::seconds(rand() % 3)); // Simulate work
    auto result = std::make_shared<VisitResult>();
    result->depth = currentDepth+1;
    result->visited = u;
    if(currentDepth == 0)
    {
        result->nextUrls = {std::to_string(1),std::to_string(2),std::to_string(3)};
    }
    else
    {
        result->nextUrls = {std::to_string(1), std::to_string(1*(currentDepth+1)),std::to_string(2*(currentDepth+1)),std::to_string(3*(currentDepth+1))};
    }
    return result;
}

void Crawler::perhapsQueue(Url u, int depth)
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

void Crawler::crawl(Url url, int maxDepth )
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
                    std::cout << "Visited  " << result->visited << std::endl;
                    inProgressBeingVisited.erase(result->visited);
                    alreadyVisited.insert(result->visited);
                    for(auto u : result->nextUrls)
                    {
                        perhapsQueue(u, result->depth);
                    }

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
            const Url url = std::get<0>(newVisit);

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
    crawler.crawl("1", 10);

    return 0;
}
