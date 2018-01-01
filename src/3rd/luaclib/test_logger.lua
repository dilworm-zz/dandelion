local logger = require "logger.core"
assert(logger)
--[[
    trace = 0,
    debug = 1,
    info = 2,
    warn = 3,
    err = 4,
    critical = 5,
    off = 6
]]

logger.set_level(0)
logger.init("test.log", 100, 3)

for i=0,5 do
    logger.log(i, "hello logger!")
end

logger.set_level(4)
logger.log(3, "won't show") 
logger.log(4, "show") 
logger.log(5, "show") 



