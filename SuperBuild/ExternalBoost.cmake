set( PROJECT BOOST )

# Note: It IS important to download different files on different OS's:
# on Unix-like systems, we need the file persmissions (only available in the .tar.gz),
# while on Windows, we need CR/LF line feeds (only available in the .zip)

set( BOOST_CONFIGURE_COMMAND )
if( UNIX )
  set( BOOST_URL "http://sourceforge.net/projects/boost/files/boost/1.59.0/boost_1_59_0.tar.gz")
  set( BOOST_MD5 51528a0e3b33d9e10aaa311d9eb451e3 )
  set( BOOST_CONFIGURE_COMMAND ./bootstrap.sh )
  set( BOOST_BUILD_COMMAND ./b2 )
else()
  if( WIN32 )
    set( BOOST_URL "http://sourceforge.net/projects/boost/files/boost/1.59.0/boost_1_59_0.zip")
    set( BOOST_MD5 08d29a2d85db3ebc8c6fdfa3a1f2b83c )
    set( BOOST_CONFIGURE_COMMAND cmd /C bootstrap.bat msvc )
    set( BOOST_BUILD_COMMAND b2.exe )
  endif()
endif()

set( BOOST_PREFIX_DIR "${CMAKE_INSTALL_PREFIX}/${PROJECT}-prefix" )
set( BOOST_BUILD_DIR "${CMAKE_INSTALL_PREFIX}/${PROJECT}-build" )

ExternalProject_Add( BOOST
  BUILD_IN_SOURCE 1
  URL "${BOOST_URL}"
  URL_MD5 ${BOOST_MD5}
  UPDATE_COMMAND ""
  CONFIGURE_COMMAND ${BOOST_CONFIGURE_COMMAND} 
    --prefix=${BOOST_BUILD_DIR}
  BUILD_COMMAND ${BOOST_BUILD_COMMAND} install
    --prefix=${BOOST_BUILD_DIR}
    link=static
    --with-graph
    --variant=release
    --jobs=4
  INSTALL_COMMAND ""
)

set( BOOST_ROOT "${BOOST_PREFIX_DIR}/src/BOOST" )
