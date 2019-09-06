cmake_minimum_required(VERSION 3.15)

set(target_name test_project)

set(src
    "src/main.cpp"
)

add_executable(${target_name}  ${src})
target_link_libraries(${target_name} 
   PUBLIC 
    Qt5::Core
)
