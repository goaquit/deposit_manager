set(target_name "deposit_manager_old")

set(src
    "src/main.cpp"
	"src/deposit_manager.cpp"
)

add_executable(${target_name}  ${src})
target_include_directories(${target_name} 
	PUBLIC 
		"include"
)
