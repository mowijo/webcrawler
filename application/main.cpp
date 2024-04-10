#include <webcrawler/HttpFetcher.h>
#include <webcrawler/Url.h>

#include <iostream>

int main(int argc, char** argv)
{

    auto f = std::make_shared<HttpFetcher>();
    auto r = f->fetch(std::make_shared<Url>("http://google.com"));
    std::cout << r->response() << std::endl;
    return EXIT_SUCCESS;
}
