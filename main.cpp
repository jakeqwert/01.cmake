#include "lib.h"

#include <spdlog/spdlog.h> // для локальной сборки sudo apt install libspdlog-dev
                           // travis CI подготовит образ с пакетом нужным

int main()
{
    auto logger = spdlog::stdout_logger_mt("console"); //пишем в console
    logger->info("version {} was started", version());

    return 0;
}
