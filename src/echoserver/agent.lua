local skynet = require "skynet"

local CMD = {}

function CMD.testuuid() 
    local ok, id = pcall(skynet.call, "uuid", "lua", "get")
    --assert(ok)
    --skynet.error(string.format("%s, get uuid %s", skynet.self(), id))
end

skynet.start(function()
    skynet.dispatch("lua", function(_, _, cmd, ...)
        local f = CMD[cmd]
        if not f then
            error(string.format("Unknown command %s", cmd))
        end
        skynet.ret(skynet.pack(f(...)))
    end)
end)
