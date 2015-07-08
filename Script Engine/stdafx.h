#ifndef __STDAFX_H__
#define __STDAFX_H__

#ifdef _MSC_VER
#pragma once
#endif

#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <Psapi.h>
#include <Shellapi.h>
#include "lua.hpp"

#include "UserAPI.h"
#include "CmdLineInterface.h"
#include "CVarSystem.h"
#include "RemoteProcess.h"
#include "Lua\LuaInterface.h"
#include "Lua\Callback.h"

#endif