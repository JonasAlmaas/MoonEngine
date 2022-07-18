#pragma once

#include <algorithm>
#include <array>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <memory>
#include <random>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include <xhash>

#include <entt.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

#include <imgui.h>
#include <imgui_internal.h>

#include "Moon/Core/Base.h"

#include "Moon/Debug/Instrumentor.h"
#include "Moon/Core/Util/Math/Math.h"
#include "Moon/Core/Util/Timer.h"
#include "Moon/Core/Type/Types.h"

#include "Moon/Core/Event/Event.h"
#include "Moon/Core/Event/ApplicationEvent.h"
#include "Moon/Core/Event/KeyEvent.h"
#include "Moon/Core/Event/MouseEvent.h"
#include "Moon/Core/Event/WindowEvent.h"

#include "Moon/Core/Input/Code/KeyCode.h"
#include "Moon/Core/Input/Code/MouseCode.h"
