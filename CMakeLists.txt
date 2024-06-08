cmake_minimum_required(VERSION 3.12)

# Set the name of the project and target:
SET(TARGET "silhouette")

project(${TARGET} VERSION 1.0)

include(CMakePrintHelpers)
include(FetchContent)
include(CheckCXXCompilerFlag)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SRC CONFIGURE_DEPENDS RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")

add_executable(${TARGET} ${SRC})

target_include_directories(${TARGET} PRIVATE ${CMAKE_SOURCE_DIR}/include)


find_package(TBB REQUIRED)

#find_package(Boost 1.45.0 COMPONENTS filesystem regex) 
find_package(Boost REQUIRED) 
find_package(Eigen3 REQUIRED)
#find_package(BLAS REQUIRED)
#find_package(LAPACK REQUIRED)
#find_package(MKL CONFIG REQUIRED PATHS "/opt/intel/oneapi/mkl/2024.1")
find_package(Threads REQUIRED)
#find_package(OpenMP REQUIRED)

FetchContent_Declare(vtu11
  GIT_REPOSITORY https://github.com/phmkopp/vtu11.git
  GIT_TAG v1.2
)

FetchContent_GetProperties(vtu11)
if(NOT vtu11_POPULATED)
  MESSAGE(STATUS "Fetching vtu11")
  FetchContent_Populate(vtu11 )
  add_subdirectory(${vtu11_SOURCE_DIR} ${vtu11_BINARY_DIR} EXCLUDE_FROM_ALL)
  MESSAGE(STATUS "Fetching ftxui ${vtu11_SOURCE_DIR} ${vtu11_BINARY_DIR}")
endif()

target_link_libraries( ${TARGET} PRIVATE vtu11::vtu11 )

check_cxx_compiler_flag("-fopenmp-simd" HAS_OPENMP_SIMD)
if(HAS_OPENMP_SIMD)
    target_compile_options(${TARGET} PRIVATE -fopenmp-simd)
endif()

check_cxx_compiler_flag("-fopenmp" HAS_OPENMP)
if(HAS_OPENMP)
    target_compile_options(${TARGET} PRIVATE -fopenmp)
endif()

check_cxx_compiler_flag("-msse2" HAS_sse2)
if(HAS_sse2)
    target_compile_options(${TARGET} PRIVATE -msse2)
endif()

check_cxx_compiler_flag("-mavx" HAS_avx)
if(HAS_avx)
    target_compile_options(${TARGET} PRIVATE -mavx)
endif()

IF(CMAKE_BUILD_TYPE MATCHES "Debug")
    message("Debug mode. No vectorization or optimiztion flags set.")
ELSE()
    message("Release mode. Vectorization and optimiztion flags set.")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        message(STATUS "Using GCC compiler")
        target_compile_options(${TARGET} PRIVATE -O3 -ftree-vectorize)
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        message(STATUS "Using Clang compiler")
        target_compile_options(${TARGET} PRIVATE -O2)
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        message(STATUS "Using MSVC compiler")
        target_compile_options(${TARGET} PRIVATE /O2 /Qvec)
    else()
        message(WARNING "Compiler not recognized, auto-vectorization flags not set")
    endif()
ENDIF(CMAKE_BUILD_TYPE MATCHES "Debug")


target_link_libraries(${TARGET} PRIVATE ${CMAKE_THREAD_LIBS_INIT})

#target_link_libraries(${TARGET} PRIVATE OpenMP::OpenMP_CXX)
#target_link_libraries(${TARGET} PRIVATE TBB::tbb)
target_link_libraries(${TARGET} PRIVATE Eigen3::Eigen)
#target_link_libraries(${TARGET} PRIVATE ${BLAS_LIBRARIES} ${LAPACK_LIBRARIES})
#target_link_libraries(${TARGET} PRIVATE MKL::MKL)

# target_include_directories(${TARGET} PRIVATE ${TBB_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} ${EIGEN3_INCLUDE_DIR})
