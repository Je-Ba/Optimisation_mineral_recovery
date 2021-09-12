cmake_minimum_required(VERSION 3.18.4)
project(Optimisation)

set(CMAKE_CXX_STANDARD 14)

if(UNIX)
	set(LINUX TRUE)
endif()

if(LINUX)
	install(DIRECTORY DESTINATION ${directory})

	include_directories(includes)

	find_package(MPI)

	include_directories(SYSTEM ${MPI_INCLUDE_PATH})

	add_executable(main
		includes/CCircuit.h
		includes/CUnit.h
		includes/Evaluate_Circuit.h
		includes/Genetic_Algorithm.h
		src/Evaluate_Circuit.cpp
		src/CCircuit.cpp
		src/Genetic_Algorithm.cpp
		src/main.cpp
		)
	set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY "executables")

	add_executable(test_GeneticAlgorithm
		includes/CCircuit.h
		includes/CUnit.h
		includes/Evaluate_Circuit.h
		includes/Genetic_Algorithm.h
		src/Evaluate_Circuit.cpp
		src/CCircuit.cpp
		src/Genetic_Algorithm.cpp
		tests/test_GeneticAlgorithm.cpp
		)
	set_target_properties(test_GeneticAlgorithm PROPERTIES RUNTIME_OUTPUT_DIRECTORY "executables")

	add_executable(test1
		includes/CUnit.h
		includes/CCircuit.h
		src/CCircuit.cpp
		tests/test1.cpp
		)
	set_target_properties(test1 PROPERTIES RUNTIME_OUTPUT_DIRECTORY "executables")

	add_executable(test2
		includes/CCircuit.h
		includes/CUnit.h
		includes/Evaluate_Circuit.h
		includes/Genetic_Algorithm.h
		src/Evaluate_Circuit.cpp
		src/CCircuit.cpp
		src/Genetic_Algorithm.cpp
		tests/test2.cpp
			)
	set_target_properties(test2 PROPERTIES RUNTIME_OUTPUT_DIRECTORY "executables")
else()
	include_directories(includes)
	add_executable(main
		includes/CCircuit.h
		includes/CUnit.h
		includes/Evaluate_Circuit.h
		includes/Genetic_Algorithm.h
		src/Evaluate_Circuit.cpp
		src/CCircuit.cpp
		src/Genetic_Algorithm.cpp
		src/main.cpp
		)

	add_executable(test_GeneticAlgorithm
		includes/CCircuit.h
		includes/CUnit.h
		includes/Evaluate_Circuit.h
		includes/Genetic_Algorithm.h
		src/Evaluate_Circuit.cpp
		src/CCircuit.cpp
		src/Genetic_Algorithm.cpp
		tests/test_GeneticAlgorithm.cpp
		)
	
	add_executable(test1
		includes/CUnit.h
		includes/CCircuit.h
		src/CCircuit.cpp
		tests/test1.cpp
		)

	add_executable(test2
		includes/CCircuit.h
		includes/CUnit.h
		includes/Evaluate_Circuit.h
		includes/Genetic_Algorithm.h
		src/Evaluate_Circuit.cpp
		src/CCircuit.cpp
		src/Genetic_Algorithm.cpp
		tests/test2.cpp
			)
endif()





FIND_PACKAGE( OpenMP REQUIRED)
if(OPENMP_FOUND)
    message("OPENMP FOUND")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")

endif()