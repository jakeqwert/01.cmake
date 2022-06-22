cmake_minimum_required(VERSION 3.2)

# git add .gitlab-ci.yml CMakeLists.txt
# git commit -m "CMakeLists.txt for Gitlab CI"
# git push -u origin master

# project(cmake VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
# Jenkins    $BUILD_NUMBER

# Gitlab CI нет переменной VERSION
project(cmake)
set(PROJECT_VERSION_PATCH 11)

# Gitlab CI  $CI_PIPELINE_IID  CI_PIPELINE_ID  CI_COMMIT_SHA CI_COMMIT_SHORT_SHA
#                       docker build . -t myapp:$CI_COMMIT_SHA
#project(cmake VERSION 0.0.1$ENV{CI_COMMIT_SHORT_SHA})
#project(cmake VERSION 0.0.1$ENV{TRAVIS_BUILD_NUMBER})

# имя проекта = cmake
# автоматом сформировалось 3 переменные
# ${PROJECT_VERSION_MAJOR}  0
# ${PROJECT_VERSION_MINOR}  0
# ${PROJECT_VERSION_PATCH}  1


# подключаем зависимости, cmake попытается найти в системе зависимости
# если они не устанвовлена, то будет ошибка что он не смог найти, нужно установить в систему эти зависимости
find_package(Threads REQUIRED)
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

# взять шаблон version.h.in и получить из него файл ${PROJECT_SOURCE_DIR}/version.h
configure_file(version.h.in ${PROJECT_SOURCE_DIR}/version.h)
# файл 'version.h'
##pragma once
##define PROJECT_VERSION_PATCH 1
#// кодогенератор сформировал этот файл


add_executable(helloworld_cli main.cpp)
# исполняемый файл helloworld_cli
# артифакт сборки helloworld_cli
# для MS VS сюда еще нужно добавить файлы .h, там будет solution


add_library(helloworld lib.cpp)
# статическая библиотека (по-умолчанию) helloworld исполняемый файл libhelloworld.a

# add_library(helloworld shared lib.cpp)
# shared динамическая библиотека helloworld исполняемый файл


add_executable(test_version test_version.cpp)
# test_version исполняемый файл, тесты собираются в бинарник

#для сборки артифактов helloworld_cli helloworld test_version, нужны настройки конфигуратора
set_target_properties(helloworld_cli helloworld test_version PROPERTIES
    CXX_STANDARD 17               # стандарт cpp
    CXX_STANDARD_REQUIRED ON      # если не дотягиваем до 17 стандарт, то ошибка
    COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra"   # опции компиляции, для линукс тут параметры
                     # pedantic; -all;-extra ворнинги при компиляции
)

# для сборки test_version нужно задать свойста компилятора
set_target_properties(test_version PROPERTIES
    COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)

# прилинковать статическую библиотеку helloworld и Threads
target_link_libraries(helloworld_cli
    helloworld        # статическая библиотека libhelloworld.a под линуксом
    Threads::Threads  #
)

# к test_version прилинковать статическую библиотеку helloworld и Boost_LIBRARIES
target_link_libraries(test_version
    ${Boost_LIBRARIES}
    helloworld          # статическая библиотека libhelloworld.a под линуксом
)

# установить helloworld_cli в user\bin
install(TARGETS helloworld_cli RUNTIME DESTINATION bin)

# собрать пакет DEB для Линукс
set(CPACK_GENERATOR DEB)

# указываем версии пакета из версии программы
#set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
#set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
#set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_VERSION_MAJOR 0)
set(CPACK_PACKAGE_VERSION_MINOR 0)
#set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_VERSION_PATCH 33)

# ответственный за пакет
set(CPACK_PACKAGE_CONTACT eliseevz@mail.ru)

# inna setup для windows, система сборки для windows
# включить опцию CPACK_DEBIAN_PACKAGE_SHLIBDEPS для Debian, т.е. включит информацию
# о зависимых библиотек, найдет в каких они пакетах и добавит информацию о зависимости в данный пакет
# т.е. при установке данного пакета будет установлен boost пакет
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
# либо если не использовать эту опции, то вручную вписывать зависимые пакеты

#собрать пакет
include(CPack)

# добавляет новую виртуальную цель сборки test
enable_testing()
# последовательно будут запущены все тесты helloworld_tests test_version
add_test(helloworld_tests test_version)
# задача с именем helloworld_tests, за выполнение задачи отвечает бинарник test_version
