ecm_add_qml_module(plasmoidplugin URI "org.kde.plasma.plasmoid" VERSION 2.0 CLASSNAME plasmoidplugin)

target_sources(plasmoidplugin PRIVATE
    plasmoidplugin.cpp
)

target_link_libraries(plasmoidplugin
    plasma_appletscript_object
)

install(TARGETS plasmoidplugin DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/plasma/plasmoid)

ecm_finalize_qml_module(plasmoidplugin DESTINATION ${KDE_INSTALL_QMLDIR})
ecm_generate_qmltypes(org.kde.plasma.plasmoid 2.0 DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/plasma/plasmoid)
