#include <webcrawler/HrefExtractor.h>

#include <gtest/gtest.h>

TEST(HrefExtractor, verify_extraction_works) {
    #include "google.com.html"
    const auto results = extractHrefs(html);
    ASSERT_EQ(results, std::set<std::string>{"http://www.google.com/"});
}



TEST(HrefExtractor, verify_extraction_works_full_webpage) {
    #include "uzf.dk.html"
    const auto results = extractHrefs(html);
    const std::set<std::string> expected{"#aboutMeSection", "#contactMe", "#skip-link", "#slide-1", "#slide-2", "#slide-3", "#slide-4", "#testimonialsSection", "#whatDoIOffer", "tel:+4542738310" };
    ASSERT_EQ(results, expected);
}
