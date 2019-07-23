#include "protocol.h"
#include "misc.h"

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

//协议指令回复
void UsartCmdReply(void)
{
	uint8_t idx=0;
	usart_t *pUsart = &usart;
	
	if(pUsart->tx_flag == DEF_Busy)	{//发送忙 等待空闲
		return;
	}
	switch(pUsart->tx_cmd)	{
		case _CMD_TX_GET_VERSION:
			data_buf[0] = 1;
			strcpy(data_buf+1, VERSION);
			PackageSendData(_CMD_TX_GET_VERSION, data_buf, strlen(VERSION)+1); 
			break;
		default:
			break;
	}
}

//串口指令处理函数
void  UsartCmdProcess (void)
{
    uint8_t cmd,iPara;
	usart_t *pUsart = &usart;

	if(pUsart->rx_flag==DEF_No)	{//无数据接收 返回
		return;
	}
    
    if (pUsart->rx_err == MSG_ERR_NONE) {//数据解析无错误
//		pUsart->rx_cmd = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx);                                 /* First byte contains command      */
		switch(pUsart->rx_cmd)	{
			case _CMD_RX_GET_STATE:
				pUsart->tx_cmd = _CMD_TX_GET_STATE;
				break;
			case _CMD_RX_GET_VERSION:
				pUsart->tx_cmd = _CMD_TX_GET_VERSION;
			default:
				pUsart->tx_cmd = _CMD_TX_NONE;
		}
		
	}else {//数据解析异常
        //UsartSendError(&msg_pkt_usart[0], pUsart->rx_err);
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
	uint8_t idx=0,datlen;
	uint8_t temp;

	usart.tx_flag = DEF_Busy;//串口发送忙标记
	usart.tx_buf[idx++] = _485_PROTOCOL_RX_SD0;
	usart.tx_buf[idx++] = _485_PROTOCOL_RX_SD1;
	usart.tx_buf[idx++] = cmd;
	usart.tx_buf[idx++] = len;
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
	
	if(usart.tx_idx<=usart.tx_len)	{
		//tx_dat = usart.tx_buf[usart.tx_idx++];
		UART4_SendByte(usart.tx_buf[usart.tx_idx++]);
	}else {
		usart.tx_flag = DEF_Idle;//发送完成
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