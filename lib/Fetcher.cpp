#include <webcrawler/Fetcher.h>

void FetchResult::setResponse( const std::string &response)
{
    d.response = response;
}

std::string FetchResult::response( ) const
{
    return d.response;
}


void FetchResult::setStatus( Status status )
{
    d.status = status;
}

FetchResult::Status FetchResult::status( ) const
{
    return d.status;
}


void FetchResult::setErrorMessage( const  std::string &errorMessage)
{
    d.errorMessage = errorMessage;
}

std::string FetchResult::errorMessage() const
{
    return d.errorMessage;
}

std::shared_ptr<Url> FetchResult::url() const
{
    return d.url;
}

void FetchResult::setUrl(std::shared_ptr<Url> url)
{
    d.url = url;
}


std::shared_ptr<FetchResult> Fetcher::fetch(std::shared_ptr<Url> url)
{
    return fetchImplementation(url);
}
