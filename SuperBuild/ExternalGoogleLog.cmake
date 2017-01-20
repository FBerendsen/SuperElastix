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

set( proj GoogleLog )

set( GLOG_prefix ${CMAKE_CURRENT_BINARY_DIR}/${proj}-prefix )
set( GLOG_binary_dir ${CMAKE_CURRENT_BINARY_DIR}/${proj}-build )
set( GLOG_install_dir ${CMAKE_CURRENT_BINARY_DIR}/${proj} )

ExternalProject_Add( ${proj}
  PREFIX ${GLOG_prefix}
  GIT_REPOSITORY "https://github.com/google/glog"
  GIT_TAG b6a5e0524c28178985f0d228e9eaa43808dbec3c
  BINARY_DIR ${GLOG_binary_dir}
  INSTALL_DIR ${GLOG_install_dir}
  CMAKE_ARGS
    --no-warn-unused-cli
    -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
  INSTALL_COMMAND
      "${CMAKE_COMMAND}"
      --build .
      --target install
)

set( GLOG_ROOT ${GLOG_install_dir} )
list( APPEND ELASTIX_DEPENDENCIES ${proj} )

