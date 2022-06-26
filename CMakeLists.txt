cmake_minimum_required(VERSION 3.2)

if(DEFINED ENV{GITHUB_RUN_NUMBER})
    set(PATCH_NUM $ENV{GITHUB_RUN_NUMBER})
else()
    set(PATCH_NUM 1)
endif()

#set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
#set(PROJECT_VESRION 0.0.${PATCH_VERSION})
#project(helloworld VERSION ${PROJECT_VESRION})

# project(cmake VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

# Gitlab CI нет переменной VERSION
#project(cmake)
#set(PROJECT_VERSION_MAJOR 1)
#set(PROJECT_VERSION_MINOR 1)
##set(PROJECT_VERSION_PATCH 15)
##set(PROJECT_VERSION_PATCH $ENV{CI_PIPELINE_ID})  # Gitlab CI
##message($ENV{CI_PIPELINE_ID})

# GitHub Action
project(cmake
        LANGUAGES CXX
        VERSION 1.1.${PATCH_NUM}
        )
# Обращение к переменным окружения из CMakeLists.txt $ENV{GITHUB_RUN_NUMBER}
#message("CMakeLists.txt GITHUB_RUN_NUMBER = $ENV{GITHUB_RUN_NUMBER}")
#set(PROJECT_VERSION_PATCH $ENV{GITHUB_RUN_NUMBER})  # GitHub Action environment-variables


# Jenkins    $BUILD_NUMBER
#set(PROJECT_VERSION_PATCH $BUILD_NUMBER)

# задать переменную окружения
#set(ENV{MYENVVAR} "My-Content")
#message($ENV{MYENVVAR})

#alex@alex-ubuntu:~$ export MYENVVAR=FooBarBaz
#alex@alex-ubuntu:~$ cat test.cmake
#cmake_minimum_required(VERSION 3.20)
#message($ENV{MYENVVAR})
#set(ENV{MYENVVAR} "My-Content")
#message($ENV{MYENVVAR})
#alex@alex-ubuntu:~$ cmake -P test.cmake
#FooBarBaz
#My-Content
#alex@alex-ubuntu:~$ echo $MYENVVAR  //CMake может как считывать переменную среды, так и записывать в нее в процессе, но это не влияет на среду управляющего процесса
#FooBarBaz


# Gitlab CI  $CI_PIPELINE_IID  CI_PIPELINE_ID  CI_COMMIT_SHA CI_COMMIT_SHORT_SHA
#                       docker build . -t myapp:$CI_COMMIT_SHA
#project(cmake VERSION 0.0.1$ENV{CI_COMMIT_SHORT_SHA})
#project(cmake VERSION 0.0.1$ENV{TRAVIS_BUILD_NUMBER})

# имя проекта = cmake
# автоматом сформировалось 3 переменные
# ${PROJECT_VERSION_MAJOR}  0
# ${PROJECT_VERSION_MINOR}  0
# ${PROJECT_VERSION_PATCH}  1


set(CMAKE_BUILD_TYPE Debug)

# Нужно ли создавать Boost-тест
option(WITH_BOOST_TEST "Whether to build Boost test" ON)

#include_directories(${CMAKE_BINARY_DIR})
# add_subdirectory(src)  # исходники в папке src

# подключаем зависимости, cmake попытается найти в системе зависимости
# если они не устанвовлена, то будет ошибка что он не смог найти, нужно установить в систему эти зависимости
find_package(Threads REQUIRED)
#find_package(Boost COMPONENTS unit_test_framework REQUIRED)

# взять шаблон version.h.in и получить из него файл ${PROJECT_SOURCE_DIR}/version.h
configure_file(version.h.in ${PROJECT_SOURCE_DIR}/version.h)
#configure_file(version.h.in version.h)
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


#для сборки артифактов helloworld_cli helloworld test_version, нужны настройки конфигуратора
set_target_properties(helloworld_cli helloworld PROPERTIES  # test_version перенес в if
    CXX_STANDARD 17               # стандарт cpp
    CXX_STANDARD_REQUIRED ON      # если не дотягиваем до 17 стандарт, то ошибка
#    COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra"   # опции компиляции, для линукс тут параметры
                     # pedantic; -all;-extra ворнинги при компиляции
)

# прилинковать статическую библиотеку helloworld и Threads
target_link_libraries(helloworld_cli  # PRIVATE
        helloworld        # статическая библиотека libhelloworld.a под линуксом
        Threads::Threads  #
        )

if(WITH_BOOST_TEST)
    find_package(Boost COMPONENTS unit_test_framework REQUIRED)

    add_executable(test_version test_version.cpp)
    # test_version исполняемый файл, тесты собираются в бинарник

    # для сборки test_version нужно задать свойста компилятора
    set_target_properties(test_version PROPERTIES
       COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
       INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}

       CXX_STANDARD 17               # стандарт cpp
       CXX_STANDARD_REQUIRED ON      # если не дотягиваем до 17 стандарт, то ошибка
#       COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra"   # опции компиляции, для линукс тут параметры
                 # pedantic; -all;-extra ворнинги при компиляции
    )

    # к test_version прилинковать статическую библиотеку helloworld и Boost_LIBRARIES
    target_link_libraries(test_version
            ${Boost_LIBRARIES}
            helloworld          # статическая библиотека libhelloworld.a под линуксом
            )
endif()


if (MSVC)
    target_compile_options(helloworld_cli PRIVATE
            /W4
            )
    target_compile_options(helloworld PRIVATE
            /W4
            )
    if(WITH_BOOST_TEST)
        target_compile_options(test_version PRIVATE
                /W4
                )
    endif()
else ()
    target_compile_options(helloworld_cli PRIVATE
            -Wall -Wextra -pedantic -Werror
            )
    target_compile_options(helloworld PRIVATE
            -Wall -Wextra -pedantic -Werror
            )
    if(WITH_BOOST_TEST)
        target_compile_options(test_version PRIVATE
                -Wall -Wextra -pedantic -Werror
                )
    endif()
endif()



# установить helloworld_cli в user\bin
install(TARGETS helloworld_cli RUNTIME DESTINATION bin)

# собрать пакет DEB для Линукс
set(CPACK_GENERATOR DEB)

# указываем версии пакета из версии программы
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
#set(CPACK_PACKAGE_VERSION_MAJOR 1)
#set(CPACK_PACKAGE_VERSION_MINOR 1)
#set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

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

if(WITH_BOOST_TEST)
   # добавляет новую виртуальную цель сборки test
   enable_testing()
   # последовательно будут запущены все тесты helloworld_tests test_version
   add_test(helloworld_tests test_version)
   # задача с именем helloworld_tests, за выполнение задачи отвечает бинарник test_version
endif()
