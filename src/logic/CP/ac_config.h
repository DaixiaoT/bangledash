#ifndef __AC_CONFIG_H__
#define __AC_CONFIG_H__



#define DI_NUM_MAINBOARD	(76)
#define DO_NUM_MAINBOARD	(48)
#define	AI_NUM_MAINBOARD	(6)


#define DI_NUM				(DI_NUM_MAINBOARD)
#define DO_NUM				(DO_NUM_MAINBOARD)
#define	AI_NUM				(AI_NUM_MAINBOARD)

#define DI_BYTES			((DI_NUM+7)>>3)
#define DO_BYTES			((DO_NUM+7)>>3)

#endif // !__AC_CONFIG_H__
