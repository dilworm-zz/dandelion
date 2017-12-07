local uc = require "util.core"
assert(uc)

local st = os.time()
print("t ", st)
for i=1,10000000 do
    uc.get_realtime()
end
print("t 100w", os.time() - st)

for k,v in pairs(uc.get_realtime()) do
    print (k,v)
end

