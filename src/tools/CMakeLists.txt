add_executable(ast-tool ast_tool.cpp)

# Link Catch2 to tests (if needed)
if(CATCH_FOUND)
    target_link_libraries(ast-tool PRIVATE Catch2::Catch2WithMain)
endif()
