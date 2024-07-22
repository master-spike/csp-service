set(tests 
    interval_set_test.cpp
)

add_executable(
    csplibtest ${tests}
)

target_link_libraries(csplibtest csplib doctest::doctest)

add_test(NAME csplibtest COMMAND $<TARGET_FILE:csplibtest>)
