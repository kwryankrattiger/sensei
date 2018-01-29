#*****************************************************************************
# FindNumpy
#
# Check if numpy is installed and configure c-api includes
#
# This module defines
#  NUMPY_FOUND, set TRUE if numpy and c-api are available
#  NUMPY_INCLUDE_DIR, where to find c-api headers
#  NUMPY_VERSION, numpy release version
set(_TMP_PY_OUTPUT)
set(_TMP_PY_RETURN)
exec_program("${PYTHON_EXECUTABLE}"
  ARGS "-c 'import numpy; print(numpy.get_include())'"
  OUTPUT_VARIABLE _TMP_PY_OUTPUT
  RETURN_VALUE _TMP_PY_RETURN)
set(NUMPY_INCLUDE_FOUND FALSE)
if(NOT _TMP_PY_RETURN AND EXISTS "${_TMP_PY_OUTPUT}")
  set(NUMPY_INCLUDE_FOUND TRUE)
else()
  set(_TMP_PY_OUTPUT)
endif()
set(NUMPY_INCLUDE_DIR "${_TMP_PY_OUTPUT}" CACHE PATH
  "numpy include directory")

set(_TMP_PY_OUTPUT)
set(_TMP_PY_RETURN)
exec_program("${PYTHON_EXECUTABLE}"
  ARGS "-c 'import numpy; print(numpy.version.version)'"
  OUTPUT_VARIABLE _TMP_PY_OUTPUT
  RETURN_VALUE _TMP_PY_RETURN)
set(NUMPY_VERSION_FOUND FALSE)
if(NOT _TMP_PY_RETURN)
  set(NUMPY_VERSION_FOUND TRUE)
else()
  set(_TMP_PY_OUTPUT)
endif()
set(NUMPY_VERSION "${_TMP_PY_OUTPUT}" CACHE STRING
  "numpy version string")

if (NOT ${QUIET})
  message(STATUS "NUMPY_INCLUDE_DIR=${NUMPY_INCLUDE_DIR}")
  message(STATUS "NUMPY_VERSION=${NUMPY_VERSION}")
endif()

mark_as_advanced(NUMPY_INCLUDE_DIR NUMPY_VERSION)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(NUMPY DEFAULT_MSG
  NUMPY_INCLUDE_FOUND NUMPY_VERSION_FOUND)
