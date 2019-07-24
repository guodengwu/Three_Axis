#include "protocol.h"
#include "misc.h"
#include "motor.h"

usart_t      usart;
#define USART_TXBUFF_SIZE		20
#define USART_RXBUFF_SIZE		20

//usart_t      usart;
//_ACKData_t ack_data;
//===================================================================================================
static uint8_t       usart_rx_buf        [USART_RXBUFF_SIZE];
static uint8_t       usart_tx_buf        [USART_TXBUFF_SIZE];
static uint8_t       data_buf[20];

static uint8_t  UsartRxGetINT8U (uint8_t *buf,uint32_t *idx);
static uint16_t  UsartRxGetINT16U (uint8_t *buf,uint32_t *idx);
static uint32_t  UsartRxGetINT32U (uint8_t *buf,uint32_t *idx);
static uint8_t uart_message_rx_handler(usart_t *pUsart, uint8_t rx_dat);
static void uart_message_tx_handler(usart_t *pUsart);
static void PackageSendData(uint8_t cmd, uint8_t *pdat, uint8_t len);

void ProDataInit(void)
{
	usart.rx_state     = PRO_RX_STATE_SD0;
    usart.rx_idx       = 0;
    usart.rx_cnt       = 0;
    usart.rx_len       = 0;
    usart.rx_bufsize      = USART_RXBUFF_SIZE;
    usart.rx_crc       = 0;
    usart.rx_err       = MSG_ERR_NONE;
    usart.rx_buf       = usart_rx_buf;
	usart.rx_flag			= DEF_No;
	usart.rx_indicate  = &uart_message_rx_handler;
	
    usart.tx_cmd     = _CMD_TX_NONE;
    usart.tx_idx       = 0;
    usart.tx_len       = 0;
    usart.tx_bufsize      = USART_TXBUFF_SIZE;
    usart.tx_crc       = 0;
    usart.tx_err       = MSG_ERR_NONE;
    usart.tx_buf       = usart_tx_buf;
	usart.tx_flag 			= DEF_Idle;   
    usart.tx_complete  = &uart_message_tx_handler;
}

//����ָ��ظ�
void UsartCmdReply(void)
{
	uint8_t idx=0,cmd;
	usart_t *pUsart = &usart;
	
	if(pUsart->tx_flag == DEF_Busy || pUsart->tx_cmd == _CMD_TX_NONE)	{//����æ �ȴ�����
		return;
	}
	cmd = pUsart->tx_cmd;
	switch(cmd)	{
		case _CMD_TX_GET_STATE://0x64,//�ظ� _CMD_RX_GET_STATE
			data_buf[idx++] = IOState.state1.ubyte;//����״̬
			data_buf[idx++] = IOState.state2.ubyte;
		    data_buf[idx++] = SysMotor.ALLMotorState.ubyte;//���е��״̬
			data_buf[idx++] = 0;
			data_buf[idx++] = IOState.HongWaiState.ubyte;
			data_buf[idx++] = DevState.ubyte;
			data_buf[idx++] = SysHDError.E1.ubyte;
			data_buf[idx++] = SysHDError.E2.ubyte;
			data_buf[idx++] = SysLogicErr.logic;
			data_buf[idx++] = SysMotor.motor[MOTOR_X].CurPos>>8;
			data_buf[idx++] = SysMotor.motor[MOTOR_X].CurPos&0xff;
			data_buf[idx++] = SysMotor.motor[MOTOR_Y].CurPos>>8;
			data_buf[idx++] = SysMotor.motor[MOTOR_Y].CurPos&0xff;
			data_buf[idx++] = Sys.DevAction;
			PackageSendData(cmd, data_buf, idx);
			break;
		case _CMD_TX_SHIP://0X65,//�ظ� _CMD_RX_SHIP
			if(Sys.DevAction == ActionState_Doing)	{
				data_buf[idx++] = 0;
			}else	{
				Sys.DevAction = ActionState_DoReady;
				data_buf[idx++] = 1;
			}
			PackageSendData(cmd, data_buf, idx);
			break;
		case _CMD_TX_RESET://0x66,//�ظ� _CMD_RX_RESET
			if(Sys.DevAction == ActionState_Doing)	{
				data_buf[idx++] = 0;
			}
			else	{
				data_buf[idx++] = 1;
			}
			PackageSendData(cmd, data_buf, idx);			
			break;
		case _CMD_TX_GET_VERSION://0x67,//�ظ� _CMD_RX_GET_VERSION
			data_buf[idx++] = 1;
			strcpy(data_buf+idx, VERSION);
			PackageSendData(cmd, data_buf, strlen(VERSION)+idx); 
			break;
		case _CMD_TX_CLR_RESULT://0x68,//�ظ� _CMD_RX_CLR_RESULT
			//data_buf[idx++] = Sys.DevAction;
			break;
		default:
			break;
	}
}

