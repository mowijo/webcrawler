#include <webcrawler/HttpFetcher.h>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>
#include <iostream>
#include <string>

namespace beast = boost::beast;     // from <boost/beast.hpp>
namespace http = beast::http;       // from <boost/beast/http.hpp>
namespace net = boost::asio;        // from <boost/asio.hpp>
using tcp = net::ip::tcp;           // from <boost/asio/ip/tcp.hpp>

#include <webcrawler/Url.h>

HttpFetcher::HttpFetcher()
{
}

std::shared_ptr<FetchResult> HttpFetcher::fetchImplementation(std::shared_ptr<Url> url)
{

    auto r = std::make_shared<FetchResult>();
    r->setUrl(url);
    try
    {

        const int version = 11;

        // The io_context is required for all I/O
        net::io_context ioc;

        // These objects perform our I/O
        tcp::resolver resolver(ioc);
        beast::tcp_stream stream(ioc);

        // Look up the domain name

        boost::asio::ip::basic_resolver<boost::asio::ip::tcp>::results_type results;
        try
        {
            results = resolver.resolve(url->host, std::to_string(url->port));
        }
        catch( ... )
        {
            r->setStatus(FetchResult::DidNotResolve);
            return r;
        }


        // Make the connection on the IP address we get from a lookup
        stream.connect(results);

        // Set up an HTTP GET request message
        http::request<http::string_body> req{http::verb::get, url->path, version};
        req.set(http::field::host, url->host);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

        // Send the HTTP request to the remote host
        http::write(stream, req);

        // This buffer is used for reading and must be persisted
        beast::flat_buffer buffer;

        // Declare a container to hold the response
        http::response<http::dynamic_body> res;

        // Receive the HTTP response
        http::read(stream, buffer, res);

        // Write the message to standard out
        r->setResponse(boost::beast::buffers_to_string(res.body().data()));

        // Gracefully close the socket
        beast::error_code ec;
        stream.socket().shutdown(tcp::socket::shutdown_both, ec);

        // not_connected happens sometimes
        // so don't bother reporting it.
        //
        if(ec && ec != beast::errc::not_connected)
            throw beast::system_error{ec};

        // If we get here then the connection is closed gracefully
        r->setStatus(FetchResult::Success);
    }
    catch(std::exception const& e)
    {
        r->setStatus(FetchResult::UnknownError);
        r->setErrorMessage(e.what());
    }
    catch( ... )
    {
        r->setStatus(FetchResult::UnknownError);
        r->setErrorMessage("Unknwon stuff thrown. Juggles..");
    }
    return r;
}

