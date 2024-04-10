cmake_minimum_required(VERSION 3.20 )

add_library( webcrawler 
	HttpFetcher.cpp
)

target_include_directories( webcrawler  PUBLIC include )
