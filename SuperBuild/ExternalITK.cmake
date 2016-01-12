set( proj ITK )
set( ITK_REPOSITORY https://github.com/InsightSoftwareConsortium/ITK.git )
set( ITK_TAG "v${ITK_VERSION_STRING}")

#TODO either of these variables should contain the MSVC specific compilerflags
MESSAGE( STATUS "proj ITK MSVC Compiler flags: " ${MSVC_COMP_FLAGS} )
MESSAGE( STATUS "proj ITK CompilerConfigurations: " ${CompilerConfigurations} )

ExternalProject_Add( ${proj}
  GIT_REPOSITORY ${ITK_REPOSITORY}
  GIT_TAG ${ITK_TAG}
  UPDATE_COMMAND ""
  SOURCE_DIR ${proj}
  BINARY_DIR ${proj}-build
  CMAKE_ARGS
    --no-warn-unused-cli
	# TODO make flags MSVC specific
    -DCMAKE_CXX_FLAGS:STRING=/MP
    -DCMAKE_C_FLAGS:STRING=/MP
    -DBUILD_EXAMPLES:BOOL=OFF
    -DBUILD_TESTING:BOOL=OFF
    -DITK_BUILD_DEFAULT_MODULES:BOOL=ON
    -DModule_ITKReview:BOOL=ON
    -DCMAKE_SKIP_RPATH:BOOL=ON
    -DITK_LEGACY_REMOVE:BOOL=ON
    -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
)

ExternalProject_Get_Property( ${proj} install_dir )
set( ITK_DIR "${install_dir}/lib/cmake/ITK-${ITK_VERSION_MAJOR}.${ITK_VERSION_MINOR}" )

list( APPEND ELASTIX_DEPENDENCIES ${proj} )
