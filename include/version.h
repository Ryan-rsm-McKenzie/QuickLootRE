#ifndef	QKLT_VERSION_INCLUDED
#define QKLT_VERSION_INCLUDED

#define MAKE_STR_HELPER(x) #x
#define MAKE_STR(x) MAKE_STR_HELPER(x)

#define QKLT_VERSION_MAJOR		1
#define QKLT_VERSION_MINOR		17
#define QKLT_VERSION_PATCH		5
#define QKLT_VERSION_BETA		0
#define QKLT_VERSION_VERSTRING	MAKE_STR(QKLT_VERSION_MAJOR) "." MAKE_STR(QKLT_VERSION_MINOR) "." MAKE_STR(QKLT_VERSION_PATCH) "." MAKE_STR(QKLT_VERSION_BETA)

#endif
