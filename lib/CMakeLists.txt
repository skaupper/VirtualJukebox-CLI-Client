add_library(project_libs OBJECT # httplib
            httplib/httplib.cc httplib/httplib.h)

target_link_libraries(project_libs PRIVATE lib_options)
target_include_directories(project_libs INTERFACE .)
