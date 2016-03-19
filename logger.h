#ifndef LOGGER_H
#define LOGGER_H

#include "spdlog/include/spdlog/spdlog.h"

namespace logger {
    class Logger {
    private:
        std::shared_ptr<spdlog::logger> console_logger;

        Logger() {
            std::cout << "Called constructor of Logger" << std::endl;
            try {
                this->console_logger = spdlog::stdout_logger_st("console");
                spdlog::set_level(spdlog::level::debug);
                spdlog::set_pattern("%l\t: %v");
            }
            catch (const spdlog::spdlog_ex &ex) {
                std::cout << "Log failed: " << ex.what() << std::endl;
                exit(-1);
            }
        }

        Logger(Logger const &) = delete;
        void operator=(Logger const &) = delete;

    public:
        ~Logger() {
            std::cout << "Called destructor of Logger" << std::endl;
        }

        static Logger &get_logger() {
            static Logger new_loger;
            return new_loger;
        }

        void log(std::string a_str) {
            this->console_logger->debug("{}", a_str);
        }

        template<typename... Args>
        void debug(const char *fmt, const Args... args) {
            this->console_logger->debug(fmt, args...);
        }

        template<typename... Args>
        void info(const char *fmt, const Args... args) {
            this->console_logger->info(fmt, args...);
        }

        template<typename... Args>
        void warn(const char *fmt, const Args... args) {
            this->console_logger->warn(fmt, args...);
        }

        template<typename... Args>
        void critical(const char *fmt, const Args... args) {
            this->console_logger->critical(fmt, args...);
            exit(-1);
        }
    };
}

#ifdef ALLOCATE_LOGGER

logger::Logger& my_logger = logger::Logger::get_logger();

#else
extern logger::Logger& my_logger;
#endif


#define INFO(...)        my_logger.info(__VA_ARGS__)
#define LOG(...)         my_logger.info(__VA_ARGS__)
#define WARN(...)        my_logger.warn(__VA_ARGS__)
#define CRITICAL(...)    my_logger.critical(__VA_ARGS__)

#ifdef DEBUG_ON
#include <string.h>
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define DEBUG(format, ...) logger.debug("[{}:{}] " format, __FILENAME__, __LINE__, ##__VA_ARGS__)
#else
#define DEBUG(format, ...)
#endif

#endif //MODERN_CRYPTOLOGY_LOGGER_H
