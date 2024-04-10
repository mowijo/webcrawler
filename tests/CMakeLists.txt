set(gtest_force_shared_crt on)

add_subdirectory ( googletest )

macro( create_test TESTNAME)
    add_executable( ${TESTNAME} ${TESTNAME}.cpp )
    target_link_libraries( ${TESTNAME} webcrawler gtest gtest_main)
    add_test( ${TESTNAME} ${TESTNAME} )
endmacro()


create_test( tst_linkextractor )
