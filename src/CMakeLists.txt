add_executable(lox 
    main.cpp
    interpreter/Scanner.h
    interpreter/Scanner.cpp 
    interpreter/Tokens.h
    interpreter/Tokens.cpp)

set_target_properties(lox PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")  # Set the build directory for the executable

# Link Catch2 to tests (if needed)
if(CATCH_FOUND)
    target_link_libraries(lox PRIVATE Catch2::Catch2WithMain)
endif()
