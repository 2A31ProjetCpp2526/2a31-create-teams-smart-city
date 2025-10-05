# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Materiel_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Materiel_autogen.dir\\ParseCache.txt"
  "Materiel_autogen"
  )
endif()
