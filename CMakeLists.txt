cmake_minimum_required(VERSION 3.5)
project (cloud)

set(CMAKE_CXX_STANDARD 17)

# Included the conan build information
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)


#---------------------------
# Check modern-cpp-kafka library
#---------------------------
set(LIBMCK_LIBRARY_DIR ${CMAKE_SOURCE_DIR}/lib/modern-cpp-kafka/include)

message(STATUS "modern-cpp-kafka include directory: ${LIBMCK_LIBRARY_DIR}")

set (COMPONENTS_DIR "${CMAKE_SOURCE_DIR}/src/components")

set(CMAKE_BUILD_TYPE Debug)

# Add an executable
add_executable(cloudcontroller
    ${COMPONENTS_DIR}/http_handlers/file_upload_handler.cpp
    ${COMPONENTS_DIR}/http_handlers/auth_handler.cpp
    ${COMPONENTS_DIR}/http_handlers/http_endpoint.cpp
    ${COMPONENTS_DIR}/authbe/auth_backend.cpp
    src/cloudcontroller/cloudcontroller.cpp
    src/cloudcontroller/main.cpp)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

target_link_libraries(cloudcontroller
    PRIVATE
        CONAN_PKG::pistache
        CONAN_PKG::fmt
        CONAN_PKG::capnproto
)

# Add an executable
add_executable(worker
    src/worker/main.cpp
    ${COMPONENTS_DIR}/kafka_helpers/kafka_consumer.cpp
    ${COMPONENTS_DIR}/kafka_helpers/kafka_producer.cpp)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

target_link_libraries(worker
    PRIVATE
        CONAN_PKG::pistache
        CONAN_PKG::fmt
        CONAN_PKG::capnproto
        CONAN_PKG::librdkafka
)

# Include for all targets
include_directories(
    ${CMAKE_SOURCE_DIR}/src/components
)

#Include for specific target
target_include_directories(worker
    PRIVATE
    ${LIBMCK_LIBRARY_DIR}
    )


# Add an executable test
add_executable(test
    src/test/main.cpp
    ${COMPONENTS_DIR}/kafka_helpers/kafka_consumer.cpp
    ${COMPONENTS_DIR}/kafka_helpers/kafka_producer.cpp
    ${COMPONENTS_DIR}/authbe/auth_backend.cpp)

target_include_directories(test
    PRIVATE
    ${LIBMCK_LIBRARY_DIR}
    )

target_link_libraries(test
    PRIVATE
        CONAN_PKG::pistache
        CONAN_PKG::fmt
        CONAN_PKG::capnproto
        CONAN_PKG::librdkafka
)
