#ifndef _SYS_LOG_H_
#define _SYS_LOG_H_

#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbyte */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbyte */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbyte */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbyte */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbyte */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbyte */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbyte */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbyte */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base @ of Sector 8, 128 Kbyte */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base @ of Sector 9, 128 Kbyte */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbyte */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbyte */

static inline U16 get_sector(U32 Address)
{
	U16 sector = 0;

	if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
	{
		sector = FLASH_Sector_0;
	}
	else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
	{
		sector = FLASH_Sector_1;
	}
	else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
	{
		sector = FLASH_Sector_2;
	}
	else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
	{
		sector = FLASH_Sector_3;
	}
	else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
	{
		sector = FLASH_Sector_4;
	}
	else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
	{
		sector = FLASH_Sector_5;
	}
	else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
	{
		sector = FLASH_Sector_6;
	}
	else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
	{
		sector = FLASH_Sector_7;
	}
	else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
	{
		sector = FLASH_Sector_8;
	}
	else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
	{
		sector = FLASH_Sector_9;
	}
	else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
	{
		sector = FLASH_Sector_10;
	}
	else/*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_11))*/
	{
		sector = FLASH_Sector_11;
	}
	return sector;
}

static inline U16 next_sector(U16 sector)
{
    if(sector==FLASH_Sector_0)
        return FLASH_Sector_1;
    if(sector==FLASH_Sector_1)
        return FLASH_Sector_2;
    if(sector==FLASH_Sector_2)
        return FLASH_Sector_3;
    if(sector==FLASH_Sector_3)
        return FLASH_Sector_4;
    if(sector==FLASH_Sector_4)
        return FLASH_Sector_5;
    if(sector==FLASH_Sector_5)
        return FLASH_Sector_6;
    if(sector==FLASH_Sector_6)
        return FLASH_Sector_7;
    if(sector==FLASH_Sector_7)
        return FLASH_Sector_8;
    if(sector==FLASH_Sector_8)
        return FLASH_Sector_9;
    if(sector==FLASH_Sector_9)
        return FLASH_Sector_10;
    if(sector==FLASH_Sector_10)
        return FLASH_Sector_11;
    return 0xFFFF;
}

FLASH_Status EraseFlash(U16 sector1, U16 sector2);

#define E_PROG_FLASH   -2
#define E_EMPY_SYS_LOG -3
#define E_SYS_LOG_IDX  -4

// sys log api
void sys_log_init();
void sys_log_clear();

// return the total number of logs
int sys_log_num();

// 添加一条记录
// return 0, ok. other, error
int sys_log_add(U8 *buf);

// 从logIdx开始读logNum条记录到buf中
// logIdx: 从-1开始，到-log_num, -1是最近的一条记录
// logNum: 1~log_num，读取的条数，
// buf必须能放下全部要读的log数据
// return 0, ok. <0, error
int sys_log_read(int logIdx, int logNum, U8 *buf);

#endif //_SYS_LOG_H_
