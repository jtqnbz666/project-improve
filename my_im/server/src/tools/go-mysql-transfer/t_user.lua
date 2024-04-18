local ops = require("redisOps")

local row = ops.rawRow()
local action = ops.rawAction()

if action == "insert" or action == "update" then
	local id = row["user_id"]
	local status = row["status"]
	local key = "user_id_" .. id
	ops.SET(key, status)
elseif action == "delete" then 
	local id = row["user_id"]
	local key = "user_id_" .. id
	ops.DEL(key)
end
