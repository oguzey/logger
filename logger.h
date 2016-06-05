#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include "spdlog/include/spdlog/spdlog.h"

namespace simple_logger {
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

simple_logger::Logger& my_logger = simple_logger::Logger::get_logger();

#else
extern simple_logger::Logger& my_logger;
#endif


#define INFO(...)        my_logger.info(__VA_ARGS__)
#define LOG(...)         my_logger.info(__VA_ARGS__)
#define WARN(...)        my_logger.warn(__VA_ARGS__)
#define CRITICAL(...)    my_logger.critical(__VA_ARGS__)

#ifndef NDEBUG
#include <string.h>
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define DEBUG(...)  my_logger.debug("[{}:{}]", __FILENAME__, __LINE__); my_logger.debug(__VA_ARGS__)
#else
#define DEBUG(...)
#endif

#endif //LOGGER_H
