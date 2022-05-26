#define BOOST_TEST_MODULE helloworld_test_module // файл только тесты без main(), сделает исполняемым
//           тест модуль  helloworld_test_module

#include "lib.h"

#include <boost/test/unit_test.hpp>  // boost unit_test

//+++ название теста helloworld_test_suite
BOOST_AUTO_TEST_SUITE(helloworld_test_suite)
// BOOST_AUTO_TEST_SUITE - макрос, вставит код cpp

// отдельный тест CASE, с именем helloworld_test_version
BOOST_AUTO_TEST_CASE(helloworld_test_version)
{
    BOOST_CHECK(version() > 0); // код выполняет тестирование
}

BOOST_AUTO_TEST_SUITE_END()
//---
