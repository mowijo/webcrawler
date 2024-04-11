#ifndef FETCHER_H
#define FETCHER_H

#include <memory>
#include <string>

class Url;



/** Class FetchResult is a class holding information about a downloading of a resource on the internet.
It contains the downloaded data, where it came from, a success status and possibly an error message*/
class FetchResult
{

public:

    enum Status
    {
        Success = 0,
        DidNotResolve,
        HostUnreachable,    //Can both be host AND/OR port
        HttpError,
        UnknownError
    };

    void setResponse( const std::string &response);
    std::string response( ) const;

    void setStatus( Status status );
    Status status( ) const;

    void setErrorMessage( const  std::string &errorMessage);
    std::string errorMessage() const;

    std::shared_ptr<Url> url() const;
    void setUrl(std::shared_ptr<Url> url);


private:
    struct {
        std::string response;
        Status status;
        std::string errorMessage;
        std::shared_ptr<Url> url;
    } d;
};

/** Class fetcher is an abstract base class for Fetchers, that is classes that are capable of downloading resources from the internet.*/
class Fetcher
{
public:

    virtual ~Fetcher() = default;
    std::shared_ptr<FetchResult> fetch(std::shared_ptr<Url> url);

private:
    virtual std::shared_ptr<FetchResult> fetchImplementation(std::shared_ptr<Url> url) = 0;


};

#endif // FETCHER_H
