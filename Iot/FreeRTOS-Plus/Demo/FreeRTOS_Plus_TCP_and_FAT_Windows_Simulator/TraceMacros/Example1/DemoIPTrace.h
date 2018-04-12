/*
    FreeRTOS V9.0.0 - Copyright (C) 2016 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>!AND MODIFIED BY!<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

/*
 * This file, along with DemoIPTrace.c, provides a basic example use of the
 * FreeRTOS+TCP trace macros.  The statistics gathered here can be viewed in
 * the command line interface.
 * See http://www.FreeRTOS.org/FreeRTOS-Plus/FreeRTOS_Plus_TCP/UDP_CLI.html
 *
 * A simple generic mechanism is used that allocates a structure (see the
 * ExampleDebugStatEntry_t definition below) to each ID defined in this file.
 * The structures are stored in an array (see the xIPTraceValues[] array in
 * DemoIPTrace.c).
 *
 * The structure associates a function with a data value.  See the
 * vPerformAction and ulData members of ExampleDebugStatEntry_t respectively.
 * The function is used to manipulate the data.  At the time of writing two
 * functions can be used - these are prvIncrementEventCount() which simply
 * increments the data each time it is called, and prvStoreLowest() which
 * sets the data to the lowest value of an input parameter ever seen.  For
 * example, to store the lowest ever number of free network buffer descriptors
 * the parameter value is the current number of network buffer descriptors.
 *
 * The trace macros themselves are defined in this file and just invoke
 * vExampleDebugStatUpdate(), passing in an ID value.  vExampleDebugStatUpdate()
 * then just executes the function associated with that value (prvStoreLowest(),
 * prvIncrementEventCount(), etc.) as defined in the xIPTraceValues[] array.
 */

#ifndef DEMO_IP_TRACE_MACROS_H
#define DEMO_IP_TRACE_MACROS_H

typedef void ( *vTraceAction_t )( uint32_t *, uint32_t );

/* Type that defines each statistic being gathered. */
typedef struct ExampleDebugStatEntry
{
	uint8_t ucIdentifier;				/* Unique identifier for statistic. */
	const char * const pucDescription;	/* Text description for the statistic. */
	vTraceAction_t vPerformAction;		/* Action to perform when the statistic is updated (increment counter, store minimum value, store maximum value, etc. */
	uint32_t ulData; 					/* The meaning of this data is dependent on the trace macro ID. */
} ExampleDebugStatEntry_t;

/* Unique identifiers used to locate the entry for each trace macro in the
xIPTraceValues[] table defined in DemoIPTrace.c.  See the comments at the top of
this file. */
#define iptraceID_NETWORK_DOWN                                  0
#define iptraceID_TOTAL_NETWORK_BUFFERS_RELEASED                1
#define iptraceID_NETWORK_BUFFER_OBTAINED                       2
#define iptraceID_TOTAL_NETWORK_BUFFERS_OBTAINED                3
#define iptraceID_FAILED_TO_OBTAIN_NETWORK_BUFFER               4
#define iptraceID_CREATING_ARP_REQUEST                          5
#define iptraceID_ARP_TABLE_ENTRY_EXPIRED                       6
#define iptraceIP_ARP_TABLE_ENTRY_CREATED                       7
#define iptraceID_SENDING_UDP_PACKET                            8
#define iptraceID_PACKET_DROPPED_TO_GENERATE_ARP                9
#define iptraceID_TOTAL_ICMP_PACKET_RECEIVED                    10
#define iptraceID_TOTAL_PING_REPLY                              11
#define traceID_TOTAL_ARP_PACKET_RECEIVED                       12
#define iptraceID_TOTAL_ARP_UPDATES                             13
#define iptraceID_TOTAL_SENT_ARP_REPLIES                        14
#define iptraceID_TOTAL_FAILED_TO_CREATE_SOCKET                 15
#define iptraceID_TOTAL_FAILED_TO_CREATE_EVENT_GROUP            16
#define iptraceID_TOTAL_RECVFROM_DISCARDED_BYTES                17
#define iptraceID_TOTAL_STACK_TX_EVENT_LOST                     18
#define iptraceID_TOTAL_NETWORK_EVENTS                          19
#define iptraceID_BIND_FAILED                                   20
#define iptraceID_TOTAL_DHCP_REQUESTS_FAILED_DEFAULT_ADDRESS    21
#define iptraceID_TOTAL_DHCP_DISCOVER                           22
#define iptraceID_TOTAL_DHCP_REQUEST                            23
#define iptraceID_TOTAL_DHCP_SUCCEDEED                          24
#define iptraceID_TOTAL_NETWORK_INTERFACE_TRANSMIT              25
#define iptraceID_TOTAL_NETWORK_INTERFACE_RECEIVE               26
#define iptraceID_TOTAL_DNS_REQUEST                             27
#define iptraceID_TOTAL_RECVFROM_TIMEOUT                        28
#define iptraceID_TOTAL_RECVFROM_INTERRUPTED                    29
#define iptraceID_TOTAL_NO_BUFFER_FOR_SENDTO                    30
#define iptraceID_TOTAL_SENDTO_SOCKET_NOT_BOUND                 31
#define iptraceID_TOTAL_SENDTO_DATA_TOO_LONG                    32

