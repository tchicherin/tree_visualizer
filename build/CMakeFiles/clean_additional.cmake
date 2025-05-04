# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/TreeVisualizer_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/TreeVisualizer_autogen.dir/ParseCache.txt"
  "TreeVisualizer_autogen"
  )
endif()
