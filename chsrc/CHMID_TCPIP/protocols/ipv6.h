/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */
/* 
Modifications of this software Copyright(C) 2008, Standard Microsystems Corporation
  All Rights Reserved.
  The modifications to this program code are proprietary to SMSC and may not be copied,
  distributed, or used without a license to do so.  Such license may have
  Limited or Restricted Rights. Please refer to the license for further
  clarification.
*/
/*
File: ipv6.h
*/

#ifndef SMSC_IPV6_H
#define SMSC_IPV6_H     

#include "smsc_environment.h"

#if !IPV6_ENABLED
#error IPv6 is not enabled
#endif
                   
#error IPv6 is not implemented yet

#if SMSC_ERROR_ENABLED
#define IPV6_DATA_SIGNATURE		(0x92FBEEFF)
#endif

/* TODO IPV6_ADDRESS need to be define correctly */
typedef u32_t IPV6_ADDRESS;

typedef struct IPV6_DATA_ {
	DECLARE_SIGNATURE
	FUNCTION_POINTER mOutputFunction;
	/* TODO the three addresses below were copied from the mIpv4Data
		I'm not yet sure if that is appropriate for Ipv6 so these
		addresses may need updating */
	IPV6_ADDRESS mAddress;/* must be stored in network byte order */
	IPV6_ADDRESS mNetMask;/* must be stored in network byte order */
	IPV6_ADDRESS mGateway;/* must be stored in network byte order */
} IPV6_DATA, * PIPV6_DATA;
	
void Ipv6_Initialize();
void Ipv6_InitializeInterfaceData(PIPV6_DATA data);

#endif /* SMSC_IPV6_H */
