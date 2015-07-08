-- file: example.lua
-- usage: test this script with a source engine game.

local firsttick = true
function MainCallbackFunction( )
	
	if( firsttick == true ) then
		print( "print from MainCallbackFunction" )
		firsttick = false
	end
end

if( Process.RegisterModule( "client.dll" ) == true and Process.RegisterModule( "engine.dll" ) == true ) then
	
	local ClientBase, ClientSize = Process.GetModuleData( "client.dll" )
	print( string.format( "found client.dll at 0x%X, size: 0x%X", ClientBase, ClientSize ) )
	
	local EngineBase, EngineSize = Process.GetModuleData( "engine.dll" )
	print( string.format( "found engine.dll at 0x%X, size: 0x%X", EngineBase, EngineSize ) )
	
	if( ClientBase ~= 0 and EngineBase ~= 0 ) then
		RegisterCallbackFunction( "MainCallbackFunction" )
	end
end