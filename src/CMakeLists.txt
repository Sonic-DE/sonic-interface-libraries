add_subdirectory(desktoptheme)
add_subdirectory(plasma)
add_subdirectory(declarativeimports)
add_subdirectory(plasmaquick)
add_subdirectory(packages)

ecm_qt_install_logging_categories(
    EXPORT PLASMA
    FILE plasma-framework.categories
    DESTINATION ${KDE_INSTALL_LOGGINGCATEGORIESDIR}
)
