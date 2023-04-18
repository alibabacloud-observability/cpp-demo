cmake_minimum_required(<your-cmake-version>)
project(otel-grpc-export-demo)

set(CMAKE_CXX_STANDARD 14)


add_executable(otel-grpc-export-demo grpc_exporter.cc)

find_package(</path/to/opentelemetry-cpp-lib>/lib/cmake/opentelemetry-cpp/opentelemetry-cpp CONFIG REQUIRED)
find_package(protobuf REQUIRED)
find_package(gRPC REQUIRED)
find_package(CURL REQUIRED)
find_package(</path/to/opentelemetry-cpp-lib>/lib/cmake/nlohmann_json/nlohmann_json CONFIG REQUIRED)

target_include_directories(otel-grpc-export-demo PRIVATE ${OPENTELEMETRY_CPP_INCLUDE_DIRS})
target_link_libraries(otel-grpc-export-demo PRIVATE ${OPENTELEMETRY_CPP_LIBRARIES})

