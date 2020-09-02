project(rpu-emulator)
cmake_minimum_required(VERSION 3.16)

set(MAJOR 0)
set(MINOR 0)
set(PATCH 0)
set(RELEASE 0)
set(CMAKE_CXX_STANDARD 14)

add_subdirectory(src)
############################################################
# Create DEB
############################################################
set(${PROJECT_NAME}_VERSION "${MAJOR}.${MINOR}.${PATCH}.${RELEASE}")
set(${DEB_PACKAGE_NAME} ${PROJECT_NAME})
# Tell CPack to generate a .deb package
set(CPACK_GENERATOR "DEB")

# Set a Package Maintainer.
# This is required
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Sandesh J. Ghimire ")

# Set a Package Version
set(CPACK_PACKAGE_VERSION ${GrandCentral_VERSION})

# Include CPack
include(CPack)