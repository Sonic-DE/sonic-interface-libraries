# SPDX-FileCopyrightText: 2024 Fushan Wen <qydwhotmail@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause

add_executable(qmltest qmltest.cpp)
target_link_libraries(qmltest Qt::QuickTest Qt::Qml)

add_test(
    NAME bug485688test
    COMMAND qmltest -import ${CMAKE_BINARY_DIR}/bin -input "${CMAKE_CURRENT_SOURCE_DIR}/bug485688.qml"
)
set_tests_properties(bug485688test PROPERTIES ENVIRONMENT "LD_PRELOAD=${LIBASAN_PATH}")
