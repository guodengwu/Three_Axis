#include "protocol.h"
#include "misc.h"
#include "motor.h"

usart_t      usart;
#define USART_TXBUFF_SIZE		30
#define USART_RXBUFF_SIZE		30

//===================================================================================================
uint8_t   xdata    usart_rx_buf        [USART_RXBUFF_SIZE];
uint8_t   xdata    usart_tx_buf        [USART_TXBUFF_SIZE];
static uint8_t   xdata    data_buf[20];

static uint8_t  UsartRxGetINT8U (uint8_t *buf,uint16_t *idx);
static uint16_t  UsartRxGetINT16U (uint8_t *buf,uint16_t *idx);
static uint32_t  UsartRxGetINT32U (uint8_t *buf,uint16_t *idx);
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

void RS485TxToRx(void)
{
	static u8 txflag;
	if(usart.tx_flag == DEF_Idle)	{
		txflag ++;
		if(txflag>1)	{
			txflag = 0;
			RS485_CTRL = 0;
		}
	}
	else
		txflag = 0;
}
//串口指令回复
void UsartCmdReply(void)
{
	uint8_t idx=0,cmd;
	usart_t *pUsart = &usart;
	
	if(pUsart->tx_flag == DEF_Busy || pUsart->tx_cmd == _CMD_TX_NONE)	{//发送忙 等待空闲
		return;
	}
	cmd = pUsart->tx_cmd;
	switch(cmd)	{
		case _CMD_TX_GET_STATE://0x64,//回复 _CMD_RX_GET_STATE
			data_buf[idx++] = IOState.state1.ubyte;//开关状态
			data_buf[idx++] = IOState.state2.ubyte;
		    data_buf[idx++] = SysMotor.ALLMotorState.ubyte;//所有电机状态
			data_buf[idx++] = 0;
			data_buf[idx++] = IOState.HongWaiState.ubyte;
			data_buf[idx++] = DevState.ubyte;//机器状态
			data_buf[idx++] = SysHDError.E1.ubyte;
			data_buf[idx++] = SysHDError.E2.ubyte;
			data_buf[idx++] = SysLogicErr.logic;
			data_buf[idx++] = SysMotor.motor[MOTOR_X_ID].CurPos>>8;
			data_buf[idx++] = SysMotor.motor[MOTOR_X_ID].CurPos&0xff;
			data_buf[idx++] = SysMotor.motor[MOTOR_Y_ID].CurPos>>8;
			data_buf[idx++] = SysMotor.motor[MOTOR_Y_ID].CurPos&0xff;
			data_buf[idx++] = Sys.DevAction;//动作结果
			PackageSendData(cmd, data_buf, idx);
			break;
		case _CMD_TX_SHIP://0X65,//回复 _CMD_RX_SHIP
			PackageSendData(cmd, data_buf, pUsart->tx_idx);
			break;
		case _CMD_TX_RESET://0x66,//回复 _CMD_RX_RESET
			PackageSendData(cmd, data_buf, pUsart->tx_idx);			
			break;
		case _CMD_TX_GET_VERSION://0x67,//回复 _CMD_RX_GET_VERSION
			data_buf[idx++] = 1;
			strcpy(data_buf+idx, VERSION);
			PackageSendData(cmd, data_buf, strlen(VERSION)+idx); 
			break;
		case _CMD_TX_CLR_RESULT://0x68,//回复 _CMD_RX_CLR_RESULT 机器复位
			PackageSendData(cmd, data_buf, pUsart->tx_idx);
			break;
		case _CMD_TX_SYS_TEST://0x69,//回复 _CMD_RX_SYS_TEST
			PackageSendData(cmd, data_buf, pUsart->tx_idx);
			break;
		case _CMD_TX_SHIP_OK://0X6b,//回复 _CMD_RX_SHIP_OK
			PackageSendData(cmd, data_buf, 0);
			SysMotor.ALLMotorState.bits.ZMotor = DEF_Run;
			ZMotorStart();
			break;
		default:
			break;
	}
}
extern RINGBUFF_T uart4_rxring;
//串口指令处理函数
void  UsartCmdProcess (void)
{
    uint8_t cmd,iPara,rxdat;
	usart_t *pUsart = &usart;
	u16 temp;

	if(RingBuffer_Pop(&uart4_rxring, (void *)&rxdat) == 0)//无数据
		return;
	uart_message_rx_handler(&usart, rxdat);
	if(pUsart->rx_flag==DEF_No)	{//无数据接收 返回
		return;
	}

    if (pUsart->rx_err == MSG_ERR_NONE) {//数据解析无错误
		cmd	= pUsart->rx_cmd;
		switch(cmd)	{
			case _CMD_RX_GET_STATE://0X01,//查询系统状态
				pUsart->tx_cmd = _CMD_TX_GET_STATE;
				break;
			case _CMD_RX_SHIP:	//0X02,//出货指令			
				pUsart->tx_cmd = _CMD_TX_SHIP;
				pUsart->tx_idx = 0;
				temp = pUsart->rx_idx+6;
				if(Sys.DevAction == ActionState_Doing)	{
					data_buf[pUsart->tx_idx++] = 0;
				}else	{
					data_buf[pUsart->tx_idx++] = 1;				
					SysMotor.motor[MOTOR_X_ID].ObjPos = (INT32S)UsartRxGetINT16U(pUsart->rx_buf,&temp);
					SysMotor.motor[MOTOR_Y_ID].ObjPos = (INT32S)UsartRxGetINT16U(pUsart->rx_buf,&temp);
					DevState.bits.State = DEV_STATE_SHIPING;
					DevState.bits.SubState = DEV_ShipStateMotorUp;//升降机上升
					SysMotor.ALLMotorState.bits.XMotor = DEF_Run;
					SysMotor.ALLMotorState.bits.YMotor = DEF_Run;
					XMotorStart();
					YMotorStart();
				}
				break;
			case _CMD_RX_RESET://0X03,//重启
				iPara = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx); 
				if(iPara==2)	{
					pUsart->tx_cmd = _CMD_TX_RESET;
					pUsart->tx_idx = 0;
					if(Sys.DevAction == ActionState_Doing)	{
						data_buf[pUsart->tx_idx++] = 0;
					}else	{//1：开始重启
						data_buf[pUsart->tx_idx++] = 1;				
					}
				}
				break;
			case _CMD_RX_GET_VERSION://0X04,//获取版本
				iPara = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx); 
				if(iPara==2)	{
					pUsart->tx_cmd = _CMD_TX_GET_VERSION;
				}
				break;
			case _CMD_RX_CLR_RESULT://	0X05,//机器复位 清除运行结果
				temp = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx); //动作类型
				iPara = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx); 
				if(iPara==2)	{
					pUsart->tx_idx = 0;
					if(temp==0)	{//查询结果
						data_buf[pUsart->tx_idx++] = Sys.DevAction;
					}else if(temp==1)	{//执行复位
						if(SysMotor.ALLMotorState.ubyte == 0)	{//没有电机运行
							SysLogicErr.logic = 0;
							Sys.DevAction = ActionState_Doing;
							MotorReset(MOTOR_X_ID);//X Y电机复位
							MotorReset(MOTOR_Y_ID);
							data_buf[pUsart->tx_idx++] = ActionState_Doing;
						}
						else {
							data_buf[pUsart->tx_idx++] = ActionState_Busy;
						}
					}
					pUsart->tx_cmd = _CMD_TX_CLR_RESULT;
				}
				break;
			case _CMD_RX_SYS_TEST://0X06,//系统测试
				pUsart->tx_cmd = _CMD_TX_SYS_TEST;
				temp = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx); //动作类型
				iPara = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx);//电机类型 
				//SysMotor.MotorIDRunning = iPara - 3;
				iPara -= 3;
				if(iPara>=MOTOR_X_ID && iPara <= MOTOR_QuHuoMen_ID)	{//检测测试电机类型是否符合要求
					//
				}else {
					break;
				}	
				SysMotor.MotorIDRunning = iPara;
				iPara += 3;		
				if(temp==1)	{//执行测试					
					DevState.bits.State = DEV_STATE_TEST;//设备状态 独立部件运行
					if(iPara==3)	{
						SysMotor.ALLMotorState.bits.XMotor = DEF_Run;
						SysMotor.motor[MOTOR_X_ID].ObjPos = UsartRxGetINT16U(pUsart->rx_buf,&pUsart->rx_idx);
						XMotorStart();						
					}
					else if(iPara==4)	{
						SysMotor.ALLMotorState.bits.YMotor = DEF_Run;
						SysMotor.motor[MOTOR_Y_ID].ObjPos = UsartRxGetINT16U(pUsart->rx_buf,&pUsart->rx_idx);
						YMotorStart();
					}
					else if(iPara==5)	{//履带电机
						SysMotor.ALLMotorState.bits.ZMotor = DEF_Run;
						SysMotor.motor[MOTOR_Z_ID].Param = UsartRxGetINT16U(pUsart->rx_buf,&pUsart->rx_idx);
						ZMotorStart();
					}
					else if(iPara==6)	{//推杆电机
						SysMotor.ALLMotorState.bits.TMotor = DEF_Run;
						SysMotor.motor[MOTOR_T_ID].Param = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx);
						TMotorStart();
					}
					else if(iPara==7)	{//门电机
						SysMotor.ALLMotorState.bits.DMotor = DEF_Run;
						SysMotor.motor[MOTOR_D_ID].Param = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx);
						DMotorStart();
					}
					else if(iPara==8)	{//取货履带电机
						SysMotor.ALLMotorState.bits.LMotor = DEF_Run;
						SysMotor.motor[MOTOR_L_ID].Param = UsartRxGetINT16U(pUsart->rx_buf,&pUsart->rx_idx);
						LMotorStart();
					}
					else if(iPara==9)	{//取货口门电机
						SysMotor.ALLMotorState.bits.QuHuoMenMotor = DEF_Run;
						SysMotor.motor[MOTOR_QuHuoMen_ID].Param = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx);
						QuHuoMenMotorStart();
					}
					MotorTest();
					pUsart->tx_idx = 0;				
