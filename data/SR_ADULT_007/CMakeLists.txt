cmake_minimum_required(VERSION 3.24)
project(ExamCard C)

add_executable(dump dump.c)
set_property(TARGET dump PROPERTY C_STANDARD 11)

add_executable(get get.c)
set_property(TARGET get PROPERTY C_STANDARD 11)

add_executable(dump2 dump2.c)
set_property(TARGET dump2 PROPERTY C_STANDARD 11)

add_executable(test test.c)
set_property(TARGET test PROPERTY C_STANDARD 11)

find_package(LibXml2 2.9 MODULE REQUIRED)
add_executable(xpath1 xpath1.c)
target_link_libraries(xpath1 PRIVATE LibXml2::LibXml2)

add_executable(examcard2xml examcard2xml.c)
target_link_libraries(examcard2xml PRIVATE LibXml2::LibXml2)
