install(
    FILES colors metadata.json plasmarc
    DESTINATION ${PLASMA_DATA_INSTALL_DIR}/desktoptheme/silver-dark
)

file(GLOB_RECURSE silver_dark_svgs RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.svg" "*.svgz")
foreach(svg ${silver_dark_svgs})
    get_filename_component(subpath "${svg}" DIRECTORY)
    install(FILES "${svg}" DESTINATION "${PLASMA_DATA_INSTALL_DIR}/desktoptheme/silver-dark/${subpath}")
endforeach()
