# How2Start:
via command line: executablename -process "targetprocess.exe" -file "example.lua"


# API: CVarSystem
+ bool CVar.RegisterCVar( Name, value )
+ bool CVar.GetBool( Name )
+ float CVar.GetFloat( Name )
+ int CVar.GetInt( Name )
+ bool CVar.SetValue( Name, value as bool or number )


# API: File
+ bool File.GetBool( AppName, KeyName, File, optional default_value )
+ float File.GetFloat( AppName, KeyName, File, optional default_value )
+ int File.GetInt( AppName, KeyName, File, optional default_value )
+ bool File.WriteBool( AppName, KeyName, File, value )
+ bool File.WriteFloat( AppName, KeyName, File, value )
+ bool File.WriteInt( AppName, KeyName, File, value )


# API: RemoteProcess
- bool Process.RegisterModule( ModuleName )
+ int Process.FindSignature( ModuleName, Signature, ExtraOffset )
+ int, int Process.GetModuleData( ModuleName )
+ bool Process.SendMessage( Msg, WPARAM, LPARAM )
+ bool Process.ReadBool( Address )
+ int Process.ReadByte( Address )
+ float Process.ReadFloat( Address )
+ int Process.ReadInteger( Address )
+ bool Process.WriteBool( Address, value )
+ bool Process.WriteByte( Address, value )
+ bool Process.WriteFloat( Address, value )
+ bool Process.WriteInt( Address, value )
