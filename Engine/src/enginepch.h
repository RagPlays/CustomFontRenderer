#ifndef ENGINEPCH_H
#define ENGINEPCH_H

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#ifdef ENGINE_PLATFORM_WINDOWS
	#include <Windows.h>
#endif

#include "Engine/Core/EngineDefines.h"

#include "Engine/Core/Structs.h"
#include "Engine/Core/Assert.h"
#include "Engine/Core/Log.h"
#include "Engine/Core/Base.h"

#include "Engine/Debug/Instrumentor.h"

#endif // !ENGINEPCH_H