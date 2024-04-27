# SPDX-FileCopyrightText: 2024 Fushan Wen <qydwhotmail@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause

set(RUNNER "$<TARGET_FILE:${QT_CMAKE_EXPORT_NAMESPACE}::qmltestrunner>")
execute_process(COMMAND gcc -print-file-name=libasan.so OUTPUT_VARIABLE LIBASAN_PATH ECHO_OUTPUT_VARIABLE OUTPUT_STRIP_TRAILING_WHITESPACE)

add_test(
    NAME bug485688test
    COMMAND ${RUNNER} -import ${CMAKE_BINARY_DIR}/bin -input "${CMAKE_CURRENT_SOURCE_DIR}/bug485688.qml"
)
set_tests_properties(bug485688test PROPERTIES ENVIRONMENT "LD_PRELOAD=${LIBASAN_PATH}")
