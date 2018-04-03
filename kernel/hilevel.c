
#include "hilevel.h"


/**
 * Configure system at start
 *  - initialise timer for pre-emptive multitasking
 */
void hilevel_handler_rst(ctx_t* ctx) {

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

  sched_rst(ctx);

  return;
}

/**
 * IRQ interrupt handler
 * handles the following hardware (low-level) interrupts
 *   - Timer innterrupt
 */
void hilevel_handler_irq(ctx_t* ctx) {
  // Read  the interrupt identifier
  uint32_t id = GICC0->IAR;

  // handle timer interrupt
  if (id == GIC_SOURCE_TIMER0) {
    sched_tick();

    if (sched_need_resched()) sched(ctx);

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

    // void yield()
    case SYS_YIELD: {
      sched(ctx);
      break;
    }

    // int write(fd, x, n)
    case SYS_WRITE: {
      int fd  = (int)  (ctx->gpr[0]);
      char* x = (char*)(ctx->gpr[1]);
      int n   = (int)  (ctx->gpr[2]);

      for (int i = 0; i < n; i++) {
        PL011_putc(UART0, *x++, true);
      }

      // return value
      ctx->gpr[0] = n;
      break;
    }

    // pid_t fork()
    case SYS_FORK: {
      sched_fork(ctx);
      break;
    }

    // void exec(const void* x)
    case SYS_EXEC: {
      const void * x = ctx->gpr[0];

      ctx->pc = (uint32_t) x;
      ctx->sp = pcb_of(executing_process())->base_sp;
      memset(ctx->gpr[0], 0, sizeof(ctx->gpr));

      break;
    }

    // unknown/unsupported
    default: {
      break;
    }
  }

  return;
}
