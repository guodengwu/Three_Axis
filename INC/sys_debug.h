#ifndef _SYS_DEBUG_H
#define _SYS_DEBUG_H

#define	BSP_DEBUG_LEVEL		1
#define	SYS_DEBUG_LEVEL		1
#define	SYS_LINE_ENDING		"\n"

#if (SYS_DEBUG_LEVEL > 0U)
#define  SYS_PRINTF(...)   do { \
                            printf(__VA_ARGS__); \
                            printf(SYS_LINE_ENDING); \
} while (0)
#else
#define SYS_PRINTF(...) do {} while (0)
#endif
	
#if (BSP_DEBUG_LEVEL > 0U)
#define  BSP_PRINTF(...)   do { \
                            printf(__VA_ARGS__); \
                            printf(SYS_LINE_ENDING); \
} while (0)
#else
#define BSP_PRINTF(...) do {} while (0)
#endif
	
#endif
