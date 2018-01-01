#include <ctime>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/base_sink.h"
#include "spdlog/details/file_helper.h"

namespace spdlog
{
namespace sinks
{

class SizeAndDayFileSink : public sink
{
public:
    SizeAndDayFileSink(
        const filename_t& base_filename, 
        std::size_t max_size, 
        std::size_t max_files):
        _base_filename(base_filename),
        _max_size(max_size),
        _max_files(max_files),
        _file_helper()
    {
        _file_helper.open(_base_filename);
        _current_size = _file_helper.size();
        _next_index = 1;
        _next_rotate_timepoint = _next_rotate_tp(std::chrono::system_clock::now());
    }

    void log(const details::log_msg& msg)
    {
        _rotate(msg.time, msg.formatted.size());
        _current_size += msg.formatted.size();
        _file_helper.write(msg);
        //printf("current_size %ld\n", _current_size);
    }

    void flush()
    {
        _file_helper.flush();
    }

private:
    std::chrono::system_clock::time_point _next_rotate_tp(const std::chrono::system_clock::time_point& now)
    {
        time_t tnow = std::chrono::system_clock::to_time_t(now);
        tm date = spdlog::details::os::localtime(tnow);
        date.tm_hour = 0;
        date.tm_min = 0;
        date.tm_sec = 0;

        auto rotate_tp = std::chrono::system_clock::from_time_t(std::mktime(&date)); 
        return std::chrono::system_clock::time_point(rotate_tp + std::chrono::hours(24));
    }
        
    std::string _calc_rotation_filename(const log_clock::time_point& now) {
        std::chrono::system_clock::time_point tp;
        if (now >= _next_rotate_timepoint) { 
            tp = _next_rotate_timepoint - std::chrono::seconds(1);
        } else {
            tp = now;
        }

        time_t tt = std::chrono::system_clock::to_time_t(tp);
        tm date = spdlog::details::os::localtime(tt);
        char name[1024] = {0};
        snprintf(name, sizeof(name), "%s.%04d%02d%02d.%d", 
                 _base_filename.c_str(), 
                 date.tm_year + 1900, 
                 date.tm_mon + 1,
                 date.tm_mday,
                 _next_index);
        return name;
    }

    void _rotate(const log_clock::time_point& now, int size) {
        using details::os::filename_to_str;
        if (now >= _next_rotate_timepoint) 
        {
            _file_helper.close();
            std::string target = _calc_rotation_filename(now);
            if (details::file_helper::file_exists(target) && details::os::rename(_base_filename, target))
            {
                throw spdlog_ex("rotating_file_sink: failed renaming " + filename_to_str(_base_filename) + " to " + filename_to_str(target), errno);
            }
            _file_helper.reopen(true);
            
            _next_rotate_timepoint = _next_rotate_tp(now);
            _next_index = 1;
            _current_size = 0;
        }

        if (_current_size + size > _max_size)
        {
            _file_helper.close();
            std::string target = _calc_rotation_filename(now);
            if (details::file_helper::file_exists(_base_filename) && 
                details::os::rename(_base_filename, target))
            {
                throw spdlog_ex("rotating_file_sink: failed renaming " + filename_to_str(_base_filename) + " to " + filename_to_str(target), errno);
            }
            _file_helper.reopen(true);

            _next_index++;
            _current_size = 0;
        }
    }

    filename_t _base_filename;
    std::size_t _max_size;
    std::size_t _max_files;
    std::size_t _current_size;
    int _next_index;
    std::chrono::system_clock::time_point _next_rotate_timepoint; // 下一个 00:00:00 
    details::file_helper _file_helper;
};

}
}
