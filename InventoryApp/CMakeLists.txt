cmake_minimum_required(VERSION 3.10)

project(InventoryApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt package
find_package(Qt6 COMPONENTS Widgets Sql PrintSupport REQUIRED)

# Add source files
set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/DatabaseManager.cpp
    src/InventoryManager.cpp
    src/ReportGenerator.cpp
    src/Component.cpp
)

set(HEADERS
    include/MainWindow.h
    include/DatabaseManager.h
    include/InventoryManager.h
    include/ReportGenerator.h
    include/Component.h
)

# Create executable
add_executable(InventoryApp ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(InventoryApp PRIVATE Qt6::Widgets Qt6::Sql Qt6::PrintSupport)

# Setup include directories
target_include_directories(InventoryApp PRIVATE include)

# Handle Window application on Windows (removes console window)
if(WIN32)
    set_property(TARGET InventoryApp PROPERTY WIN32_EXECUTABLE ON)
endif()
