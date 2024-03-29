/* Copyright (C) 2017 Daniel Page <csdsp@bristol.ac.uk>
 *
 * Use of this source code is restricted per the CC BY-NC-ND license, a copy of
 * which can be found via http://creativecommons.org (and should be included as
 * LICENSE.txt within the associated archive or repository).
 */

#ifndef __PL011_H
#define __PL011_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "device.h"

/* The ARM PrimeCell UART (PL011) is documented at
 *
 * http://infocenter.arm.com/help/topic/com.arm.doc.ddi0183g/index.html
 *
 * In particular, Section 3 explains the programmer's model, i.e., how to
 * interact with it: this includes
 *
 * - Section 3.2, which summarises the device register layout in Table 3.1
 *   (including an offset from the device base address, in the memory map,
 *   for each register), and
 * - Section 3.3, which summarises the internal structure of each device
 *   register.
 *
 * Note that the field identifiers used here follow the documentation in a
 * general sense, but with a some minor alterations to improve clarity and
 * consistency.
 */

typedef struct {
          RW uint32_t    DR;            // 0x0000          : data
  union { RW uint32_t   RSR;            // 0x0004          : receive status
          RW uint32_t   ECR;         }; //                 | error   clear
          RO RSVD( 0, 0x0008, 0x0017 ); // 0x0008...0x0017 : reserved
          RO uint32_t    FR;            // 0x0018          : flag
          RO RSVD( 1, 0x001C, 0x001F ); // 0x001C...0x001F : reserved
          RW uint32_t   LPR;            // 0x0020          : low-power counter
          RW uint32_t  IBRD;            // 0x0024          : integer    baud rate
          RW uint32_t  FBRD;            // 0x0028          : fractional baud rate
          RW uint32_t   LCR;            // 0x002C          : line control
          RW uint32_t    CR;            // 0x0030          :      control
          RW uint32_t  IFLS;            // 0x0034          :        interrupt level select
          RW uint32_t  IMSC;            // 0x0038          :        interrupt mask
          RO uint32_t   RIS;            // 0x003C          : raw    interrupt status
          RO uint32_t   MIS;            // 0x0040          : masked interrupt status
          WO uint32_t   ICR;            // 0x0044          :        interrupt clear
          RW uint32_t DMACR;            // 0x0048          : DMA control
          RO RSVD( 2, 0x004C, 0x0FDF ); // 0x004C...0x0FDF : reserved
          RO uint32_t PeriphID0;        // 0x0FE0          : peripheral ID
          RO uint32_t PeriphID1;        // 0x0FE4          : peripheral ID
          RO uint32_t PeriphID2;        // 0x0FE8          : peripheral ID
          RO uint32_t PeriphID3;        // 0x0FEC          : peripheral ID
          RO uint32_t  PCellID0;        // 0x0FF0          : PrimeCell  ID
          RO uint32_t  PCellID1;        // 0x0FF4          : PrimeCell  ID
          RO uint32_t  PCellID2;        // 0x0FF8          : PrimeCell  ID
          RO uint32_t  PCellID3;        // 0x0FFC          : PrimeCell  ID
} PL011_t;

/* Per Table 4.2 (for example: the information is in several places) of
 *
 * http://infocenter.arm.com/help/topic/com.arm.doc.dui0417d/index.html
 *
 * we know the registers are mapped to fixed addresses in memory, so we
 * can just define a (structured) pointer to each one to support access.
 */

extern PL011_t * const UART0;
extern PL011_t * const UART1;
extern PL011_t * const UART2;
extern PL011_t * const UART3;

// convert a hexadecimal character x into an integer 0 < r < 16
extern int  xtoi( char x );
// convert an integer 0 < x < 16 into a hexadecimal character r
extern char itox( int  x );

// check whether transmitting via PL011 instance d will block
extern bool    PL011_can_putc( PL011_t* d );
// check whether    receiving via PL011 instance d will block
extern bool    PL011_can_getc( PL011_t* d );

// transmit raw      byte x via PL011 instance d (blocking iff. f = true)
extern void    PL011_putc( PL011_t* d, uint8_t x, bool f );
// receive  raw      byte r via PL011 instance d (blocking iff. f = true)
extern uint8_t PL011_getc( PL011_t* d,            bool f );
// transmit hexified byte x via PL011 instance d (blocking iff. f = true)
extern void    PL011_puth( PL011_t* d, uint8_t x, bool f );
// receive  hexified byte r via PL011 instance d (blocking iff. f = true)
extern uint8_t PL011_geth( PL011_t* d,            bool f );

#endif
