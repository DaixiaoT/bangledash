#ifndef __LLDP_H__
#define __LLDP_H__

#define LLDP_USE_VERIFY	1

//LLDP�������ݳ��ȳ���
#define LLDP_RET_ERR_DATA_LEN    -1
//LLDP�������ݳ�ʱ
#define LLDP_RET_ERR_TIMEOUT     -2
//LLDP��������û�б仯
#define LLDP_RET_NOT_CHANGE      -3

#define LLDP_DATA_DEFAULT_TIMEOUT_S 5 //LLDP����Ĭ�ϳ�ʱʱ�䣬��λ��

#pragma  pack (push,1)
#define GATEWAY_IP_MAX 9
#define SED_IP_MAX 60
typedef struct
{
	int len;//�ַ����ܳ��� 
	int gateway_num;//���ص�ַ����k
	U32 gateway_ip[GATEWAY_IP_MAX];//���ص�ַ
	U32 sed_ip_seg;//�ն����ε�ַ
	int sed_ip_num;//�ն� IP ��ַ���� u
	U16 sed_ip_tail[SED_IP_MAX];//�ն� IP ��ַ
	U8  car_no;//�������� 
	U16 train_no;//�г�����
	U8  line_no;//��·����
	U8   metro_corp_no;//������˾����
	U32 server_ip;//���ܷ����� IP��ַ
	U16 server_port;//���ܷ���˿ں�
	U16 server_safe_port;//���ܰ�ȫ������Ʒ���˿ں�
	U32 net_mask;//������������ 
}TLV_SIFANG_CUSTOM;

typedef struct
{
	U8 m_tlv_chassis_mac[6];//���� mac ��ַ
	U8 m_tlv_port_mac[6];//�ӿ� mac ��ַ
	U16 m_tlv_ttl;//Time To Live 
	U8 m_tlv_vlan_id;//vlan����
	U8 m_tlv_port_id;//�ӿڱ���
	TLV_SIFANG_CUSTOM m_tlv_sf;
	U16 crc16;
}TLV_LLDP_APP;
#pragma  pack (pop)

//lldp��ʼ��
void lldp_init();

// ���������������
void lldp_update_data(U8* data,int len);

//����LLLDP����
BOOL lldp_recv(TLV_LLDP_APP *tlv);
//���㱾��IP��nĬ��ȡLLLDP�������1��IP��ַ��Ŀǰ������ֻ��һ��IP��nĬ����0
U32 calc_loack_ip(TLV_LLDP_APP *tlv,int n=0);
//��������LLDP����
void lldp_reset_data();
#endif//__LLDP_H__


