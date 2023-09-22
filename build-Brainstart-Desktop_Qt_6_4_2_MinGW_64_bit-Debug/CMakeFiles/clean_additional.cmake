# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "Brainstart_autogen"
  "CMakeFiles\\Brainstart_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Brainstart_autogen.dir\\ParseCache.txt"
  )
endif()
