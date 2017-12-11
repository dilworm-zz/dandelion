local skynet = require "skynet"
local mysql = require "skynet.db.mysql"

local CMD = {}
local db

function on_connect()
    skynet.error("mysql on_connect succeed")
    --local sql = "use testdb; " --create table if not exists testuuid(uuid bigint not null); "
    --local res = db:query(sql)
    --assert(res)
    skynet.error("create table testuuid succeed")
    --db:query("set charset utf8");
end

function init()
    db = mysql.connect({
        host = "127.0.0.1",
        port = 3306,
        database = "testdb",
        user = "lk",
        password = "123",
        on_connect = on_connect
    })
    assert(db)
end

function CMD.testuuid1() 
    local ok, id = pcall(skynet.call, "uuid", "lua", "get")
    assert(ok)
    skynet.error(string.format("%s, get uuid %s", skynet.self(), id))
end

function CMD.testuuid2() 
    local ok, id
    local sql, res
    local st = os.time()
    for i = 1, 100000 do
        ok, id = pcall(skynet.call, "uuid", "lua", "get")
        assert(ok)
        sql = string.format("insert into testuuid values(%s);", id)
        res = db:query(sql)
        --for k, v in pairs(res) do
        --    print(k, v)
        --end
    end
    skynet.error("testuuid2 finish used time =", os.time() - st)
end

skynet.start(function()
    skynet.dispatch("lua", function(_, _, cmd, ...)
        local f = CMD[cmd]
        if not f then
            error(string.format("Unknown command %s", cmd))
        end
        skynet.ret(skynet.pack(f(...)))
    end)

    init()
end)
