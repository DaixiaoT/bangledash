/*-----------------------------------------------------------------------*//*!
     \file  sz20_PJ.H

*//*-------------------------------------------------------------------------------------------------------------------+
Revision History:

Ver: 1.00 Date:2020.10.19

+--------------------------------------------------------------------------------------------------------------------*/

#ifndef _LVF_PJ_H
#define _LVF_PJ_H

#include "LVF_TYPE.H"
#include "LVF_Modbus.h"
#include "LVF_HMI.h"

#ifndef EXT
#define EXT extern
#endif


#define HI(x) ((INT8U)((x >> 8) & 0x00FF))
#define LO(x) ((INT8U)(x & 0x00FF))

//!<For double word 
#define HH(x) ((INT8U)((x >> 24) & 0x000000FF))
#define HL(x) ((INT8U)((x >> 16) & 0x000000FF))
#define LH(x) ((INT8U)((x >> 8)  & 0x000000FF))
#define LL(x) ((INT8U)(x & 0x000000FF))

         

#endif//_LVF_PJ_H






