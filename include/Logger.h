//
// Created by quals on 2019-07-03.
//

#ifndef LOGGERDEMO_LOGGER_H
#define LOGGERDEMO_LOGGER_H

#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>
#include <chrono>
#include <sys/stat.h>
#include <map>

using namespace std::chrono;

#define LOGGER_PRINT_LIKE(fmtarg, firstvararg) __attribute__((__format__(__printf__,fmtarg, firstvararg)))

namespace Logger {
    enum class Verbosity {
        FATAL = -3,
        ERROR = -2,
        WARN = -1,
        INFO = 0,
    };

    enum FILE_MODE {
        TRUNC,
        APPEND
    };

    struct Message {
        Verbosity verbosity;
        const std::string file;
        int line;
        const std::string func;
        const std::string preamble;
        const std::string prefix;
        const std::string message;

        Message(Verbosity verbosity, std::string file, int line, std::string func, std::string preamble, std::string prefix, std::string message) : verbosity(
                verbosity), file(std::move(file)), line(line), func(std::move(func)), preamble(std::move(preamble)), prefix(std::move(prefix)), message(std::move(message)) {}

        Message(const Message &rhs) : verbosity(rhs.verbosity), file(rhs.file), line(rhs.line), func(rhs.func), preamble(rhs.preamble), message(rhs.message) {
        }
    };

    typedef void (*callback_handler_t)(void *user_data, const Message &message);

    typedef void (*close_handler_t)(void *user_data);

    typedef void (*exit_handler_t)();

    struct Callback {
        std::string id;
        void *user_data;
        callback_handler_t f_callback;
        close_handler_t f_close;
    };

    using CallbackMap = std::map<std::string, Callback>;
    using MessageVec = std::vector<Message>;

    std::string s_arguments;
    CallbackMap s_callbacks;
    std::string s_current_dir;
    exit_handler_t s_exit_handler;
    MessageVec s_MessageBacklog;
    std::shared_ptr<Message> s_msg_arguments;
    std::shared_ptr<Message> s_msg_current_dir;
    FILE *s_out = stdout;

    auto start_time = system_clock::now();

    void init(int argc, char *argv[]);

    void add_callback(std::string id, void *user_data, callback_handler_t callback, close_handler_t close);

    void remove_all_callbacks();

    void log(Verbosity verbosity, const char *file, int line, const char *func, const char *format, ...) LOGGER_PRINT_LIKE(5, 6);

    void log_err(Verbosity verbosity, const char *file, int line, const char *func, const char *format, ...);

    void on_assertion_failed(Verbosity verbosity, const char *expr, const char *file, int line, const char *func, const char *format, ...);

    void on_assertion_failed(Verbosity verbosity, const char *expr, const char *file, int line, const char *func, ...);


    void on_assertion_failed_err(Verbosity verbosity, const char *expr, const char *file, int line, const char *func, const char *format, ...);

    void on_assertion_failed_err(Verbosity verbosity, const char *expr, const char *file, int line, const char *func, ...);

    void shutdown();

    bool add_file(const char *path_in, FILE_MODE mode);

}

#define LOGEV(verbosity, format, ...) log_err(verbosity, __FILE__, __LINE__, __PRETTY_FUNCTION__, format, ##__VA_ARGS__)
#define LOGE(verbosity, format, ...) LOGEV(Logger::Verbosity::verbosity, format, ##__VA_ARGS__)

#define LOGV(verbosity, format, ...) log(verbosity, __FILE__, __LINE__, __PRETTY_FUNCTION__, format, ##__VA_ARGS__)
#define LOG(verbosity, format, ...) LOGV(Logger::Verbosity::verbosity, format, ##__VA_ARGS__)

#define CHECKV(verbosity, test, info, ...) if((test) == false) on_assertion_failed(verbosity, info, __FILE__, __LINE__, __PRETTY_FUNCTION__, ##__VA_ARGS__)
#define CHECK(verbosity, test, ...) CHECKV(Logger::Verbosity::verbosity, test, "test " #test " failed.", ##__VA_ARGS__)

#define CHECKEV(verbosity, test, info, ...) if((test) == false) on_assertion_failed_err(verbosity, info, __FILE__, __LINE__, __PRETTY_FUNCTION__, ##__VA_ARGS__)
#define CHECKE(verbosity, test, ...) CHECKEV(Logger::Verbosity::verbosity, test, "test " #test " failed.", ##__VA_ARGS__)

namespace Logger {
    const char *PREAMBLE_EXPLAIN = "date       time            (    uptime    ) [   thread name/id     ]                 file:line    func                                                       v| ";

    std::string strprintf(const char *format, ...) LOGGER_PRINT_LIKE(1, 2);

    Message simple_msg(Verbosity verbosity, const char *format, ...) LOGGER_PRINT_LIKE(2, 3);