//					data_buf[pUsart->tx_idx++] = iPara;
					data_buf[pUsart->tx_idx++] = ActionState_Doing;
					Sys.DevAction = ActionState_Doing;
				}
				else if(temp==0)	{//查询测试情况
					pUsart->tx_idx = 0;				
					data_buf[pUsart->tx_idx++] = iPara;
					data_buf[pUsart->tx_idx++] = Sys.DevAction;//SysMotor.motor[SysMotor.MotorIDRunning].status.action;										
				}
				break;
			case _CMD_RX_SHIP_OK:	{//0X08,//通知出货完成
				DevState.bits.State = DEV_STATE_IDLE;
				DevState.bits.SubState = DEV_ShipStateCeMenOpening;
				pUsart->tx_cmd = _CMD_TX_SHIP_OK;
				break;
			}
			default:
				pUsart->tx_cmd = _CMD_TX_NONE;
				break;
		}		
	}else {//数据解析异常
        pUsart->rx_err = MSG_ERR_NONE;        // clear rx error
    }		
	pUsart->rx_flag = DEF_No;//清解析完成标志
}

static uint8_t uart_message_rx_handler(usart_t *pUsart, uint8_t rx_dat)
{
    switch (pUsart->rx_state) {
        case PRO_RX_STATE_SD0:                     /* waiting for start first  start delimiter (SD0)  */
            if (rx_dat == _485_PROTOCOL_RX_SD0) {//机器类型
                pUsart->rx_state = PRO_RX_STATE_SD1;
                pUsart->rx_crc	 = rx_dat;
                pUsart->rx_idx   = 0;
                pUsart->rx_cnt   = 0;
            }
            break;

        case PRO_RX_STATE_SD1:                     /* waiting for start second start delimiter (SD1)  */
            if (rx_dat == _485_PROTOCOL_RX_SD1) {//机器地址
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
			if(pUsart->rx_len==0)	{
				pUsart->rx_state = PRO_RX_STATE_CHKSUM;
			}
			else if (/*(pUsart->rx_len < PRO_EXTENT_LEN) || */(pUsart->rx_len > pUsart->rx_bufsize)) {
                pUsart->rx_state = PRO_RX_STATE_SD0;/* Can not handle this size ...                    */
                pUsart->rx_err   = MSG_ERR_LENGTH;
				pUsart->rx_flag		 = DEF_Yes;
            } else {              
                pUsart->rx_state = PRO_RX_STATE_DATA;
            }
			pUsart->rx_crc  ^= rx_dat;
            break;
        case PRO_RX_STATE_DATA:                    /* waiting for data                                */
            if (pUsart->rx_cnt < pUsart->rx_bufsize) {
                pUsart->rx_buf[pUsart->rx_cnt] = rx_dat;
				pUsart->rx_cnt ++;
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
	//uint8_t temp;

	usart.tx_flag = DEF_Busy;//串口发送忙标记
	RS485_CTRL = 1;
	delay_us(4000);
	usart.tx_buf[idx++] = _485_PROTOCOL_RX_SD0;
	usart.tx_buf[idx++] = _485_PROTOCOL_RX_SD1;
	usart.tx_buf[idx++] = cmd;
	usart.tx_buf[idx++] = len;
	if (len) {
        memcpy(&usart.tx_buf[idx], pdat, len);
    }
	idx += len;
	usart.tx_buf[idx++] = CRC8_XOR(usart.tx_buf, idx);
	usart.tx_len = idx;
	usart.tx_idx = 0;
	usart4_tx_int_enable();
}

static void uart_message_tx_handler(usart_t *pUsart)
{
	INT8U  tx_dat;	
	
	if(pUsart->tx_idx<pUsart->tx_len)	{
		tx_dat = pUsart->tx_buf[usart.tx_idx];
		UART4_SendByte(tx_dat);
		pUsart->tx_idx++;
	}else {
		if(pUsart->tx_cmd == _CMD_TX_RESET)	{//指令发送完成后 再复位
			if(Sys.DevAction != ActionState_Doing)	{
				Sys.state |= SYSSTATE_RESET;
			}
		}
		pUsart->tx_cmd = _CMD_TX_NONE;
		pUsart->tx_flag = DEF_Idle;//发送完成
	}
}

static uint8_t  UsartRxGetINT8U (uint8_t *buf,uint16_t *idx)
{
    return (buf[(*idx)++]);
}

static uint16_t  UsartRxGetINT16U (uint8_t *buf,uint16_t *idx)
{
    uint16_t  lowbyte;
    uint16_t  highbyte;

    highbyte = UsartRxGetINT8U(buf,idx);
    lowbyte = UsartRxGetINT8U(buf,idx);
    return ((highbyte << 8) | lowbyte);
}

static uint32_t  UsartRxGetINT32U (uint8_t *buf,uint16_t *idx)
{
    uint32_t  highword;
    uint32_t  lowword;

    highword = UsartRxGetINT16U(buf,idx);
    lowword = UsartRxGetINT16U(buf,idx);
    return ((highword << 16) | lowword);
}