/* ###################################################################
**     Filename    : main.c
**     Project     : Firmware_Tangke
**     Processor   : MC9S08QE128CLK
**     Version     : Driver 01.12
**     Compiler    : CodeWarrior HCS08 C Compiler
**     Date/Time   : 2018-01-27, 23:16, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.12
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */
/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "Bit1.h"
#include "Bit2.h"
#include "PWM1.h"
#include "Cap1.h"

#include "AS1.h"
#include "TI1.h"
#include "AD1.h"
/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

//	DECLARANDO VARIABLES:	
int estado;
unsigned char busin;
unsigned int Enviados = 3;	
byte err;
word Sent;
struct {
	byte Trama1Canal_1;
	byte Trama2Canal_1;
	byte Trama1Canal_2;
	byte Trama2Canal_2;
	byte Trama1Canal_3;
	byte Trama2Canal_3;
}CANALES;
struct {
	byte Hum_int;
	/*byte Hum_dec;*/
	byte Temp_int;
	/*byte Temp_dec;*/
	/*byte Pari;*/
}DTH11;
unsigned int Medida;
bool D1;
bool D2;


void main(void)
{
  /* Write your local variable definition here */
  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/
  estado = 1;
  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  //	Iniciamos la Maquina de Estados:	
    for(;;){
    	switch (estado){ 
    	/*	Estados:
    	 * 1. Espera
    	 * 2. Medir
    	 * 3. Enviar
    	 */   	
  		case 1:
  			/*Espera...*/
  			break;
  		case 2:
  			//	Para un posible uso del ADC:
  			/*busin = AD1_Measure(TRUE);
  			busin = AD1_GetValue16(&Medida);*/
  			//	Guardamos el Estado de los Sensores Digitales:	
  			D2 = Bit1_GetVal();
  			D1 = Bit2_GetVal();
  			estado = 3;
  			break;
  		case 3:
  			//	Incorporamos la data medida al protocolo de comunicacion: 
  			CANALES.Trama1Canal_1 = (Medida >> 7) & 0x1F;
  			CANALES.Trama2Canal_1 = (Medida)  & 0x7F;
			CANALES.Trama1Canal_2 = (DTH11.Temp_int >> 7) & 0x1F;
			CANALES.Trama2Canal_2 = (DTH11.Temp_int) & 0x7F;
			CANALES.Trama1Canal_3 = (DTH11.Hum_int >> 7) & 0x1F;
			CANALES.Trama2Canal_3 = (DTH11.Hum_int) & 0x7F;
			if(D1==0x08){
				CANALES.Trama1Canal_1=CANALES.Trama1Canal_1&0x3F;
				/*CANALES.Trama1Canal_2=CANALES.Trama1Canal_2&0x3F;*/
			}else{
				CANALES.Trama1Canal_1=CANALES.Trama1Canal_1|0x40;
				/*CANALES.Trama1Canal_2=CANALES.Trama1Canal_2|0x40;*/	
			}
			if(D2==0x04){
				CANALES.Trama1Canal_1=CANALES.Trama1Canal_1&0x5F;
				/*CANALES.Trama1Canal_2=CANALES.Trama1Canal_2&0x5F;*/
			}else{
			  	CANALES.Trama1Canal_1=CANALES.Trama1Canal_1|0x20;
			  	/*CANALES.Trama1Canal_2=CANALES.Trama1Canal_2|0x20;*/
			}
  			//	Se envia la data por tx serial:	
  			err=AS1_SendChar(CANALES.Trama1Canal_1);
  			err=AS1_SendChar(CANALES.Trama2Canal_1);
  			err=AS1_SendChar(CANALES.Trama1Canal_2);
  			err=AS1_SendChar(CANALES.Trama2Canal_2);
  			err=AS1_SendChar(CANALES.Trama1Canal_3);
  			err=AS1_SendChar(CANALES.Trama2Canal_3);
			/*err = AS1_SendBlock((byte*)&CANALES,6,&Sent);*/ 
  			estado = 1;
  			break;
  		default:
  			break;
  	}
  }
  
 
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
}
