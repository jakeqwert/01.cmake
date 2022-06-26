#define BOOST_TEST_MODULE test_version_test_module // файл только тесты без main(), сделает исполняемым
//           тест модуль  test_version_test_module
#include "lib.h"

#include <boost/test/unit_test.hpp>  // boost unit_test

//+++ название теста project_01_cmake_test_suite
BOOST_AUTO_TEST_SUITE(project_01_cmake_test_suite)
// BOOST_AUTO_TEST_SUITE - макрос, вставит код cpp

// отдельный тест CASE, с именем test_valid_version
BOOST_AUTO_TEST_CASE(test_valid_version)
{
    BOOST_CHECK(version() > 0); // код выполняет тестирование
}

BOOST_AUTO_TEST_SUITE_END()
//---
