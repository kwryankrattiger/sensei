include(ExternalData)

if(NOT SVTK_DATA_STORE)
  # Select a default from the following.
  set(SVTK_DATA_STORE_DEFAULT "")
  if(EXISTS "${SVTK_SOURCE_DIR}/.ExternalData/config/store")
    # Configuration left by developer setup script.
    file(STRINGS "${SVTK_SOURCE_DIR}/.ExternalData/config/store"
      SVTK_DATA_STORE_DEFAULT LIMIT_COUNT 1 LIMIT_INPUT 1024)
  elseif(IS_DIRECTORY "${CMAKE_SOURCE_DIR}/../SVTKExternalData")
    # Adjacent directory created by user.
    get_filename_component(SVTK_DATA_STORE_DEFAULT
      "${CMAKE_SOURCE_DIR}/../SVTKExternalData" ABSOLUTE)
  elseif(IS_DIRECTORY "${CMAKE_SOURCE_DIR}/../ExternalData")
    # Generic adjacent directory created by user.
    get_filename_component(SVTK_DATA_STORE_DEFAULT
      "${CMAKE_SOURCE_DIR}/../ExternalData" ABSOLUTE)
  elseif(DEFINED "ENV{SVTKExternalData_OBJECT_STORES}")
    # The SVTKExternalData environment variable.
    file(TO_CMAKE_PATH "$ENV{SVTKExternalData_OBJECT_STORES}" SVTK_DATA_STORE_DEFAULT)
  elseif(DEFINED "ENV{ExternalData_OBJECT_STORES}")
    # Generic ExternalData environment variable.
    file(TO_CMAKE_PATH "$ENV{ExternalData_OBJECT_STORES}" SVTK_DATA_STORE_DEFAULT)
  endif()
endif()

# Provide users with an option to select a local object store,
# starting with the above-selected default.
set(SVTK_DATA_STORE "${SVTK_DATA_STORE_DEFAULT}" CACHE PATH
  "Local directory holding ExternalData objects in the layout %(algo)/%(hash).")
mark_as_advanced(SVTK_DATA_STORE)

# Use a store in the build tree if none is otherwise configured.
if(NOT SVTK_DATA_STORE)
  if(ExternalData_OBJECT_STORES)
    set(SVTK_DATA_STORE "")
  else()
    set(SVTK_DATA_STORE "${CMAKE_BINARY_DIR}/ExternalData/Objects")
    file(MAKE_DIRECTORY "${SVTK_DATA_STORE}")
  endif()
endif()

# Tell ExternalData module about selected object stores.
list(APPEND ExternalData_OBJECT_STORES
  # Store selected by SVTK-specific configuration above.
  ${SVTK_DATA_STORE}

  # Local data store populated by the SVTK pre-commit hook
  "${CMAKE_SOURCE_DIR}/.ExternalData"
  )

set(ExternalData_BINARY_ROOT ${CMAKE_BINARY_DIR}/ExternalData)

set(ExternalData_URL_TEMPLATES "" CACHE STRING
  "Additional URL templates for the ExternalData CMake script to look for testing data. E.g. file:///var/bigharddrive/%(algo)/%(hash)")
mark_as_advanced(ExternalData_URL_TEMPLATES)
if(NOT SVTK_FORBID_DOWNLOADS)
  list(APPEND ExternalData_URL_TEMPLATES
    # Data published by Girder
    "https://data.kitware.com/api/v1/file/hashsum/%(algo)/%(hash)/download"

    # Data published by developers using git-gitlab-push.
    "https://www.svtk.org/files/ExternalData/%(algo)/%(hash)"
  )
endif()

# Tell ExternalData commands to transform raw files to content links.
# TODO: Condition this feature on presence of our pre-commit hook.
set(ExternalData_LINK_CONTENT SHA512)

# Match series of the form <base>.<ext>, <base>_<n>.<ext> such that <base> may
# end in a (test) number that is not part of any series numbering.
set(ExternalData_SERIES_PARSE "()(\\.[^./]*)$")
set(ExternalData_SERIES_MATCH "(_[0-9]+)?")

if(DEFINED ENV{DASHBOARD_TEST_FROM_CTEST})
  # Dashboard builds always need data.
  set(SVTK_DATA_EXCLUDE_FROM_ALL OFF)
endif()

if(NOT DEFINED SVTK_DATA_EXCLUDE_FROM_ALL)
  if(EXISTS "${SVTK_SOURCE_DIR}/.ExternalData/config/exclude-from-all")
    # Configuration left by developer setup script.
    file(STRINGS "${SVTK_SOURCE_DIR}/.ExternalData/config/exclude-from-all"
      SVTK_DATA_EXCLUDE_FROM_ALL_DEFAULT LIMIT_COUNT 1 LIMIT_INPUT 1024)
  elseif(DEFINED "ENV{SVTK_DATA_EXCLUDE_FROM_ALL}")
    set(SVTK_DATA_EXCLUDE_FROM_ALL_DEFAULT
      "$ENV{SVTK_DATA_EXCLUDE_FROM_ALL}")
  else()
    set(SVTK_DATA_EXCLUDE_FROM_ALL_DEFAULT OFF)
  endif()
  set(SVTK_DATA_EXCLUDE_FROM_ALL "${SVTK_DATA_EXCLUDE_FROM_ALL_DEFAULT}"
    CACHE BOOL "Exclude test data download from default 'all' target."
    )
  mark_as_advanced(SVTK_DATA_EXCLUDE_FROM_ALL)
endif()
