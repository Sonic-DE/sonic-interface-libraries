# SPDX-FileCopyrightText: 2024 Fushan Wen <qydwhotmail@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause

ecm_query_qt(QtBinariesDir QT_INSTALL_BINS)

add_test(
    NAME bug485688test
    COMMAND ${QT_INSTALL_BINS}/qmltestrunner -input ${CMAKE_CURRENT_SOURCE_DIR}/bug485688.qml
)
execute_process(COMMAND gcc -print-file-name=libasan.so OUTPUT_VARIABLE LIBASAN_PATH ECHO_OUTPUT_VARIABLE OUTPUT_STRIP_TRAILING_WHITESPACE)
set_tests_properties(bug485688test PROPERTIES ENVIRONMENT "LD_PRELOAD=${LIBASAN_PATH};QML_EXEC=${QT_INSTALL_BINS}/qml")
