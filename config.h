/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File: config.h
 * Author: Groffils Karel
 * Comments: Global configuration file
 * Revision history: last update 22/05/2020
 */

#ifndef CONFIG_H
#define	CONFIG_H

#include <pic18f25k50.h>
#include <xc.h> // contains register names
#include <stdio.h> // sprintf


#define _XTAL_FREQ 1000000 //1MHz
#define BaudRate 9600
#define true 1
#define false 0

//global variables
#define BUFFER_SIZE 8
#define CODE_PARITY_ERROR 255
#define CODE_RX_ERROR 254
#define CODE_SENDER_ID 253
#define CODE_RECIPIENT_ID 252
#define CODE_REQUEST_ID 251
#define CODE_DATA_RESEND_REQUEST 250

extern unsigned char trpBufferSend[BUFFER_SIZE];
extern unsigned char trpBufferSendID[BUFFER_SIZE];
extern unsigned char trpBufferRecieve[BUFFER_SIZE];
extern unsigned char trpBufferSendIndex;
extern unsigned char trpBufferRecieveIndex;
#endif	

