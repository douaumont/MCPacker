cmake_minimum_required(VERSION 3.22.1)
project(MCPacker)
set(CXX_STANDARD 20)
find_package(Boost 1.83.0 REQUIRED)
find_package(wxWidgets REQUIRED COMPONENTS core base)

if(wxWidgets_USE_FILE)
    include(${wxWidgets_USE_FILE})
endif()

include_directories(lib/ src/)
add_executable(${PROJECT_NAME})
target_compile_features(MCPacker PUBLIC cxx_std_20)
target_link_libraries(MCPacker ${wxWidgets_LIBRARIES} X11)
target_sources(MCPacker PUBLIC 
    src/main.cpp 
    src/core/Mod.cpp 
    src/core/ModPack.cpp 
    src/core/ModPackManager.cpp
    src/ui/MainFrame.hpp
    src/ui/MainFrame.cpp)