#ifndef _SYS_DEBUG_H
#define _SYS_DEBUG_H

#define	BSP_DEBUG_LEVEL		1
#define	SYS_DEBUG_LEVEL		1
#define	SYS_LINE_ENDING		"\n"

#if (SYS_DEBUG_LEVEL > 0U)

#define  SYS_PRINTF  printf   

#else

#define SYS_PRINTF //

#endif
	
#if (BSP_DEBUG_LEVEL > 0U)

#define  BSP_PRINTF  printf  

#else

#define BSP_PRINTF //

#endif
	
#endif
