add_executable(
    virtualjukebox-cli
    main.cpp
    Exception.cpp
    shell/Shell.cpp
    shell/ShellCommand.cpp
    shell/commands/Help.cpp
    shell/commands/Exit.cpp
    shell/commands/v1/Login.cpp
    shell/commands/v1/PrintQueues.cpp
    shell/commands/v1/AddTrack.cpp
    shell/commands/v1/ControlPlayer.cpp
    shell/commands/v1/Vote.cpp
    api/v1/Api.cpp
    api/v1/deserializer.cpp
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
