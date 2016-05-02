set( CompilerFlags
  CMAKE_CXX_FLAGS
  CMAKE_CXX_FLAGS_DEBUG
  CMAKE_CXX_FLAGS_MINSIZEREL
  CMAKE_CXX_FLAGS_RELEASE
  CMAKE_CXX_FLAGS_RELWITHDEBINFO
  CMAKE_C_FLAGS
  CMAKE_C_FLAGS_DEBUG
  CMAKE_C_FLAGS_MINSIZEREL
  CMAKE_C_FLAGS_RELEASE
  CMAKE_C_FLAGS_RELWITHDEBINFO
)

if(WIN32)
  if(NOT MINGW)
    if(CMAKE_GENERATOR MATCHES "^Visual Studio")
      option(SUPERELASTIX_USE_MP "Add the /MP for object-file level parallel builds" OFF)
      set(use_mp ${SUPERELASTIX_USE_MP})
      foreach(listname ${compile_flag_lists})
        if("${${listname}}" MATCHES "/Gm")
          set(use_mp OFF)
          message(AUTHOR_WARNING "/Gm is not compatible with /MP: Set SUPERELASTIX_USE_MP to OFF to avoid this warning.")
        endif()
      endforeach()
      if(use_mp)
        set(CMAKE_C_FLAGS "/MP ${CMAKE_C_FLAGS}")
        set(CMAKE_CXX_FLAGS "/MP ${CMAKE_CXX_FLAGS}")
      endif()
    endif()
  endif()
endif()