#include "mt_task.h"
#include "M0518.h"
#include "mb.h"
#include "mt_modbus.h"
void mt_task_init(void)
{
	MtModbusInit();
}

void mt_task(void)
{
	( void )eMBPoll( );
}






