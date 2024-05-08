# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\NetWork_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\NetWork_autogen.dir\\ParseCache.txt"
  "NetWork_autogen"
  )
endif()
