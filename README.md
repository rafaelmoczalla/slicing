# Prototype - Slicing for Punctuation Windows
Authors: Rafael Moczalla

Date: 21.04.2021

Last Update: 16.05.2021

## Prerequisits

### Git and GCC
To clone the project you need the revision tool Git and for building and executing
need GCC. The project was tested with Gnu Compiler Collection versio 7.5.0. One
can install both with the following commands `sudo apt install build-essential git`
on Ubuntu.

### Python 3
The experiments use python 3 to plot results. You can install python and pythons
dev packages via `sudo apt install python3-dev` on Ubuntu. Make sure that you
exported correct library directories, e.g. for python 3.6
`export CPATH=/usr/include/python3.6m:$CPATH`, and correct include directories,
e.g. `export LD_LIBRARY_PATH=/usr/lib:/usr/lib/python3.6/config-3.6m-x86_64-linux-gnu:$LD_LIBRARY_PATH`.
You may want to add this commands to your `~/.bashrc` script to automatically
export on startup/login. If you are using another python version you need to
adapt your python version in the `Experiments/gradle.build` file in the line
`args << '-lpython3.6m'`.

### Visual Studio Code IDE
This project is build with the Visual Studio Code IDE. To install Visual Studio
Code run the following commands.
```
snap install --classic code
```
Set up the IDE by clicking on `File -> Preferences Extensions` and install
`C/C++` as well as `Gradle Extension Pack`

## Import the Project in Visual Studio Code
1. After cloning the project open the root folder of the project with the via
   `File -> Open Folder`. Visual Studio Code will automatically detect the gradle
   build file and prepare the project for you.

2. Build the project by clicking on `Terminal -> Run Task..` on the top of the IDE
   and selecting `gradle: build`.

3. Check if the binaries are running correctly with `Terminal -> Run Task..` and
   select `gradle: runTest`.

4. A script for running the app is provided in the folder `app/install/main/debug`
   The script is executable and sets up the environment and starts the app.

5. The slicing folder contains under `slicing/build/lib/main/debug/shared` a shared
   library file that can be used in other projects.

## Usage
1. Build project with `gradle build`

2. Run tests with `gradle runTest`

3. Run experiments normally with `DYLD_LIBRARY_PATH=./Experiments/build/install/main/debug/lib LD_LIBRARY_PATH=./Experiments/build/install/main/debug/lib Experiments/build/install/main/debug/lib/Experiments`.

4. Run experiments in debug mode with `gdb` debugger `DYLD_LIBRARY_PATH=./Experiments/build/install/main/debug/lib LD_LIBRARY_PATH=./Experiments/build/install/main/debug/lib gdb Experiments/build/install/main/debug/lib/Experiments -ex quit`.