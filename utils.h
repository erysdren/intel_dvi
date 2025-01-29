
#ifndef _UTILS_H_
#define _UTILS_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <SDL3/SDL.h>

#define log_warning(...) SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, __VA_ARGS__)
#define log_info(...) SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, __VA_ARGS__)

#define soft_assert(equ) do { if (!(equ)) die("Assertion failed: \"" #equ "\""); } while (0)

bool string_endswith(const char *s, const char *e);
[[ noreturn ]] void die(const char *fmt, ...);

#ifdef __cplusplus
}
#endif
#endif /* _UTILS_H_ */
