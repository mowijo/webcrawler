set(gtest_force_shared_crt on)

macro( create_test TESTNAME)
    add_executable( ${TESTNAME} ${TESTNAME}.cpp )
    target_link_libraries( ${TESTNAME} webcrawler gtest gtest_main)
    add_test( ${TESTNAME} ${TESTNAME} )
endmacro()


create_test( tst_linkextractor )
create_test( tst_url )
create_test( tst_http_fetcher )
create_test( tst_https_fetcher )

