cmake_minimum_required(VERSION 3.12)

project(TestProject VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SRC CONFIGURE_DEPENDS RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")

add_executable(silhouette ${SRC})

target_include_directories(silhouette PRIVATE ${CMAKE_SOURCE_DIR}/include)


find_package(TBB REQUIRED)

#find_package(Boost 1.45.0 COMPONENTS filesystem regex) 
find_package(Boost REQUIRED) 
find_package(Eigen3 REQUIRED)
#find_package(BLAS REQUIRED)
#find_package(LAPACK REQUIRED)
#find_package(MKL CONFIG REQUIRED PATHS "/opt/intel/oneapi/mkl/2024.1")
find_package(Threads REQUIRED)
#find_package(OpenMP REQUIRED)

include(CheckCXXCompilerFlag)

check_cxx_compiler_flag("-fopenmp-simd" HAS_OPENMP_SIMD)
if(HAS_OPENMP_SIMD)
    target_compile_options(silhouette PRIVATE -fopenmp-simd)
endif()

check_cxx_compiler_flag("-fopenmp" HAS_OPENMP)
if(HAS_OPENMP)
    target_compile_options(silhouette PRIVATE -fopenmp)
endif()

check_cxx_compiler_flag("-msse2" HAS_sse2)
if(HAS_sse2)
    target_compile_options(silhouette PRIVATE -msse2)
endif()

check_cxx_compiler_flag("-mavx" HAS_avx)
if(HAS_avx)
    target_compile_options(silhouette PRIVATE -mavx)
endif()

IF(CMAKE_BUILD_TYPE MATCHES "Debug")
    message("Debug mode. No vectorization or optimiztion flags set.")
ELSE()
    message("Release mode. Vectorization and optimiztion flags set.")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        message(STATUS "Using GCC compiler")
        target_compile_options(silhouette PRIVATE -O3 -ftree-vectorize)
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        message(STATUS "Using Clang compiler")
        target_compile_options(silhouette PRIVATE -O2)
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        message(STATUS "Using MSVC compiler")
        target_compile_options(silhouette PRIVATE /O2 /Qvec)
    else()
        message(WARNING "Compiler not recognized, auto-vectorization flags not set")
    endif()
ENDIF(CMAKE_BUILD_TYPE MATCHES "Debug")


target_link_libraries(silhouette PRIVATE ${CMAKE_THREAD_LIBS_INIT})

#target_link_libraries(silhouette PRIVATE OpenMP::OpenMP_CXX)
#target_link_libraries(silhouette PRIVATE TBB::tbb)
target_link_libraries(silhouette PRIVATE Eigen3::Eigen)
#target_link_libraries(silhouette PRIVATE ${BLAS_LIBRARIES} ${LAPACK_LIBRARIES})
#target_link_libraries(silhouette PRIVATE MKL::MKL)

# target_include_directories(silhouette PRIVATE ${TBB_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} ${EIGEN3_INCLUDE_DIR})
