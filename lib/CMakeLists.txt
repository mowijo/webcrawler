cmake_minimum_required(VERSION 3.20 )

add_library( webcrawler
    Fetcher.cpp
    HttpFetcher.cpp
    HttpsFetcher.cpp
    Url.cpp
)

target_link_libraries( webcrawler boost_url boost_system )
target_include_directories( webcrawler  PUBLIC include )
