cmake_minimum_required (VERSION 3.8)

# Включение горячей перезагрузки для компиляторов MSVC, если поддерживается.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("LogBaseNumeration")
# add_compile_options(/constexpr:steps10000000)
add_executable (LogBaseNumeration "include/lns.h" "main.cpp" "include/bitsetExtended.h" "include/constexprMath.h" "include/fixedPoint.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET LogBaseNumeration PROPERTY CXX_STANDARD 23)
endif()