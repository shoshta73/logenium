function(_log LEVEL MESSAGE)
    if(PROJECT_NAME)
        set(_tag "${PROJECT_NAME}")
    else()
        set(_tag "CMake")
    endif()

    message(${LEVEL} "[${_tag}] ${MESSAGE}")
endfunction()

function(log_status MESSAGE)
    _log(STATUS "${MESSAGE}")
endfunction()

function(log_info MESSAGE)
    _log(STATUS "INFO: ${MESSAGE}")
endfunction()

function(log_warning MESSAGE)
    _log(WARNING "WARNING: ${MESSAGE}")
endfunction()

function(log_error MESSAGE)
    _log(FATAL_ERROR "ERROR: ${MESSAGE}")
endfunction()
