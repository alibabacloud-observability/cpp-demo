## 通过OpenTelemetry上报C++应用数据

### 1. 下载 Opentelemetry C++

```shell
git clone --recurse-submodules https://github.com/open-telemetry/opentelemetry-cpp
```

- 如果子module下载失败，执行以下命令
```shell
cd opentelemetry-cpp
git submodule update --init
```

### 2. 编译构建 Opentelemetry C++

```shell
cd opentelemetry-cpp

mkdir build && cd build && cmake -DCMAKE_CXX_STANDARD=14 -DBUILD_TESTING=OFF -DWITH_OTLP=ON -DWITH_OTLP_HTTP=ON ..

cmake --build . --target all

# 请替换</path/to/opentelemetry-cpp-lib>，构建好的cmake targets会保存在该路径
cmake --install . --prefix </path/to/opentelemetry-cpp-lib>
```


### 3. 方法一：通过gRPC协议上报数据（otel-grpc-export-demo）

1. 修改demo项目CMakeLists中的内容

- 请替换 `<your-cmake-version>` 为您的cmake版本
- 请替换 `</path/to/opentelemetry-cpp-lib>` 为之前的设置的路径

```
cmake_minimum_required(VERSION <your-cmake-version>)
project(otel-grpc-export-demo)

set(CMAKE_CXX_STANDARD 14)

add_executable(otel-grpc-export-demo grpc_exporter.cc)

# 关键内容
find_package(</path/to/opentelemetry-cpp-lib>/lib/cmake/opentelemetry-cpp/opentelemetry-cpp CONFIG REQUIRED)
find_package(</path/to/opentelemetry-cpp-lib>/lib/cmake/nlohmann_json/nlohmann_json CONFIG REQUIRED)
find_package(protobuf REQUIRED)
find_package(gRPC REQUIRED)
find_package(CURL REQUIRED)

target_include_directories(otel-grpc-export-demo PRIVATE ${OPENTELEMETRY_CPP_INCLUDE_DIRS})
target_link_libraries(otel-grpc-export-demo PRIVATE ${OPENTELEMETRY_CPP_LIBRARIES})
# 关键内容
```

2. 编译demo项目

```
mkdir build && cd build && cmake .. && make 
```

3. 运行

```
./otel-grpc-export-demo
```


### 4. 方法二：通过HTTP协议上报数据（otel-http-export-demo）

1. 修改demo项目CMakeLists中的内容

- 请替换 `<your-cmake-version>` 为您的cmake版本
- 请替换 `</path/to/opentelemetry-cpp-lib>` 为之前的设置的路径

```
cmake_minimum_required(VERSION <your-cmake-version>)
project(otel-http-export-demo)

set(CMAKE_CXX_STANDARD 14)

add_executable(otel-http-export-demo http_exporter.cc)

# 关键内容
find_package(</path/to/opentelemetry-cpp-lib>/lib/cmake/opentelemetry-cpp/opentelemetry-cpp CONFIG REQUIRED)
find_package(</path/to/opentelemetry-cpp-lib>/lib/cmake/nlohmann_json/nlohmann_json CONFIG REQUIRED)
find_package(protobuf REQUIRED)
find_package(gRPC REQUIRED)
find_package(CURL REQUIRED)

target_include_directories(otel-http-export-demo PRIVATE ${OPENTELEMETRY_CPP_INCLUDE_DIRS})
target_link_libraries(otel-http-export-demo PRIVATE ${OPENTELEMETRY_CPP_LIBRARIES})
# 关键内容
```

2. 编译demo项目

```
mkdir build && cd build && cmake .. && make 
```

3. 运行

```
./otel-http-export-demo
```









