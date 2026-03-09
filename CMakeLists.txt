cmake_minimum_required(VERSION 3.20)
project(ProjectSkyblock)

# Force the compiler to use modern C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Create the executable from your main.cpp file
add_executable(ProjectSkyblock
        main.cpp
        dataToken.cpp
        itemHour.cpp
        itemHour.h
        SkyblockItem.cpp
        SkyblockItem.h
        lib/sqlite/sqlite3.c
        DataBaseManager.cpp
        DataBaseManager.h
        PriceObject.cpp
        PriceObject.h
)

# 2. Tell CMake where to find your downloaded nlohmann/json.hpp
target_include_directories(ProjectSkyblock PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib)

# 3. Safely find and link libcurl (The professional way)
find_package(CURL REQUIRED)
target_include_directories(ProjectSkyblock PRIVATE ${CURL_INCLUDE_DIR})
target_link_libraries(ProjectSkyblock PRIVATE ${CURL_LIBRARIES})