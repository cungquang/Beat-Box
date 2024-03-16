# CMake Build Configuration for root of project
cmake_minimum_required(VERSION 3.18)
project(my_hello_world VERSION 1.0 DESCRIPTION "Starter project" LANGUAGES C)

# Compiler options (inherited by sub-folders)
set(CMAKE_C_STANDARD 11)
add_compile_options(-Wall -g -Werror -Wpedantic -Wextra)
add_compile_options(-fdiagnostics-color)

# Enable address sanitizer
# (Comment this out to make your code faster)
#add_compile_options(-fsanitize=address)
#add_link_options(-fsanitize=address)

# What folders to build
add_subdirectory(hal)  
add_subdirectory(app)

# Copy audio node folders
# ---------------- NEW! ----------------
# Copy the node_server folder to NFS
add_custom_command(TARGET wave_player_cmake POST_BUILD 
  COMMAND "${CMAKE_COMMAND}" -E copy_directory
     "${CMAKE_SOURCE_DIR}/beatbox-server"
     "~/cmpt433/public/myApps/beatbox-server-copy" 
  COMMENT "Copying WAVE files to public NFS directory")
