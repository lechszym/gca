include_directories(${gca_SOURCE_DIR}/gca)

add_executable(test_blade test_blade.cpp)

add_executable(test_mvec test_mvec.cpp)

add_executable(test_blades test_genb.cpp test_blades.cpp)

add_executable(test_mvecs test_genb.cpp test_mvecs.cpp)

add_executable(test_vectors test_genb.cpp test_vectors.cpp)

add_executable(test_mvecs_omp test_genb.cpp test_mvecs.cpp)

add_executable(test_sym test_sym.cpp)

#set_target_properties(test_mvecs_omp PROPERTIES COMPILE_FLAGS "-fopenmp -DOMP_ENABLED")
#set_target_properties(test_mvecs_omp PROPERTIES LINK_FLAGS "-fopenmp")

