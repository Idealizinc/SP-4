function SaveToLuaFile(outputString, overwrite)
   print("SaveToLuaFile...")
   local f;						-- The file
   if overwrite == 1 then		-- Wipe the contents with new data
      f = assert(io.open("Scripts/Datastore.lua", "w"))
   elseif overwrite == 0 then	-- Append with new data
      f = assert(io.open("Scripts/Datastore.lua", "a"))
   end
   -- Write to the file
   f:write(outputString)
   -- Close the file
   f:close()
   print("Value Saved")
end