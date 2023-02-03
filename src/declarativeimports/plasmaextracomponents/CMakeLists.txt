add_library(plasmaextracomponentsplugin SHARED)

target_sources(plasmaextracomponentsplugin PRIVATE
    plasmaextracomponentsplugin.cpp
    enums.cpp
    qmenu.cpp
    qmenuitem.cpp
)

target_link_libraries(plasmaextracomponentsplugin
        Qt${QT_MAJOR_VERSION}::Quick
        Qt${QT_MAJOR_VERSION}::Qml
        Qt${QT_MAJOR_VERSION}::Widgets
        KF6::ConfigWidgets
        KF6::Plasma)


install(TARGETS plasmaextracomponentsplugin DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/plasma/extras)

install(DIRECTORY qml/ DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/plasma/extras)

ecm_generate_qmltypes(org.kde.plasma.extras 2.0 DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/plasma/extras)