//����ָ�����
void  UsartCmdProcess (void)
{
    uint8_t cmd,iPara;
	usart_t *pUsart = &usart;
	u32 temp;

	if(pUsart->rx_flag==DEF_No)	{//�����ݽ��� ����
		return;
	}
    
    if (pUsart->rx_err == MSG_ERR_NONE) {//���ݽ����޴���
		cmd	= pUsart->rx_cmd;
		switch(cmd)	{
			case _CMD_RX_GET_STATE://0X01,//��ѯϵͳ״̬
				pUsart->tx_cmd = _CMD_TX_GET_STATE;
				break;
			case _CMD_RX_SHIP:	//0X02,//����ָ��			
				pUsart->tx_cmd = _CMD_TX_SHIP;
				temp = pUsart->rx_idx+6;
				SysMotor.motor[MOTOR_X].ObjPos = UsartRxGetINT16U(pUsart->rx_buf,&temp);
				SysMotor.motor[MOTOR_Y].ObjPos = UsartRxGetINT16U(pUsart->rx_buf,&temp);
				break;
			case _CMD_RX_RESET://0X03,//��λ
				iPara = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx); 
				if(iPara==2)	{
					pUsart->tx_cmd = _CMD_TX_RESET;
					DevState.bits.State = DEV_STATE_RESET;//��λ��
					if(Sys.DevAction != ActionState_Doing)	{
						DevState.bits.SubState = 4;
					}
				}
				break;
			case _CMD_RX_GET_VERSION://0X04,//��ȡ�汾
				iPara = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx); 
				if(iPara==2)	{
					pUsart->tx_cmd = _CMD_TX_GET_VERSION;
				}
				break;
			case _CMD_RX_CLR_RESULT://	0X05,//������н��
				iPara = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx); 
				if(iPara==2)	{
					pUsart->tx_cmd = _CMD_TX_CLR_RESULT;
				}
				break;
			case _CMD_RX_SYS_TEST://0X06,//ϵͳ����
				iPara = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx); 
				if(iPara==1)	{
					DevState.bits.State = DEV_STATE_TEST;//�豸״̬ ������������
					DevState.bits.SubState = 1;//���������
					iPara = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx); 
					if(iPara==3)	{
						SysMotor.ALLMotorState.bits.b0 = DEF_Busy;
						SysMotor.motor[MOTOR_X].ObjPos = UsartRxGetINT16U(pUsart->rx_buf,&pUsart->rx_idx);
					}else if(iPara==4)	{
						SysMotor.ALLMotorState.bits.b1 = DEF_Busy;
						SysMotor.motor[MOTOR_Y].ObjPos = UsartRxGetINT16U(pUsart->rx_buf,&pUsart->rx_idx);
					}else if(iPara==5)	{
						SysMotor.ALLMotorState.bits.b3 = DEF_Busy;
						SysMotor.motor[MOTOR_TuiGan].Param = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx);
					}else if(iPara==6)	{
						SysMotor.ALLMotorState.bits.b4 = DEF_Busy;
						SysMotor.motor[MOTOR_CeMen].Param = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx);
					}else if(iPara==7)	{
						SysMotor.ALLMotorState.bits.b5 = DEF_Busy;
						SysMotor.motor[MOTOR_QuHuoKou].Param = UsartRxGetINT16U(pUsart->rx_buf,&pUsart->rx_idx);
					}else if(iPara==8)	{
						SysMotor.ALLMotorState.bits.b6 = DEF_Busy;
						SysMotor.motor[MOTOR_QuHuoMen].Param = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx);
					}
				}
				break;
			default:
				pUsart->tx_cmd = _CMD_TX_NONE;
		}		
	}else {//���ݽ����쳣
        pUsart->rx_err = MSG_ERR_NONE;        // clear rx error
    }		
	pUsart->rx_flag = DEF_No;//�������ɱ�־
}

