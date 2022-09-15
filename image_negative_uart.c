/*
 * image_inverson.c
 *
 *  Created on: 30-Jul-2022
 *      Author: hp
 */

#include <stdlib.h>
#include <xil_types.h>
#include <xparameters.h>
#include <xuartps.h>
#include <xil_io.h>

#define imageSize 100*100
#define headerSize 20054
#define fileSize imageSize+headerSize

int main(){
	u8 *imageData;
	u8 *imageData2;
	XUartPs_Config *myUartConfig;
	XUartPs myUart;
	u32 status;
	u32 totalRecievedBytes = 0;
	u32 recievedBytes = 0;
	u32 totalRecievedBytes2 = 0;
	u32 recievedBytes2 = 0;
	u32 totalTransmittedBytes = 0;
	u32 transmittedBytes = 0;

	imageData = malloc(sizeof(u8)*(fileSize));
	myUartConfig =  XUartPs_LookupConfig(XPAR_PS7_UART_1_DEVICE_ID);

	status =  XUartPs_CfgInitialize(&myUart, myUartConfig, myUartConfig->BaseAddress);

	if(status != XST_SUCCESS){
		print("Uart initialization failed...\n\r");
	}

	status =  XUartPs_SetBaudRate(&myUart, 115200);
	if(status != XST_SUCCESS){
			print("baudrate initialization failed...\n\r");
		}
	xil_printf("executing loop 1");
	// Data transfer from computer to DDR
	while(totalRecievedBytes < fileSize){
		recievedBytes =  XUartPs_Recv(&myUart,(u8*)&imageData[totalRecievedBytes],100);
		totalRecievedBytes += recievedBytes;
	}
	xil_printf("executing loop 2");
	for(int i=0;i<100; i++){
		xil_printf("%0x\t", imageData[i]);
	}
	xil_printf("executing loop 3");
	while(totalRecievedBytes2 < fileSize){
			recievedBytes2 =  XUartPs_Recv(&myUart,(u8*)&imageData2[totalRecievedBytes2],100);
//			xil_printf("%0x\n\r", (u8)imageData2[totalRecievedBytes2]);
//			Xil_Out8(XPAR_PS7_DDR_0_S_AXI_BASEADDR+totalRecievedBytes2, (u8)imageData2[totalRecievedBytes2]);
			totalRecievedBytes2 += recievedBytes2;
	}
	for(int i=0;i<100; i++){
				xil_printf("%0x\t", imageData2[i]);
	}
	u8 temp;
	for(int i=0;i<100; i++){
		Xil_Out8(XPAR_PS7_DDR_0_S_AXI_BASEADDR+0x100000+i, imageData2[i]);
	}

	for(int i=0;i<100; i++){
		temp = Xil_In8(XPAR_PS7_DDR_0_S_AXI_BASEADDR+i);
		xil_printf("%0x\t%0x\n\r", temp, imageData2[i]);
	}
	 read from ddr process it and store back in ddr
	for(int i=headerSize;i<fileSize; i++){
		imageData[i] = 255 - imageData[i];
	}
	while(totalTransmittedBytes < fileSize){
		transmittedBytes =  XUartPs_Send(&myUart,(u8*)&imageData[totalTransmittedBytes],1);
		totalTransmittedBytes += transmittedBytes;
		}

}

