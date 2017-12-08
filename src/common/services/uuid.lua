local skynet = require "skynet"
require "skynet.manager" -- import skynet.register
local uo = require "util.core"


local CMD = {}
local nodeid = tonumber(skynet.getenv ("nodeid") or 1)
assert(nodeid < 1024)

local reference_time = os.time({year=2017,month=1,day=1})
local next_seq = 1

-- gen uuid using 'snowflake'
-- return value: 64 bit integer
--      41 bits of millonsecond: support 40 year, math.log(40*356*24*3600*1000, 2) = 41
--      10 bits of node ids: maxinum of 1024 nodes 
--      12 bits of sequence number: 400w/per second
function CMD.get()
    local ti = uo.get_realtime()
    if not (ti and ti.sec and ti.usec) then
        error("get_realtime failed")
    end
    
    local millonsec = ti.sec * 1000 + math.floor(ti.usec /1000)
    local ret = math.tointeger(millonsec) << 22 
    ret = ret + (nodeid << 12)
    if next_seq == 4096 then
        next_seq = 1
    end
    ret = ret + next_seq
    next_seq = next_seq + 1
    skynet.error(string.format("new uuid %s", ret))

    return ret
end

skynet.start(function()
    skynet.dispatch("lua", function(_, _, cmd, ...)
        local f = CMD[cmd]
        if not f then
            error(string.format("Unknown command %s", tostring(cmd)))
        end
        skynet.ret(skynet.pack(f(...)))
    end)

    skynet.register "uuid"
end)