static uint8_t uart_message_rx_handler(usart_t *pUsart, uint8_t rx_dat)
{
    switch (pUsart->rx_state) {
        case PRO_RX_STATE_SD0:                     /* waiting for start first  start delimiter (SD0)  */
            if (rx_dat == _485_PROTOCOL_RX_SD0) {//��������
                pUsart->rx_state = PRO_RX_STATE_SD1;
                pUsart->rx_crc	 = rx_dat;
                pUsart->rx_idx   = 0;
                pUsart->rx_cnt   = 0;
            }
            break;

        case PRO_RX_STATE_SD1:                     /* waiting for start second start delimiter (SD1)  */
            if (rx_dat == _485_PROTOCOL_RX_SD1) {//������ַ
                pUsart->rx_state = PRO_RX_STATE_CMD;
                pUsart->rx_crc	 ^= rx_dat;
            } else {
                pUsart->rx_state = PRO_RX_STATE_SD0;
            }
            break;
        case PRO_RX_STATE_CMD:                    /* waiting for 'len' HIGH byte                      */
            pUsart->rx_cmd       = rx_dat;
            pUsart->rx_crc       ^= rx_dat;
            pUsart->rx_state     = PRO_RX_STATE_LEN;
            break;
        case PRO_RX_STATE_LEN:                    /* waiting for 'len' LOW byte                     */
            pUsart->rx_len      = rx_dat&0XFF;
            if (/*(pUsart->rx_len < PRO_EXTENT_LEN) || */(pUsart->rx_len > pUsart->rx_bufsize)) {
                pUsart->rx_state = PRO_RX_STATE_SD0;/* Can not handle this size ...                    */
                pUsart->rx_err   = MSG_ERR_LENGTH;
				pUsart->rx_flag		 = DEF_Yes;
            } else {
                pUsart->rx_crc  ^= rx_dat;
                pUsart->rx_state = PRO_RX_STATE_DATA;
            }
            break;
        case PRO_RX_STATE_DATA:                    /* waiting for data                                */
            if (pUsart->rx_cnt < pUsart->rx_bufsize) {
                pUsart->rx_buf[pUsart->rx_cnt++] = rx_dat;
            }
            pUsart->rx_crc ^= rx_dat;
            if ((pUsart->rx_len /*- SENSOR_END_LEN*/) <= pUsart->rx_cnt) {
                pUsart->rx_state = PRO_RX_STATE_CHKSUM;
            }
            break;

        case PRO_RX_STATE_CHKSUM:                 /* waiting for checksum HIGH byte                   */
            if (pUsart->rx_crc == rx_dat) {
                pUsart->rx_flag		 = DEF_Yes;
				pUsart->rx_state = PRO_RX_STATE_SD0;
            } else {
                pUsart->rx_state = PRO_RX_STATE_SD0;
                pUsart->rx_err   = MSG_ERR_CHECKSUM;
				pUsart->rx_flag		 = DEF_Yes;
            }
            break;

        default:
            pUsart->rx_state = PRO_RX_STATE_SD0;
            pUsart->rx_err   = MSG_ERR_STATE;
			pUsart->rx_flag	 = DEF_Yes;
            break;
    }
	return 1;
}

static void PackageSendData(uint8_t cmd, uint8_t *pdat, uint8_t len)
{
	uint8_t idx=0;
	uint8_t temp;

	usart.tx_flag = DEF_Busy;//���ڷ���æ���
	usart.tx_buf[idx++] = _485_PROTOCOL_RX_SD0;
	usart.tx_buf[idx++] = _485_PROTOCOL_RX_SD1;
	usart.tx_buf[idx++] = cmd;
	usart.tx_buf[idx++] = len + PRO_EXTENT_LEN;
	if (len) {
        memcpy(&usart.tx_buf[idx], pdat, len);
    }
	idx += len;
	temp = CRC8_XOR(usart.tx_buf, idx);
	usart.tx_buf[idx++] = temp;
	usart.tx_len = idx;
	usart.tx_idx = 0;
	usart4_tx_int_enable();
}

static void uart_message_tx_handler(usart_t *pUsart)
{
	//INT8U  tx_dat;	
	
	if(pUsart->tx_idx<=pUsart->tx_len)	{
		//tx_dat = usart.tx_buf[usart.tx_idx++];
		UART4_SendByte(pUsart->tx_buf[pUsart->tx_idx++]);
	}else {
		if(pUsart->tx_cmd == _CMD_TX_RESET)	{//ָ�����ɺ� �ٸ�λ
			Sys.state |= SYSSTATE_RESET;
		}
		pUsart->tx_cmd = _CMD_TX_NONE;
		pUsart->tx_flag = DEF_Idle;//�������
	}
}

static uint8_t  UsartRxGetINT8U (uint8_t *buf,uint32_t *idx)
{
    return (buf[(*idx)++]);
}

static uint16_t  UsartRxGetINT16U (uint8_t *buf,uint32_t *idx)
{
    uint16_t  lowbyte;
    uint16_t  highbyte;

    lowbyte  = UsartRxGetINT8U(buf,idx);
    highbyte = UsartRxGetINT8U(buf,idx);
    return ((highbyte << 8) | lowbyte);
}

static uint32_t  UsartRxGetINT32U (uint8_t *buf,uint32_t *idx)
{
    uint32_t  highword;
    uint32_t  lowword;

    lowword = UsartRxGetINT16U(buf,idx);
    highword = UsartRxGetINT16U(buf,idx);
    return ((highword << 16) | lowword);
}