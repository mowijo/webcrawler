#ifndef FETCHER_H
#define FETCHER_H

#include <memory>

class Url;

class FetchResult
{

public:

    enum Status
    {
        Success = 0,
        DidNotResolve,
        HostUnreachable,
        HttpError,
        Timeout,
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

class Fetcher
{
public:

    virtual ~Fetcher() = default;
    std::shared_ptr<FetchResult> fetch(std::shared_ptr<Url> url);

private:
    virtual std::shared_ptr<FetchResult> fetchImplementation(std::shared_ptr<Url> url) = 0;


};

#endif // FETCHER_H
