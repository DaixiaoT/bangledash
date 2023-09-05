#ifndef __LLDP_H__
#define __LLDP_H__

#define LLDP_USE_VERIFY	1

//LLDP接收数据长度出错
#define LLDP_RET_ERR_DATA_LEN    -1
//LLDP接收数据超时
#define LLDP_RET_ERR_TIMEOUT     -2
//LLDP接收数据没有变化
#define LLDP_RET_NOT_CHANGE      -3

#define LLDP_DATA_DEFAULT_TIMEOUT_S 5 //LLDP接收默认超时时间，单位秒

#pragma  pack (push,1)
#define GATEWAY_IP_MAX 9
#define SED_IP_MAX 60
typedef struct
{
	int len;//字符串总长度 
	int gateway_num;//网关地址数量k
	U32 gateway_ip[GATEWAY_IP_MAX];//网关地址
	U32 sed_ip_seg;//终端网段地址
	int sed_ip_num;//终端 IP 地址数量 u
	U16 sed_ip_tail[SED_IP_MAX];//终端 IP 地址
	U8  car_no;//车辆编码 
	U16 train_no;//列车编码
	U8  line_no;//线路编码
	U8   metro_corp_no;//地铁公司编码
	U32 server_ip;//网管服务器 IP地址
	U16 server_port;//网管服务端口号
	U16 server_safe_port;//网管安全接入控制服务端口号
	U32 net_mask;//本地子网掩码 
}TLV_SIFANG_CUSTOM;

typedef struct
{
	U8 m_tlv_chassis_mac[6];//管理 mac 地址
	U8 m_tlv_port_mac[6];//接口 mac 地址
	U16 m_tlv_ttl;//Time To Live 
	U8 m_tlv_vlan_id;//vlan编码
	U8 m_tlv_port_id;//接口编码
	TLV_SIFANG_CUSTOM m_tlv_sf;
	U16 crc16;
}TLV_LLDP_APP;
#pragma  pack (pop)

//lldp初始化
void lldp_init();

// 从驱动层更新数据
void lldp_update_data(U8* data,int len);

//处理LLLDP数据
BOOL lldp_recv(TLV_LLDP_APP *tlv);
//计算本地IP，n默认取LLLDP数据里第1个IP地址，目前数据里只有一个IP，n默认是0
U32 calc_loack_ip(TLV_LLDP_APP *tlv,int n=0);
//重新设置LLDP数据
void lldp_reset_data();
#endif//__LLDP_H__


