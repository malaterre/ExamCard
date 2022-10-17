cmake_minimum_required(VERSION 3.24)
project(ExamCard C)

add_executable(dump dump.c)
set_property(TARGET dump PROPERTY C_STANDARD 11)
