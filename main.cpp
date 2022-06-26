#include "lib.h"

#include <spdlog/spdlog.h> // для локальной сборки sudo apt install libspdlog-dev
                           // travis CI подготовит образ с пакетом нужным

//

// git status
// git add .
// git commit -m "edit CMakeLists.txt, .github/workflows/pipeline.yml for GitHubAction"
// git push origin
// git push origin_loc
// git push origin_rg

//$ git remote -v
//origin  git@github.com:jakeqwert/01.cmake.git (fetch)
//origin  git@github.com:jakeqwert/01.cmake.git (push)
//origin_loc      git@gitlab:root/01.cmake.git (fetch)
//origin_loc      git@gitlab:root/01.cmake.git (push)
//origin_rg       http://h034-ubu-01/jake/01.cmake.git (fetch)
//origin_rg       http://h034-ubu-01/jake/01.cmake.git (push)

// Запуск                   git@gitlab:root/01.cmake.git
// docker-compose up -d     Create and start containers /запустите проект в фоновом режиме

int main()
{
    auto logger = spdlog::stdout_logger_mt("console"); //пишем в console
    logger->info("version {} was started", version());

    return 0;
}