/* It is possible to remove the trace macros using the
configINCLUDE_DEMO_DEBUG_STATS setting in FreeRTOSIPConfig.h. */
#if configINCLUDE_DEMO_DEBUG_STATS == 1

	/* The trace macro definitions themselves.  Any trace macros left undefined
	will default to be empty macros.  See the comments at the top of this
	file. */
#define iptraceNETWORK_DOWN() vIpDebugStatUpdate( iptraceID_NETWORK_DOWN, 0 )
#define iptraceNETWORK_BUFFER_RELEASED( pxBufferAddress ) vIpDebugStatUpdate( iptraceID_TOTAL_NETWORK_BUFFERS_RELEASED, 1 )
#define iptraceNETWORK_BUFFER_OBTAINED( pxBufferAddress ) vIpDebugStatUpdate( iptraceID_NETWORK_BUFFER_OBTAINED, uxQueueMessagesWaiting( ( QueueHandle_t ) xNetworkBufferSemaphore ) ); vIpDebugStatUpdate( iptraceID_TOTAL_NETWORK_BUFFERS_OBTAINED, 1 )
#define iptraceFAILED_TO_OBTAIN_NETWORK_BUFFER() vIpDebugStatUpdate( iptraceID_FAILED_TO_OBTAIN_NETWORK_BUFFER, 1 )
#define iptraceFAILED_TO_OBTAIN_NETWORK_BUFFER_FROM_ISR() vIpDebugStatUpdate( iptraceID_FAILED_TO_OBTAIN_NETWORK_BUFFER_FROM_ISR, 1 )
#define iptraceCREATING_ARP_REQUEST( ulIPAddress ) vIpDebugStatUpdate( iptraceID_CREATING_ARP_REQUEST, 1 )
#define iptraceARP_TABLE_ENTRY_EXPIRED( ulIPAddress ) vIpDebugStatUpdate( iptraceID_ARP_TABLE_ENTRY_EXPIRED, 1 )
#define iptraceARP_TABLE_ENTRY_CREATED( ulIPAddress, ucMACAddress ) vIpDebugStatUpdate( iptraceIP_ARP_TABLE_ENTRY_CREATED, 1 )
#define iptraceSENDING_UDP_PACKET( ulIPAddress ) vIpDebugStatUpdate( iptraceID_SENDING_UDP_PACKET, 1 )
#define iptracePACKET_DROPPED_TO_GENERATE_ARP( ulIPAddress ) vIpDebugStatUpdate( iptraceID_PACKET_DROPPED_TO_GENERATE_ARP, 1 )
#define iptraceICMP_PACKET_RECEIVED() vIpDebugStatUpdate( iptraceID_TOTAL_ICMP_PACKET_RECEIVED, 1 )
#define iptraceSENDING_PING_REPLY( ulIPAddress ) vIpDebugStatUpdate( iptraceID_TOTAL_PING_REPLY, 1 )
#define traceARP_PACKET_RECEIVED() vIpDebugStatUpdate( traceID_TOTAL_ARP_PACKET_RECEIVED, 1 )
#define iptracePROCESSING_RECEIVED_ARP_REPLY( ulIPAddress ) vIpDebugStatUpdate( iptraceID_TOTAL_ARP_UPDATES, 1 )
#define iptraceSENDING_ARP_REPLY( ulIPAddress ) vIpDebugStatUpdate( iptraceID_TOTAL_SENT_ARP_REPLIES, 1 )
#define iptraceFAILED_TO_CREATE_SOCKET() vIpDebugStatUpdate( iptraceID_TOTAL_FAILED_TO_CREATE_SOCKET, 1 )
#define iptraceFAILED_TO_CREATE_EVENT_GROUP() vIpDebugStatUpdate( iptraceID_TOTAL_FAILED_TO_CREATE_EVENT_GROUP, 1 )
#define iptraceRECVFROM_DISCARDING_BYTES( xNumberOfBytesDiscarded ) vIpDebugStatUpdate( iptraceID_TOTAL_RECVFROM_DISCARDED_BYTES, xNumberOfBytesDiscarded )
#define iptraceSTACK_TX_EVENT_LOST( xEvent ) vIpDebugStatUpdate( iptraceID_TOTAL_STACK_TX_EVENT_LOST, 1 )
#define iptraceNETWORK_EVENT_RECEIVED( eEvent ) vIpDebugStatUpdate( iptraceID_TOTAL_NETWORK_EVENTS, 1 )
#define iptraceBIND_FAILED( xSocket, usPort ) vIpDebugStatUpdate( iptraceID_BIND_FAILED, 1 )
#define iptraceDHCP_REQUESTS_FAILED_USING_DEFAULT_IP_ADDRESS( ulIPAddress ) vIpDebugStatUpdate( iptraceID_TOTAL_DHCP_REQUESTS_FAILED_DEFAULT_ADDRESS, 1 )
#define iptraceSENDING_DHCP_DISCOVER() vIpDebugStatUpdate( iptraceID_TOTAL_DHCP_DISCOVER, 1 )
#define iptraceSENDING_DHCP_REQUEST() vIpDebugStatUpdate( iptraceID_TOTAL_DHCP_REQUEST, 1 )
#define iptraceDHCP_SUCCEDEED( address ) vIpDebugStatUpdate( iptraceID_TOTAL_DHCP_SUCCEDEED, 1 )
#define iptraceNETWORK_INTERFACE_TRANSMIT() vIpDebugStatUpdate( iptraceID_TOTAL_NETWORK_INTERFACE_TRANSMIT, 1 )
#define iptraceNETWORK_INTERFACE_RECEIVE() vIpDebugStatUpdate( iptraceID_TOTAL_NETWORK_INTERFACE_RECEIVE, 1 )
#define iptraceSENDING_DNS_REQUEST() vIpDebugStatUpdate( iptraceID_TOTAL_DNS_REQUEST, 1 )
#define iptraceRECVFROM_TIMEOUT() vIpDebugStatUpdate( iptraceID_TOTAL_RECVFROM_TIMEOUT, 1 )
#define iptraceRECVFROM_INTERRUPTED() vIpDebugStatUpdate( iptraceID_TOTAL_RECVFROM_INTERRUPTED, 1 )
#define iptraceNO_BUFFER_FOR_SENDTO() vIpDebugStatUpdate( iptraceID_TOTAL_NO_BUFFER_FOR_SENDTO, 1 )
#define iptraceSENDTO_SOCKET_NOT_BOUND() vIpDebugStatUpdate( iptraceID_TOTAL_SENDTO_SOCKET_NOT_BOUND, 1 )
#define iptraceSENDTO_DATA_TOO_LONG() vIpDebugStatUpdate( iptraceID_TOTAL_SENDTO_DATA_TOO_LONG, 1 )
	/*
	 * The function that updates a line in the xIPTraceValues table.
	 */
	void vIpDebugStatUpdate( uint8_t ucIdentifier, uint32_t ulValue );

	/*
	 * Returns the number of entries in the xIPTraceValues table.
	 */
	BaseType_t xExampleDebugStatEntries( void );

#endif /* configINCLUDE_DEMO_DEBUG_STATS == 1 */


#endif /* DEMO_IP_TRACE_MACROS_H */

