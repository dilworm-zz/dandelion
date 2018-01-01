#define SPDLOG_FMT_PRINTF 
#include "spdlog/spdlog.h"
#include "spdlog_time_and_size_file_sink.h"
#include <lua.hpp>

static std::shared_ptr<spdlog::logger> g_file_logger;
static std::shared_ptr<spdlog::logger> g_console_logger;

static std::string g_filename = "dandelion.log";
static spdlog::level::level_enum g_log_level = spdlog::level::level_enum::info;

static size_t g_rotate_size = 1024*1024*1024; // 1GB
static size_t g_rotate_count = 10; 

//    trace = 0,
//    debug = 1,
//    info = 2,
//    warn = 3,
//    err = 4,
//    critical = 5,
//    off = 6

extern "C" {

static int
L_log(lua_State* L) {
    int level = luaL_checkinteger(L, 1);
    const char* logmsg = luaL_checkstring(L, 2);

    if (level == 2) {
        g_file_logger->info(logmsg);
        g_console_logger->info(logmsg);
    } else if (level == 3) {
        g_file_logger->warn(logmsg);
        g_console_logger->warn(logmsg);
    } else if (level == 4) {
        g_file_logger->error(logmsg);
        g_console_logger->error(logmsg);
    } else if (level == 1) {
        g_file_logger->debug(logmsg);
        g_console_logger->debug(logmsg);
    } else if (level == 0) {
        g_file_logger->trace(logmsg);
        g_console_logger->trace(logmsg);
    } else if (level == 5) {
        g_file_logger->critical(logmsg);
        g_console_logger->critical(logmsg);
    } else {
        luaL_error(L, "unknow loglevel %d", level);
    }

    return 0;
}


static int
L_set_level(lua_State* L) {
    int level = lua_tointeger(L, -1);
    if (level > 6 || level < 0) {
        luaL_error(L, "level out of range[0,6]");
    }
    g_log_level = static_cast<spdlog::level::level_enum>(level);
    spdlog::set_level(g_log_level);
    return 0;
}

static int 
L_set_filename(lua_State* L) {
   const char* filename = luaL_checkstring(L, -1);
   g_filename = filename;
   return 0;
}

static int
L_set_rotate(lua_State* L) {
    g_rotate_size = luaL_checkinteger(L, -1);
    g_rotate_count = luaL_checkinteger(L, -1);
    return 0;
}

static int 
L_init(lua_State* L) {
    if (g_file_logger or g_console_logger) {
        luaL_error(L, "Can't init logger twice!");
    }

    std::string filename = luaL_checkstring(L, 1);
    size_t max_size = luaL_checkinteger(L, 2);
    size_t max_files = luaL_checkinteger(L, 3);

    auto sdsink = std::make_shared<spdlog::sinks::SizeAndDayFileSink>(
            filename, max_size, max_files);
    g_file_logger = std::make_shared<spdlog::logger>("dandelion", sdsink);
    g_console_logger = spdlog::stderr_color_st("dandelion");

    return 0;
}


int 
luaopen_logger_core(lua_State* L) {
    luaL_checkversion(L);
    luaL_Reg l[] = {
        {"init", L_init},
        {"set_level", L_set_level},
        {"set_filename", L_set_filename},
        {"set_rotate", L_set_rotate},
        {"log", L_log},
        {NULL, NULL},
    };

    luaL_newlib(L, l);
    return 1;
} 

} // extern "C"

