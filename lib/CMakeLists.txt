cmake_minimum_required(VERSION 3.20 )

find_package(LibXml2 REQUIRED)

add_library( webcrawler
    Fetcher.cpp
    HrefExtractor.cpp
    HttpFetcher.cpp
    HttpsFetcher.cpp
    Url.cpp
)

target_link_libraries( webcrawler boost_url boost_system ssl crypto ${LIBXML2_LIBRARIES})
target_include_directories( webcrawler  PUBLIC include )
target_include_directories( webcrawler  PRIVATE ${LIBXML2_INCLUDE_DIR})
