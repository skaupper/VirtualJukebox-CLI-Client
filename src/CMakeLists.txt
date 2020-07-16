add_executable(
    virtualjukebox-cli
    main.cpp
    Exception.cpp
    shell/Shell.cpp
    shell/ShellCommand.cpp
    shell/commands/CmdHelp.cpp
    shell/commands/CmdExit.cpp
    api/api-v1.cpp
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
            CONAN_PKG::docopt.cpp
            CONAN_PKG::spdlog
            CONAN_PKG::tl-optional
            CONAN_PKG::nlohmann_json)
