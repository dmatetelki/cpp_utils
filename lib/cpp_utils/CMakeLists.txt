include_directories (.)
aux_source_directory(. CPPUTILS_SOURCES)

add_library (CppUtils SHARED ${CPPUTILS_SOURCES})
target_link_libraries(CppUtils pthread rt gcov ssl mysqlclient)