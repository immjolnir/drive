#pragma once

#if defined(__GNUC__) || defined(__clang__) || defined(_MSC_VER)
#define ARROW_RESTRICT __restrict
#else
#define ARROW_RESTRICT
#endif

// ----------------------------------------------------------------------
// C++/CLI support macros (see ARROW-1134)

#ifndef NULLPTR

#ifdef __cplusplus_cli
#define NULLPTR __nullptr
#else
#define NULLPTR nullptr
#endif

#endif  // ifndef NULLPTR