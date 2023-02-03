ecm_add_qml_module(plasmaextracomponentsplugin VERSION 2.0 URI "org.kde.plasma.extras")

target_sources(plasmaextracomponentsplugin PRIVATE
    plasmaextracomponentsplugin.cpp
    enums.cpp
    qmenu.cpp
    qmenuitem.cpp
)

ecm_target_qml_sources(plasmaextracomponentsplugin VERSION 2.0 SOURCES
    qml/ActionTextField.qml
    qml/BasicPlasmoidHeading.qml
    qml/DescriptiveLabel.qml
    qml/ExpandableListItem.qml
    qml/Heading.qml
    qml/Highlight.qml
    qml/ListItem.qml
    qml/PasswordField.qml
    qml/PlaceholderMessage.qml
    qml/PlasmoidHeading.qml
    qml/Representation.qml
    qml/SearchField.qml
    qml/animations/ActivateAnimation.qml
    qml/animations/AppearAnimation.qml
    qml/animations/DisappearAnimation.qml
    qml/animations/PressedAnimation.qml
    qml/animations/ReleasedAnimation.qml
)

ecm_target_qml_sources(plasmaextracomponentsplugin VERSION 2.0 PATH private SOURCES
    qml/private/BackgroundMetrics.qml
)

target_link_libraries(plasmaextracomponentsplugin
        Qt${QT_MAJOR_VERSION}::Quick
        Qt${QT_MAJOR_VERSION}::Qml
        Qt${QT_MAJOR_VERSION}::Widgets
        KF6::ConfigWidgets
        KF6::Plasma)

ecm_finalize_qml_module(plasmaextracomponentsplugin DESTINATION ${KDE_INSTALL_QMLDIR})
