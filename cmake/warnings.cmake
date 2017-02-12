if(MSVC)
  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4996 /wd4244 /wd4706")
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  # -Wshadow
  # -Wold-style-cast
  # -Wconversion -Wsign-conversion
  # -Wstack-protector
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
-Wall -Wextra -W -pedantic \
-Wpointer-arith \
-Wold-style-cast \
-Wcast-qual \
-Wfloat-equal \
-Wunused \
-Wuninitialized \
-Wparentheses \
-Wreturn-type \
-Wdate-time \
-Werror")
  if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
-Wunreachable-code")
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
  elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
      #-Wunsafe-loop-optimizations \
      #-Wsuggest-attribute=pure \
      #-Wsuggest-attribute=const \
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
-Wdouble-promotion \
-Wsuggest-attribute=noreturn \
-Wsuggest-attribute=format \
-Wsuggest-final-types \
-Wsuggest-override")
  endif()
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3")
endif()
