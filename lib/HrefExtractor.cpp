#include <webcrawler/HrefExtractor.h>

#include <libxml/HTMLparser.h>
#include <libxml/tree.h>

static void extractHrefs(xmlNode *node, const char *tagName, const char *attributeName, std::set<std::string> *destination) {
    xmlChar *href;
    for (xmlNode *curNode = node; curNode; curNode = curNode->next) {
        if (curNode->type == XML_ELEMENT_NODE && xmlStrcasecmp(curNode->name, (const xmlChar *)tagName) == 0) {
            href = xmlGetProp(curNode, (const xmlChar *)attributeName);
            if (href != NULL) {

                const std::string href_as_std_string = std::string(reinterpret_cast<const char*>(href));
                destination->insert(href_as_std_string);
                xmlFree(href);
            }
        }
        extractHrefs(curNode->children, tagName, attributeName, destination);
    }
}


std::set<std::string> extractHrefs(const std::string &document)
{
    std::set<std::string>  results;
    xmlDoc *doc = htmlReadDoc(BAD_CAST document.c_str(), NULL, NULL, HTML_PARSE_RECOVER | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);
    if (doc == NULL) {
        return results;
    }

    xmlNode *root = xmlDocGetRootElement(doc);
    if (root == NULL) {
        xmlFreeDoc(doc);
        return results;
    }

    extractHrefs(root, "a", "href", &results);

    xmlFreeDoc(doc);

    return results;

}
