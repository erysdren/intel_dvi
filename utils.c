
#include <stdlib.h>
#include "utils.h"

bool string_endswith(const char *s, const char *e)
{
	size_t elen = SDL_strlen(e);
	size_t slen = SDL_strlen(s);
	if (elen > slen) return false;
	return SDL_strcasecmp(s + slen - elen, e) == 0 ? true : false;
}

[[ noreturn ]] void die(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, fmt, ap);
	va_end(ap);
	SDL_Quit();
	exit(0);
}
