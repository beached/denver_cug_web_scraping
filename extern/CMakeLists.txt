# Copyright (c) Darrell Wright
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# Official repository: https://github.com/beached/climate-change-api-example
#

include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
set(DAW_ENABLE_TESTING OFF)
FetchContent_Declare(
        daw_header_libraries
        GIT_REPOSITORY https://github.com/beached/header_libraries.git
        GIT_TAG master
)

FetchContent_Declare(
        daw_json_link
        GIT_REPOSITORY https://github.com/beached/daw_json_link.git
        GIT_TAG develop
)

FetchContent_Declare(
        curl_wrapper
        GIT_REPOSITORY https://github.com/beached/curl_wrapper.git
        GIT_TAG master
)

FetchContent_Declare(
        daw_gumbo_pp
        GIT_REPOSITORY https://github.com/beached/gumbo_pp.git
        GIT_TAG main
)

FetchContent_MakeAvailable(daw_header_libraries daw_json_link curl_wrapper daw_gumbo_pp)
