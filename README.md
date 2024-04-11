# Webcrawler

## Reading Guide

First readers should read the section "Conceptual problems" that lists some problems encountered and made this a much more complex task than "Do a bunch of HTTP GET in parallel". Next the "Architecture" section and "How to Build and Run" would be a proper reads. Those sections are written in reverse order, though, to honor the tradition of READMEs.

# How to Build and Run

To build this code, you should use a linux machine with the correct packags installed *OR* you may use a docker image. You can use the script `./test_in_docker.sh` to build a suitable dockerimage, mount it as a container and build all the code and run the unittests.

If you want to build in a non virtualized environment, you should use a ubuntu 24.04 distribution and install the packages listed in `Dockerfile`.

Once a build is complete, you can run the `application/crawler` binary to fetch https://www.pixop.com/ recursively.

# Architecture

The code is split into three parts.

1. A library implementing the basic blocks needed for the project
2. A set of unittests
3. An application poviding the deliverable 

On disk it looks like this:
```
.
├── CMakeLists.txt
├── Dockerfile
├── README.md
├── test_in_docker.sh
├── application
│   ├── CMakeLists.txt
│   └── main.cpp
├── lib
│   ├── CMakeLists.txt
│   ├── Fetcher.cpp
│   ├── HrefExtractor.cpp
│   ├── HttpFetcher.cpp
│   ├── HttpsFetcher.cpp
│   ├── include
│   │   └── webcrawler
│   │       ├── Fetcher.h
│   │       ├── HrefExtractor.h
│   │       ├── HttpFetcher.h
│   │       ├── HttpsFetcher.h
│   │       └── Url.h
│   ├── root_certificates.hpp
│   └── Url.cpp
└── tests
    ├── CMakeLists.txt
    ├── google.com.html
    ├── tst_http_fetcher.cpp
    ├── tst_https_fetcher.cpp
    ├── tst_linkextractor.cpp
    ├── tst_url.cpp
    └── uzf.dk.html
```

The classes are documented in source .

## Conceptual problems

TL;DR: I ran out of time.

### It is a webbrowser!

In reality a webcrawler is a webbrowser that presents what it finds differently than a webbrowser with a GUI.

This means that a webcrawler must be able to extract and calculate urls pointed to in the resulting document, and it is not as trivial as just do an XPath query.

Urls in obtained documents can be relative, have differenc schemas, be absolute, point to all sorts of resources and constructing an absolute url from a relative one and the base url, is not straightforward.

For example does `http://foo.bar` not contain a path but is *most* of the time handled by a webserver as `http://foo.bar/`. However, to construct be sure. Some webservers, however, issues a `404` on a url without a path and as such, urls with no paths should have `/` appended I figured out. That of course results in  an absolute path from `http://foo.bar` and `/bob` to require an `/` to be appended to the base url before appending `/bob`. That results in `http://foo.bar//bob` which *some* webservers understand as `http://foo.bar/bob` and others throw a `404` back and some just linger on ignoring the request, causing problems with Issue1.

Writing a webcrawler is a completely different and more complex task thanb consuming a REST + HTTPS api.

A surprisingly large amount of time was spend on processing urls and learning about the caveats described above.

### HTML is not XML.

Note that the HTML you get back from a webserver, most likely is not XML. Therefore it took me some time to find `libxml2` that can process dirty HTML. None of the Boost modules I could find offers parsing fo dirty HTML.

### Summary of conceptual problems

I spend most of the time solving problems not related to resource management, concurrency, but because these problems needed to be solved prior to proceeding to the interesing parts in class Crawler, most of the time was spend on discovering what makes a webbrowser special.

# Issues/Known Bugs

## Issues mentioned in code

The issues below are mentioned in the code using the word `IssueX`. Grep for it.

Issue1
: The assignment specified _"Implement concurrency using `std::async` and `std::future` or `Boost.Asio` "_ and I chose `std::async` and `std::future` in over  `Boost.Asio.io_context` which had the unexpected side effect that timeouts on the network operations are not supported.

Issue2
: No mechanism for calculating the correct amoung of threads were implemented. This should be deducted and reported properly in the code. Instead a hardcoded value was used.
 Grep for `Issue2` to find the cooect spot in the code.

## Issues not mentioned in code