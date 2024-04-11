add_executable( crawl main.cpp)
find_package(LibXml2 REQUIRED)
target_link_libraries ( crawl webcrawler ${LIBXML2_LIBRARIES})
target_include_directories(crawl  PRIVATE ${LIBXML2_INCLUDE_DIR})
