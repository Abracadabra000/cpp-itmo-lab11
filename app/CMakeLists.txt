add_executable(MakeIndex make_index.cpp)
add_executable(Search search.cpp)

target_link_libraries(MakeIndex PUBLIC invert_index reader)
target_link_libraries(Search PUBLIC search_engine reader)