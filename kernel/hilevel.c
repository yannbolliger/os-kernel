
#include "hilevel.h"


/**
 * Configure system at start
 *  - initialise timer for pre-emptive multitasking
 */
void hilevel_handler_rst() {

  // Timer config
  TIMER0->Timer1Load  = TIMER_INTERVAL_TICKS;

  // 32-bit timer
  TIMER0->Timer1Ctrl  = 0x00000002;
  // periodic timer
  TIMER0->Timer1Ctrl |= 0x00000040;
  // enable interrupt
  TIMER0->Timer1Ctrl |= 0x00000020;
  // enable timer
  TIMER0->Timer1Ctrl |= 0x00000080;

  // General interrupt controller config
  // unmask all interrupts
  GICC0->PMR          = 0x000000F0;
  // enable timer interrupt
  GICD0->ISENABLER1  |= 0x00000010;
  // enable GIC interface
  GICC0->CTLR         = 0x00000001;
  // enable GIC distributor
  GICD0->CTLR         = 0x00000001;

  int_enable_irq();

  return;
}

/**
 * IRQ interrupt handler
 * handles the following hardware (low-level) interrupts
 *   - Timer innterrupt
 */
void hilevel_handler_irq() {
  // Read  the interrupt identifier
  uint32_t id = GICC0->IAR;

  // handle timer interrupt
  if (id == GIC_SOURCE_TIMER0) {
    PL011_putc(UART0, 'T', true);

    // invoke scheduler in SVC mode
    yield();

    // reset timer
    TIMER0->Timer1IntClr = 0x01;
  }

  // Acknowledge interrupt identifier
  GICC0->EOIR = id;

  return;
}


/**
 * SVC interrupt handler,
 * responsible for handling all syscall (SVC) interrupts.
 * See defined interrupts in libc.h
 */
void hilevel_handler_svc(ctx_t* ctx, uint32_t svc_code) {

  switch (svc_code) {

    // yield()
    case SYS_YIELD: {
      //scheduler(ctx);
      break;
    }

    // write(fd, x, n)
    case SYS_WRITE: {
      int fd  = (int)  (ctx->gpr[0]);
      char* x = (char*)(ctx->gpr[1]);
      int n   = (int)  (ctx->gpr[2]);

      for (int i = 0; i < n; i++) {
        PL011_putc(UART0, *x++, true);
      }

      ctx->gpr[0] = n;
      break;
    }

    // unknown/unsupported
    default: {
      break;
    }
  }

  return;
}
