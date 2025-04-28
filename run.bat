@echo off

:: Check if the build folder exists, and if so, delete it
if exist build (
    echo Deleting existing build folder...
    rd /s /q build
)

:: Create the build directory
mkdir build
cd build

:: Run CMake to configure the project
cmake ..

:: Build the project
cmake --build .

:: Navigate to the Debug folder where the executable is located
cd Debug

:: Check if the executable exists and run it
if exist my_project.exe (
    echo Running my_project.exe...
    my_project.exe
) else (
    echo Executable not found. Build might have failed.
)

