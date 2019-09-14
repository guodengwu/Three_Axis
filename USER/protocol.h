#ifndef __PROTOCOL_H
#define __PROTOCOL_H	 

#include	"includes.h"

#define	 PRO_CMD_POSITION     6
#define	 PRO_LENGTH_SIZE     4                                               ///< ��Ϣ��Length��ռ�ֽ���
#define  PRO_START_LEN       5                                               ///< 4�ֽڳ��� + 1�ֽ�����
#define  PRO_EXTENT_LEN      4                                               ///< 1�ֽ����� + 2�ֽ�У��� + 1�ֽڽ�����
#define  PRO_END_LEN         3                                               ///< 2�ֽ�У��� + 1�ֽڽ�����

#define  PRO_RX_STATE_SD0              0           /* waiting for start first  start delimiter (SD0)  */
#define  PRO_RX_STATE_SD1              1           /* waiting for start second start delimiter (SD1)  */
#define  PRO_RX_STATE_CMD             2
#define  PRO_RX_STATE_LEN             3           /* waiting for len0  byte                       */
#define  PRO_RX_STATE_DATA             4           /* waiting for data                                */
#define  PRO_RX_STATE_CHKSUM          5           /* waiting for checksum0 low byte                  */
#define  PRO_RX_STATE_END              6           /* waiting for end delimiter                       */

#define  PRO_TX_STATE_SD0              0           /* Transmit state machine states                   */
#define  PRO_TX_STATE_SD1              1
#define  PRO_TX_STATE_LEN0             2
#define  PRO_TX_STATE_LEN1             3
#define  PRO_TX_STATE_LEN2             4
#define  PRO_TX_STATE_LEN3             5
#define  PRO_TX_STATE_DATA             6
#define  PRO_TX_STATE_CHKSUM0          7
#define  PRO_TX_STATE_CHKSUM1          8
#define  PRO_TX_STATE_END              9

#define	 _485_PROTOCOL_RX_SD0			0x81//��������
#define	 _485_PROTOCOL_RX_SD1			0x01//������ַ

typedef struct _usart {
    uint8_t               rx_state;
    uint16_t              rx_idx;
    uint16_t              rx_cnt;
    uint16_t              rx_len;
    uint16_t              rx_bufsize;
    uint16_t              rx_crc;
    uint16_t              rx_err;
	uint16_t              rx_cmd;
    uint8_t              *rx_buf;
	uint8_t								rx_flag;
    uint8_t (*rx_indicate)(struct _usart *pUsart, uint8_t rx_dat);

    uint8_t               tx_cmd;
    uint16_t              tx_idx;
    uint16_t              tx_len;
    uint16_t              tx_bufsize;
    uint16_t              tx_crc;
    uint16_t              tx_err;
    uint8_t              *tx_buf;
	uint8_t								tx_flag;
    void (*tx_complete)(struct _usart *pUsart);
} usart_t;

typedef enum { 
    _CMD_RX_GET_STATE		=	0X01,//��ѯϵͳ״̬
	_CMD_RX_SHIP			=	0X02,//����ָ��
	_CMD_RX_RESET			= 	0X03,//��λ
	_CMD_RX_GET_VERSION		= 	0X04,//��ȡ�汾
	_CMD_RX_CLR_RESULT		= 	0X05,//������н��
	_CMD_RX_SYS_TEST			=	0X06,//ϵͳ����
	_CMD_RX_SHIP_OK			=	0X08,//֪ͨ�������
	
	_CMD_TX_NONE			=		0X63,//�޻ظ�
	_CMD_TX_GET_STATE		=		0x64,//�ظ� _CMD_RX_GET_STATE
	_CMD_TX_SHIP			=		0X65,//�ظ� _CMD_RX_SHIP
	_CMD_TX_RESET			=		0x66,//�ظ� _CMD_RX_RESET
	_CMD_TX_GET_VERSION		=		0x67,//�ظ� _CMD_RX_GET_VERSION
	_CMD_TX_CLR_RESULT		=		0x68,//�ظ� _CMD_RX_CLR_RESULT
	_CMD_TX_SYS_TEST		=		0x69,//�ظ� _CMD_RX_SYS_TEST
	_CMD_TX_SHIP_OK			=		0X6b,//�ظ� _CMD_RX_SHIP_OK
	
	//_CMD_TX_GET_STATE			=	0X64,//��ѯϵͳ״̬
//    _CMD_EXECUTE_SYS_INFOR			=	0X02,//ִ��ϵͳ������Ϣ�޸�
//	_CMD_UPLOAD_DATA = 0X03,
//	_CMD_W_FLOW_FACTOR	= 0X04,
//	_CMD_SENSOR_PRESS_CALIBRATE = 0X06,// ѹ��������У׼
//	_CMD_ACK	= 0X0A,
//	_CMD_READ_SENSOR_DATA	= 0X0B,// ��ȡ���������ݼ�����
} EMessageCmd;

enum MSG_ERR {
    MSG_ERR_NONE              =  0x0000u, // ����Ϣ����
    MSG_ERR_SLAVE_NO_ACK      =  0xFFF8u, // �Ӱ�����Ӧ
	MSG_ERR_LENGTH      		 =  0xFFF9u, // ��Ϣ���ȴ������
	MSG_ERR_CHECKSUM          =  0xFFFAu, // ��ϢУ��ʹ������
	MSG_ERR_ETX_WORD          =  0xFFFBu, // ��Ϣ�������������
	MSG_ERR_UNDEFINITION      =  0xFFFCu, // ��Ϣ����δ����������
	MSG_ERR_IAP_MODE          =  0xFFFDu, // ��ǰ����IAPģʽ
	MSG_ERR_STATE             =  0xFFFEu, // ��Ϣδ����״̬
	MSG_ERR_TIMEOUT		     =  0xFFF7u, // ��Ϣ���ճ�ʱ
};

enum ACK_MSG	{
	ACK_ERR=0,
	ACK_OK=1,
	ACK_NONE=2,
	ACK_BUSY=3,
};
extern usart_t      usart;
void ProDataInit(void);
void UsartCmdReply(void);
void  UsartCmdProcess (void);
void RS485TxToRx(void);

#endif

