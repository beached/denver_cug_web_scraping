cmake_minimum_required(VERSION 3.14)
project(denver_cug_web_scraping)

set(CMAKE_CXX_STANDARD 17)
set(Boost_NO_WARN_NEW_VERSIONS ON)

add_subdirectory(extern)

find_package(Boost REQUIRED COMPONENTS system thread)

include_directories( include/)
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_directories( ${PROJECT_NAME} PRIVATE ${GUMBO_LIBRARY_DIRS} )

target_link_libraries(
        ${PROJECT_NAME}
        PRIVATE
        daw::daw-json-link
        daw::daw-header-libraries
        daw::daw-curl-wrapper
        daw::daw-utf-range
        Boost::boost
        daw::daw-gumbo-pp
)

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}_Targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
