#if !defined (MVB_LP_H)
#define MVB_LP_H

#define SEND_WAIT_CYCLE 1500
#define REV_WAIT_CYCLE 1500

#define MUE_TRUE 0
#define MUE_FALSE 1


#if !defined (NULL)
   #define NULL   0
#endif

#if !defined (TRUE)
   #define TRUE   1==1
#endif

#if !defined (FALSE)
   #define FALSE  1!=1
#endif

#define MVB_TS_ID			0

#define LP_OK					0
#define LP_PRT_PASSIVE		1
#define LP_ERROR				2
#define LP_CONFIG				3
#define LP_MEMORY			4
#define LP_UNKNOWN_TS		5
#define LP_RANGE				6
#define LP_DATA_TYPE			7

#define PORT_SIZE_1		0
#define PORT_SIZE_2		1
#define PORT_SIZE_4		2
#define PORT_SIZE_8	   3
#define PORT_SIZE_16    4

#define PORT_SINK			0
#define PORT_SOURCE		1

#define LP_CMD_PUT_DATASET             1
#define LP_CMD_GET_DATASET             2
#define LP_CMD_PUT_MASKED_DATASET      11

#define LP_CMD_FORCE_DATASET           21
#define LP_CMD_UNFORCE_DATASET         22
#define LP_CMD_UNFORCE_ALL_DATASET     23

#define LP_CMD_PUT_DATASET_MF          31
#define LP_CMD_GET_DATASET_MF          32
#define LP_CMD_PUT_MASKED_DATASET_MF   33


typedef unsigned short    BITFIELD;
typedef unsigned char   BOOLEAN1;
typedef unsigned char   ANTIVALENT2;
typedef unsigned char   BCD4;
typedef unsigned char   ENUM4;
typedef unsigned char   BITSET8;
typedef unsigned char   ENUM8;
typedef unsigned char   UNSIGNED8;
typedef char            INTEGER8;
typedef char            CHARACTER8;
typedef unsigned short  BITSET16;
typedef unsigned short  ENUM16;
typedef unsigned short  UNSIGNED16;
typedef short           INTEGER16;
typedef float           REAL32;
typedef unsigned long   BITSET32;
typedef unsigned long   UNSIGNED32;
typedef long            INTEGER32;
typedef unsigned long   BITSET64;
typedef unsigned long   UNSIGNED64;
typedef long            INTEGER64;
typedef struct
{
   UNSIGNED32  seconds;
   UNSIGNED16  ticks;
}  TIMEDATE48;


typedef UNSIGNED16   LP_RESULT;

typedef UNSIGNED16   TS_PORT_SIZE;

typedef UNSIGNED16   TS_PORT_TYPE;

typedef UNSIGNED8    LP_CMD;


typedef struct
{
	BITFIELD port_address   : 12;
	BITFIELD port_fcode    	  : 3;
	BITFIELD port_type		: 1;
}  PORT_CONFIG;

typedef struct
{
	PORT_CONFIG	*	p_ts_port_config;
	UNSIGNED16			c_ts_port_config;
	UNSIGNED16        supervision_interval;
}  TS_CONFIG;

typedef struct
{
   BITFIELD port_address	: 12;
   BITFIELD ts_id          : 4;

}  DS_NAME;


#if defined(__cplusplus)
extern "C"
{
#endif

void 
mue_pd_put_port_config(UNSIGNED8	port_handle,BITSET16	port_config);
void
mue_sv_put_device_config( BITSET16	device_config);
void 
mue_sv_get_device_config(BITSET16   *device_config);
void 
mue_pd_put_port_data( UNSIGNED8	port_handle, UNSIGNED16	*p_port_data);



UNSIGNED8 mue_pd_get_port_data(UNSIGNED8 	port_handle,UNSIGNED16	*p_port_data,	UNSIGNED16	 *fresh_time);

void  ba_send_mf(UNSIGNED16  *master_frames_list);

LP_RESULT  
	lp_init  (ENUM8 ts_id,  void *p_descriptor  );

LP_RESULT
	lp_put_dataset (DS_NAME * dataset, void * p_value);

//LP_RESULT
LP_CMD
	lp_get_dataset (DS_NAME * dataset, void * p_value, void * p_fresh);

LP_RESULT	lp_get_sn(UNSIGNED8 * sn);


LP_RESULT
	lp_cmd_dataset (LP_CMD command, DS_NAME * dataset, void * p_value, void * p_mask, void * p_fresh);
void  
	device_addr_set_start(UNSIGNED16 device_addr);


LP_RESULT lp_set_mon(BOOLEAN1 is_mon);
int lp_mon(UNSIGNED8* buf,int len);

#if defined(__cplusplus)
}
#endif

#endif

