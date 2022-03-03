// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H
#include <vector>
#include <string>
#include <string_view>
#include <regex>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <ios>
#include <codecvt>
#include <sstream>
#include <memory>
#include <set>
#include <unordered_set>
#include <map>
#include <functional>
#define _HAS_CXX20	1
#define _HAS_CXX17	1
#include <chrono>
// #include <date/date.h>
// #include <date/tz.h>
#include <filesystem>
#include <numeric>
#include <algorithm>
// new 5 includes
#include <tuple>
#include <future>
#include <thread>
#include <exception>
#include <stdexcept>
//#include "Utilities.h"
//#include <JDGridCtrl.h>
#include <..\FixedPoint/arithmetic_types.h>
#include <any>
#include <variant>
#include <optional>
#include <sqlite_orm/sqlite_orm.h>

// add headers that you want to pre-compile here
#include "framework.h"

#include  "..\DLLs.h"
#include  "..\JDGridCtrl/JDGridCtrl.h"
#include "..\ORM_Extensions/HandleSystemError.h"


#endif //PCH_H
