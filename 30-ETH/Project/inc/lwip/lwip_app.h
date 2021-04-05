#ifndef _LWIP_APP_H
#define _LWIP_APP_H 

#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/tcp.h"
#include "lwip/udp.h"
#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "hard_bsp.h"
#include "stm32f2x7_eth_bsp.h"
#include "main.h"
   
typedef struct{
  uint32 sys_enet_init_state;//��̫����ʼ��״̬
  uint32 sys_phy_config_time;//��̫������ʱ��
  uint32 sys_check_link_time;//���������������ʱ��
  uint32 sys_MAC_change;     //MAC�Ƿ�ı�
  uint32 sys_IP_change;      //IP�Ƿ�ı�
  uint32 sys_ARPTimer;       //ARPʱ��
  uint32 sys_DHCPTimer;      //DHCPʱ��
  uint32 sys_DHCP_COARSE_Timer;//DHCP�ֲڴ���ʱ��
  uint32 sys_TCP_time;       //ѭ������TCP�ں�ʱ��
  uint32 sys_DHCP_OK;        //DHCP���
  uint32 sys_TCPclient_connect_OK;//TCP�ͻ������ӷ������ɹ�
}Struct_sys_change;


/*��̫���ϵ��ʼ������״̬  sys_enet_init_state  */  
#define   ETH_RETST           0    //��̫����λ״̬
#define   ETH_RETST_WAIT      1    //��̫����λ�쳣״̬
#define   ETH_CONFIG          2    //��̫��ͨ��ģʽ����
#define   ETH_PHY_RST         3    //��̫��PHY��λ״̬
#define   ETH_PHY_LINK        4    //��̫��PHY_LINK״̬
#define   ETH_PHY_AUTO        5    //��̫��PHY_�Զ�Э��
#define   ETH_PHY_SET         6    //��̫��PHY����״̬
#define   ETH_LWIP_INIT       7    //��̫��LWIP��ʼ��״̬
#define   ETH_INIT_OK         8    //��̫����ʼ�����״̬

/*��̫������ʱ��sys_phy_config_time*/
#define  PHY_CONFIG_SEC  20        //��̫������ʱ��20*10ms

/*���������������ʱ�� sys_check_link_time*/
#define  ETH_CHECK_SEC   100       //��̫������ʱ��100*10ms

/*ARPʱ��  sys_ARPTimer*/
#define  ARP_TIME       5*100//5s

/*DHCPʱ��  sys_DHCPTimer*/
#define DHCP_FINE_TIME  50 

/*DHCP�ֲڴ���ʱ��  sys_DHCP_COARSE_Timer*/
#define DHCP_COARSE_TIME    60*100 

/*TCPʱ��  sys_TCP_time*/
#define TCP_TMR      25

#define MAX_DHCP_TRIES        4

void LwIP_Init(void);

#endif













