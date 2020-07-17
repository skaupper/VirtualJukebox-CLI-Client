add_executable(
    virtualjukebox-cli
    main.cpp
    Exception.cpp
    shell/Shell.cpp
    shell/ShellCommand.cpp
    shell/commands/Help.cpp
    shell/commands/Exit.cpp
    shell/commands/Login.cpp
    shell/commands/PrintQueues.cpp
    api/api-v1.cpp
    api/api-v1-provider.cpp
    api/deserializer.cpp
    exceptions/ShellException.cpp
    exceptions/APIException.cpp
    exceptions/InvalidFormatException.cpp
    exceptions/NetworkException.cpp)

target_include_directories(virtualjukebox-cli PRIVATE .)
target_link_libraries(
    virtualjukebox-cli
    PRIVATE project_options
            project_warnings
            project_libs
            CONAN_PKG::spdlog
            CONAN_PKG::nlohmann_json)

target_precompile_headers(
    virtualjukebox-cli
    PRIVATE
    <nlohmann/json.hpp>
    "utils/utils.h")
