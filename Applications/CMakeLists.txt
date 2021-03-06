#=========================================================================
#
#  Copyright Leiden University Medical Center, Erasmus University Medical 
#  Center and contributors
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#        http://www.apache.org/licenses/LICENSE-2.0.txt
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#
#=========================================================================

cmake_minimum_required( VERSION 3.0.2 )
project(SuperElastixApplications)

if( NOT CMAKE_PROJECT_NAME STREQUAL "SuperElastix" )
  # Examples are build as an external project, need to import dependencies

  set(CMAKE_CXX_STANDARD 11)
  
  # -----------------------------------------------------------------
  # Compiler-dependent settings

  # GCC
  if( ${CMAKE_CXX_COMPILER_ID} STREQUAL GNU )
    add_definitions(
      -DVCL_CAN_STATIC_CONST_INIT_FLOAT=0
      -DVCL_CAN_STATIC_CONST_INIT_INT=0
    )
  endif()

  #TODO SuperElastix Application should not link against elastix libraries
  # If OpenMP is supported by this machine, elastix will be compiled with
  # OpenMP flags, and we need to add them here as well
  find_package( OpenMP )
  if (OPENMP_FOUND)
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}" )
    set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}" )
  endif()

  find_package( SuperElastix )
  include( ${SUPERELASTIX_USE_FILE} )

  find_package( ITK )
  include( ${ITK_USE_FILE} )

  message( STATUS "ELASTIX_USE_FILE: ${ELASTIX_USE_FILE}")
  include( ${ELASTIX_USE_FILE} )
endif()

add_subdirectory( CommandlineProgram )