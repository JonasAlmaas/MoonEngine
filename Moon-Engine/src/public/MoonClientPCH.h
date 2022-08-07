#pragma once

/**
 * This is not a pch file!
 * You can include this into your own pch file for your project (if you want things to "just work").
 * Do NOT include this anywhere other than a pch file!
 * Make sure to include this before the engine itself.
 *
 * pch example:
 *  #include <MoonClientPCH.h>
 *  #include <Moon.h>
 */

#include <algorithm>
#include <array>
#include <chrono>
#include <codecvt>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>
#include <random>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include <xhash>
#include <xstring>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>
#include "glm/gtx/string_cast.hpp"

#include <imgui.h>
#include <imgui_internal.h>
