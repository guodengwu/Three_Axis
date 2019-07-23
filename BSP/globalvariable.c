#include "globalvariable.h"

_sys_status_t sys_status;
_imei_t IMEI;
_syserror_t SysError;
_systempctrl_t TempCtrl;
_HuaShuangCtrl_t HuaShuangCtrl;
_SaveShipData_t SaveShipDat;

//////////////////////////////////////////////////////////////////
//ÓÃ»§Ó¦ÓÃ³ÌĞòÈ«¾Ö±äÁ¿³õÊ¼»¯
void globalvariable (void)
{
		sys_status.IR_CheckFlag = DEF_False;//»õÎï¼ì²â±êÖ¾
		sys_status.light_ctrl_enable = DEF_True;//µÆ¹â¿ØÖÆÊ¹ÄÜ
		sys_status.online_state = DEF_False;//ÁªÍø×´Ì
		sys_status.door_state = DOOR_OPEN;
		sys_status.pIMEI = &IMEI;//¶©µ¥ºÅ
		SysError.IR = 0;
		SysError.Compressor = 0;
		SysError.TempSensor = 0;
		SysError.flag = DEF_False;//´íÎó±ê¼Ç£¬Ä¬ÈÏÎŞ´íÎó
		sys_status.pError = &SysError;
		TempCtrl.inside_temp = -50;//Ä¬ÈÏÊÒÄÚÎÂ¶È
		TempCtrl.outside_temp = -50;
		TempCtrl.tInsideTempL = 0;//ÊÒÄÚÎÂ¶È¿ØÖÆ·¶Î§
		TempCtrl.tInsideTempH = 0;
		TempCtrl.flag = DEF_False;//Ä¬ÈÏVMC²»×Ô¶¯µ÷½ÚÎÂ¶È
		sys_status.pTempCtrl = &TempCtrl;
		SaveShipDat.flag = DEF_False;
		flash_savedat.type = SAVE_NONE;
	
		HuaShuangCtrl.enable = DEF_False;
		HuaShuangCtrl.runflag = DEF_False;
}

