    void on_assertion_failed_err(Verbosity verbosity, const char *expr, const char *file, int line, const char *func, const char *format, va_list ap);

    void init(int argc, char *argv[]) {
        for (int i = 0; i < argc; i++) {
            s_arguments.append(argv[i]);
            if (i != argc - 1) {
                s_arguments.append(" ");
            }
        }

        printf("%s\n", PREAMBLE_EXPLAIN);

        char current_dir_str[PATH_MAX];
        if (getcwd(current_dir_str, PATH_MAX) == nullptr) {
            LOGE(ERROR, "get current dir failed");
        }
        s_current_dir = current_dir_str;


        LOG(INFO, "%s", s_arguments.c_str());
        LOG(INFO, "Program is running in %s", current_dir_str);

        if (s_exit_handler) {
            atexit(s_exit_handler);
        }
    }

    std::string strprintf(const char *format, va_list ap) {
        char *s;
        vasprintf(&s, format, ap);
        std::string res(s);
        free(s);
        return res;
    }

    std::string strprintf(const char *format, ...) {
        va_list ap;
        va_start(ap, format);
        std::string res = strprintf(format, ap);
        va_end(ap);
        return res;
    }

    std::string get_preamble(const char *file, int line, const char *func, Verbosity verbosity) {
        char date_str[12], time_str[20], uptime_str[20], thread_name_str[30], v_str[5], file_str[22];

        auto now = system_clock::now();
        time_t us_since_epoch = duration_cast<microseconds>(now.time_since_epoch()).count();
        time_t sec_since_epoch = us_since_epoch / 1000000;
        tm time_info;
        localtime_r(&sec_since_epoch, &time_info);

        time_t us_uptime = duration_cast<microseconds>(now - start_time).count();
        time_t sec_uptime = us_uptime / 1000000;

        snprintf(date_str, 12, "%04d-%d-%d", 1900 + time_info.tm_year, time_info.tm_mon, time_info.tm_mday);
        snprintf(time_str, 20, "%02d:%02d:%02d.%06ld", time_info.tm_hour, time_info.tm_min, time_info.tm_sec, us_since_epoch % 1000000);
        snprintf(uptime_str, 20, "%ld.%06ld", sec_uptime, us_uptime % 1000000);

        pthread_getname_np(pthread_self(), thread_name_str, 23);
        if (strlen(thread_name_str) == 0) {
            uint64_t id;
            pthread_threadid_np(pthread_self(), &id);
            snprintf(thread_name_str, 30, "%08X", static_cast<unsigned int>(id));
        }

        if (verbosity > Verbosity::WARN) {
            snprintf(v_str, 5, "%d", verbosity);
        } else {
            snprintf(v_str, 5, "%s", verbosity == Verbosity::WARN ? "warn" : verbosity == Verbosity::ERROR ? "err" : "fat");
        }

        const char *p = strrchr(file, '/');
        snprintf(file_str, 22, "%s", p ? p + 1 : file);

        return strprintf("%-10s %-15s (%-14s) [%-22s] %20s:%-7d %-55s %4s|", date_str, time_str, uptime_str, thread_name_str, file_str, line, func, v_str);
    }


    void log_message(FILE *file, const Message &message) {
        s_MessageBacklog.push_back(message);
        if (s_MessageBacklog.size() == 1) {
            s_msg_arguments = std::make_shared<Message>(s_MessageBacklog[0]);
        }
        if (s_MessageBacklog.size() == 2) {
            s_msg_current_dir = std::make_shared<Message>(s_MessageBacklog[1]);
        }
        fprintf(file, "%s %s %s\n", message.preamble.c_str(), message.prefix.c_str(), message.message.c_str());
        fflush(file);
    }

    void logv(Verbosity verbosity, const char *file, int line, const char *func, const char *format, va_list ap) {
        std::string preamble = get_preamble(file, line, func, verbosity);
        std::string message_str = strprintf(format, ap);
        Message message(verbosity, file, line, func, preamble, "", message_str.length() ? message_str : "");
        log_message(s_out, message);

        for (auto &p:s_callbacks) {
            if (p.second.f_callback) {
                p.second.f_callback(p.second.user_data, message);
            }
        }

        if (verbosity == Verbosity::FATAL) {
            Message shutdown_message(verbosity, file, line, func, preamble, "", "Program shutdown");
            log_message(s_out, shutdown_message);
            for (auto &p:s_callbacks) {
                if (p.second.f_callback) {
                    p.second.f_callback(p.second.user_data, shutdown_message);
                }
            }
            shutdown();
        }
    }

    void log(Verbosity verbosity, const char *file, int line, const char *func, const char *format, ...) {
        va_list ap;
        va_start(ap, format);
        logv(verbosity, file, line, func, format, ap);
        va_end(ap);
    }

