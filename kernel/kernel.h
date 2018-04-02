

#ifndef __KERNEL_H
#define __KERNEL_H

/**
 * Type Definitions Kernel-wide
 */

// Process IDentifier (PID) type
typedef int pid_t;

/**
 * Timer configuration
 * The timers of the Cortex-A8 ["are clocked by a 32.768kHz reference"]
 * (http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0417d/index.html).
 *
 * In accordance to earlier Linux schedulers, the timer should fire 100 times a
 * second and therefore:
 */

#define HZ                   (100)
#define TIMER_CLOCK          (2^15)
#define TIMER_INTERVAL_TICKS (TIMER_CLOCK/HZ)


#endif
