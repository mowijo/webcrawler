#include <webcrawler/HttpsFetcher.h>

#include "root_certificates.hpp"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <cstdlib>
#include <iostream>
#include <string>

namespace beast = boost::beast; // from <boost/beast.hpp>
namespace http = beast::http;   // from <boost/beast/http.hpp>
namespace net = boost::asio;    // from <boost/asio.hpp>
namespace ssl = net::ssl;       // from <boost/asio/ssl.hpp>
using tcp = net::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

#include <webcrawler/Url.h>

HttpsFetcher::HttpsFetcher()
{
}

std::shared_ptr<FetchResult> HttpsFetcher::fetchImplementation(std::shared_ptr<Url> url)
{
    auto r = std::make_shared<FetchResult>();
    r->setStatus(FetchResult::Success);
    r->setUrl(url);

    try
    {

        const int version = 11;

        // The io_context is required for all I/O
        net::io_context ioc;

        // The SSL context is required, and holds certificates
        ssl::context ctx(ssl::context::tlsv12_client);

        // This holds the root certificate used for verification
        load_root_certificates(ctx);

        // Verify the remote server's certificate
        ctx.set_verify_mode(ssl::verify_peer);

            // These objects perform our I/O
        tcp::resolver resolver(ioc);
        beast::ssl_stream<beast::tcp_stream> stream(ioc, ctx);

        // Set SNI Hostname (many hosts need this to handshake successfully)

        if(! SSL_set_tlsext_host_name(stream.native_handle(), url->host.c_str()))
        {
            beast::error_code ec{static_cast<int>(::ERR_get_error()), net::error::get_ssl_category()};
            throw beast::system_error{ec};
        }

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



           try
           {
               beast::get_lowest_layer(stream).connect(results);
           }
           catch( boost::system::system_error se )
           {
               r->setStatus(FetchResult::HostUnreachable);
               return r;
           }




        // Make the connection on the IP address we get from a lookup

        // Perform the SSL handshake
        stream.handshake(ssl::stream_base::client);

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

        r->setResponse(boost::beast::buffers_to_string(res.body().data()));

        // Gracefully close the stream
        beast::error_code ec;
        stream.shutdown(ec);
        if(ec == net::error::eof)
        {
            // Rationale:
            // http://stackoverflow.com/questions/25587403/boost-asio-ssl-async-shutdown-always-finishes-with-an-error
            ec = {};
        }
        if(ec)
            throw beast::system_error{ec};

        // If we get here then the connection is closed gracefully
    }
    catch(std::exception const& e)
    {
        if(strcmp(e.what() , "stream truncated [asio.ssl.stream:1]") == 0)
        {
            //This just happens on a lot of servers. Technically an error, but ... See
            // https://i.kym-cdn.com/entries/icons/original/000/028/596/dsmGaKWMeHXe9QuJtq_ys30PNfTGnMsRuHuo_MUzGCg.jpg

        }
        else
        {
            std::cerr << "FAILURE!!!!! Error: " << e.what() << std::endl;
        }
    }

    return r;
}

