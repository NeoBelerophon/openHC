;Reservation of memory for monitor
;and dummy declaration for application functions
;Maximum needed stack-space: 	
	.PROGRAM  MONITORFUNC
    .TITLE    "MONITOR RESERVATION AND FUNCTIONS"

    ;Section reservations for kernel

	.SECTION ACC_MDATA,DATA,LOCATE=0x000380
	.RES.B 0x80
	
	.SECTION ACC_UDATA,CONST,LOCATE=0xFFFB00
	.RES.B 0x100

    ;Kernel 
    .SECTION ACC_MCODE,CONST,LOCATE=0x00FFA000
	.EXPORT _acc_InitMonitorMsgBuffer
	.EXPORT _acc_MessageBox
	.EXPORT _acc_printf
	.EXPORT _acc_WatchdogEnable
    .EXPORT _acc_KernelUART

     .ORG 0xFFAc20	
	_acc_InitMonitorMsgBuffer: 	.RES.L 1
	_acc_MessageBox:			.RES.L 1
	_acc_printf:				.RES.L 1
	_acc_WatchdogEnable:		.RES.L 1
    _acc_KernelUART:            .RES.L 1

	.ORG 0xFFBFFF	
	.RES.B   0x1           ;dummy reservation for flash sector
	.END


