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

int main(int argc, char** argv) {
    CLI::App app{ "C++ Project Initializer (cppinit) - Quickly bootstrap CMake C++ projects" };

    std::string project_name;
    app.add_option("name", project_name, "Project name")->required();

    CLI11_PARSE(app, argc, argv);

    fs::path project_path = fs::current_path() / project_name;

    if (fs::exists(project_path)) {
        std::cerr << "Error: Directory '" << project_name << "' already exists.\n";
        return 1;
    }

    fs::create_directory(project_path);

    create_cmakelists(project_name, project_path);
    create_main_cpp(project_name, project_path);

    std::cout << "Initialized C++ project '" << project_name << "' at " << project_path << std::endl;
    std::cout << "You can now run:\n";
    std::cout << "  cd " << project_name << "\n";
    std::cout << "  mkdir build && cd build\n";
    std::cout << "  cmake .. && make\n";
    std::cout << "  ./" << project_name << "\n";

    return 0;
}
