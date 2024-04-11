#ifndef HREF_EXTRACTOR_H
#define HREF_EXTRACTOR_H

#include <set>
#include <string>


/** Provided a HTML document, this function returns a set of urls found in the document.

The urls are kept as string and may be relative, absolute and of any schema and point to any content type.

*/
std::set<std::string> extractHrefs(const std::string &document);


#endif // HREF_EXTRACTOR_H