    void remove_all_callbacks() {
        for (auto it = s_callbacks.begin(); it != s_callbacks.end();) {
            it->second.f_close(it->second.user_data);
            it = s_callbacks.erase(it);
        }
    }

    void add_callback(std::string id, void *user_data, callback_handler_t callback, close_handler_t close) {
        s_callbacks[id] = {id, user_data, callback, close};
    }

    void log_err(Verbosity verbosity, const char *file, int line, const char *func, const char *format, ...) {
        va_list ap;
        va_start(ap, format);
        std::string new_format = strprintf("%s errno=%d error msg: %s", format, errno, strerror(errno));
        logv(verbosity, file, line, func, new_format.c_str(), ap);
        va_end(ap);
    }

    void on_assertion_failed(Verbosity verbosity, const char *expr, const char *file, int line, const char *func, const char *format, va_list ap) {
        std::string new_format = strprintf("%s %s", expr, format);
        logv(verbosity, file, line, func, new_format.c_str(), ap);
    }

    void on_assertion_failed(Verbosity verbosity, const char *expr, const char *file, int line, const char *func, ...) {
        va_list ap;
        va_start(ap, func);
        on_assertion_failed(verbosity, expr, file, line, func, "", ap);
        va_end(ap);
    }

    void on_assertion_failed(Verbosity verbosity, const char *expr, const char *file, int line, const char *func, const char *format, ...) {
        va_list ap;
        va_start(ap, format);
        on_assertion_failed(verbosity, expr, file, line, func, format, ap);
        va_end(ap);
    }

    void shutdown() {
        remove_all_callbacks();
        exit(0);
    }

    char *home_dir() {
        char *home = getenv("HOME");
        CHECK(FATAL, home != nullptr, "Get home dir failed");
        return home;
    }

    bool mkpath(const char *path_in) {
        char *path = strdup(path_in);
        for (char *p = strchr(path, '/'); p; p = strchr(p + 1, '/')) {
            *p = '\0';
            if (mkdir(path, 0755) == -1) {
                if (errno != EEXIST) {
                    LOGE(ERROR, "Create directory %s failed", path);
                    *p = '/';
                    free(path);
                    return false;
                }
            }
            *p = '/';
        }
        free(path);
        return true;
    }

    void file_log(void *user_data, const Message &message) {
        FILE *file = static_cast<FILE *>(user_data);
        log_message(file, message);
    }

    void file_close(void *user_data) {
        FILE *file = static_cast<FILE *>(user_data);
        fclose(file);
    }

    Message simple_msg(Verbosity verbosity, const char *format, va_list ap) {
        std::string message_str = strprintf(format, ap);
        std::string preamble = get_preamble("", 0, "", verbosity);
        return Message{verbosity, "", 0, "", preamble, "", message_str};
    }

    Message simple_msg(Verbosity verbosity, const char *format, ...) {
        va_list ap;
        va_start(ap, format);
        Message res = simple_msg(verbosity, format, ap);
        va_end(ap);
        return res;
    }

    bool add_file(const char *path_in, FILE_MODE mode) {
        char path[PATH_MAX];
        if (path_in[0] == '~') {
            snprintf(path, PATH_MAX, "%s%s", home_dir(), path_in + 1);
        } else {
            snprintf(path, PATH_MAX, "%s", path_in);
        }

        if (!mkpath(path)) {
            return false;
        }

        FILE *file = fopen(path, mode == FILE_MODE::TRUNC ? "w" : "a");
        std::string callback_id = "log file: path";
        add_callback(callback_id, file, file_log, file_close);

        fprintf(file, "%s\n", PREAMBLE_EXPLAIN);
        log_message(file, *s_msg_arguments);
        log_message(file, *s_msg_current_dir);
        fflush(file);

        return true;
    }

    void on_assertion_failed_err(Verbosity verbosity, const char *expr, const char *file, int line, const char *func, ...) {
        va_list ap;
        va_start(ap, func);
        on_assertion_failed_err(verbosity, expr, file, line, func, "", ap);
        va_end(ap);
    }

    void on_assertion_failed_err(Verbosity verbosity, const char *expr, const char *file, int line, const char *func, const char *format, ...) {
        va_list ap;
        va_start(ap, format);
        on_assertion_failed_err(verbosity, expr, file, line, func, format, ap);
        va_end(ap);
    }

    void on_assertion_failed_err(Verbosity verbosity, const char *expr, const char *file, int line, const char *func, const char *format, va_list ap) {
        std::string new_format = strprintf("%s %s. errno=%d. error message: %s", expr, format, errno, strerror(errno));
        logv(verbosity, file, line, func, new_format.c_str(), ap);
    }

}
#endif //LOGGERDEMO_LOGGER_H
