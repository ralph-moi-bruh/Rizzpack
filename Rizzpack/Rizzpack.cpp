#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include "CLI11.hpp"

namespace fs = std::filesystem;

void create_cmakelists(const std::string& project_name, const fs::path& path) {
    std::ofstream cmake(path / "CMakeLists.txt");
    cmake << "cmake_minimum_required(VERSION 3.10)\n";
    cmake << "project(" << project_name << ")\n\n";
    cmake << "set(CMAKE_CXX_STANDARD 17)\n\n";
    cmake << "add_executable(" << project_name << " src/main.cpp)\n";
}

void create_main_cpp(const std::string& project_name, const fs::path& path) {
    fs::create_directories(path / "src");
    std::ofstream main_cpp(path / "src/main.cpp");
    main_cpp << "#include <iostream>\n\n";
    main_cpp << "int main() {\n";
    main_cpp << "    std::cout << \"Hello, " << project_name << "!\" << std::endl;\n";
    main_cpp << "    return 0;\n";
    main_cpp << "}\n";
}

void build_project(const std::string& project_name, const fs::path& path) {
    fs::path build_path = path / "build";

    if (!fs::exists(build_path)) {
        fs::create_directory(build_path);
    }

    std::string command = "cd " + build_path.string() + " && cmake .. && make";
    int result = std::system(command.c_str());

    if (result != 0) {
        std::cerr << "Build failed!" << std::endl;
    }
    else {
        std::cout << "Build successful! Run ./" << project_name << " from the build directory." << std::endl;
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: rizzpack <command> [options]\n";
        return 1;
    }

    std::string command = argv[1];

    std::string project_name;
    if (argc > 2) {
        project_name = argv[2];
    }

    fs::path project_path = fs::current_path() / project_name;

    if (command == "init") {
        if (project_name.empty()) {
            std::cerr << "Error: Please provide a project name for initialization.\n";
            return 1;
        }

        if (fs::exists(project_path)) {
            std::cerr << "Error: Directory '" << project_name << "' already exists.\n";
            return 1;
        }

        fs::create_directory(project_path);
        create_cmakelists(project_name, project_path);
        create_main_cpp(project_name, project_path);

        std::cout << "Initialized C++ project '" << project_name << "' at " << project_path << std::endl;
    }
    else if (command == "build") {
        if (project_name.empty()) {
            std::cerr << "Error: Please provide a project name to build.\n";
            return 1;
        }

        if (!fs::exists(project_path)) {
            std::cerr << "Error: Project directory '" << project_name << "' not found.\n";
            return 1;
        }

        build_project(project_name, project_path);
    }
    else {
        std::cerr << "Error: Unknown command '" << command << "'.\n";
        std::cerr << "Available commands: init, build\n";
        return 1;
    }

    return 0;
}
