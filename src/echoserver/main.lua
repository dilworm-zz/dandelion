local skynet = require "skynet"
local sprotoloader = require "sprotoloader"

skynet.start(function()
	skynet.newservice("debug_console",8000)
    skynet.uniqueservice("uuid")

    local agents = {}
    for i=1,5 do
        agents[i] = skynet.newservice("agent", i)
    end

    for _, agent in pairs(agents) do
        for i=1,10000 do
           -- skynet.send(agent, "lua", "testuuid1")
        end
    end

    for _, agent in pairs(agents) do
        skynet.send(agent, "lua", "testuuid2")
    end

end)
