# Beat-Box Application - (ALSA Library)

## Project Description
The BeagleBone Drum Controller project aims to provide an interactive and customizable drumming experience using the BeagleBone platform. Leveraging its capabilities, this project offers users the ability to play background drum beats as well as generate standalone drum sounds by simply manipulating the BeagleBone in three-dimensional space.

## Key Features
- Background Drum Beats: The current version supports two different types of rock beats.
- Standalone Drum Sounds: The application enables users to shake the BeagleBone in three dimensions to generate three distinct types of sounds as follows:
  - Hi-Hat: Move the BeagleBone forward and backward along the Y-axis.
  - Snare: Shift the BeagleBone horizontally along the X-axis.
  - Bass Drum: Raise or lower the BeagleBone vertically along the Z-axis.
- Adjust Tempo and Volume: Users can fine-tune the volume (0 - 100) and tempo (40 - 300 bpm) using either the Joystick (Zen Cape Red) or the web interface.

## Devices & OS
- Devices:
  - Beagle Bone: Opern-source hardware platform (produced by Texas Instrument)
  - Accelerometer: LIS331DLH datasheet attached on Zen Cape Red (detect acceleration)
  - Joystick: attached on Zen Cape Red
- Operating System: Linux - Debian 11
   
## Folder Sturcture

- `hal/`: Contains all low-level hardware abstraction layer (HAL) modules
- `app/`: Contains all application-specific code. Broken into modules and a main file
- `beat-box-server: Contains all (NodeJS) code for web application (front end and backend)

```
  .
  ├── app
  │   ├── include
  │   │   └── badmath.h
  │   ├── src
  │   │   ├── badmath.c
  │   │   └── main.c
  │   └── CMakeLists.txt           # Sub CMake file, just for app/
  ├── hal
  │   ├── include
  │   │   └── hal
  │   │       └── button.h
  │   ├── src
  │   │   └── button.c
  │   └── CMakeLists.txt           # Sub CMake file, just for hal/
  ├── beat-box-server
  ├── CMakeLists.txt               # Main CMake file for the project
  └── README.md
```  

## Usage

- Install CMake: `sudo apt update` and `sudo apt install cmake`
- When you first open the project, click the "Build" button in the status bar for CMake to generate the `build\` folder and recreate the makefiles.
  - When you edit and save a CMakeLists.txt file, VS Code will automatically update this folder.
- When you add a new file (.h or .c) to the project, you'll need to rerun CMake's build
  (Either click "Build" or resave `/CMakeLists.txt` to trigger VS Code re-running CMake)
- Cross-compile using VS Code's CMake addon:
  - The "kit" defines which compilers and tools will be run.
  - Change the kit via the menu: Help > Show All Commands, type "CMake: Select a kit".
    - Kit "GCC 10.2.1 arm-linux-gnueabi" builds for target.
    - Kit "Unspecified" builds for host (using default `gcc`).
  - Most CMake options for the project can be found in VS Code's CMake view (very left-hand side).
- Build the project using Ctrl+Shift+B, or by the menu: Terminal > Run Build Task...
  - If you try to build but get an error about "build is not a directory", the re-run CMake's build as mentioned above.

## Address Sanitizer

- The address sanitizer built into gcc/clang is very good at catching memory access errors.
- Enable it by uncomment the `fsanitize=address` lines in the root CMakeFile.txt.
- For this to run on the BeagleBone, you must run:
  `sudo apt install libasan6`
  - Without this installed, you'll get an error:   
    "error while loading shared libraries: libasan.so.6: cannot open shared object file: No such file or directory"

## Suggested addons

- "CMake Tools" automatically suggested when you open a `CMakeLists.txt` file
- "Output Colourizer" by IBM 
    --> Adds colour to the OUTPUT panel in VS Code; useful for seeing CMake messages

## Manually Running CMake

To manually run CMake from the command line use:

```shell
  # Regenerate build/ folder and makefiles:
  rm -rf build/         # Wipes temporary build folder
  cmake -S . -B build   # Generate makefiles in build\

  # Build (compile & link) the project
  cmake --build build
```
