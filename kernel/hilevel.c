
#include "hilevel.h"

void configure_timer_rst() {
  TIMER0->Timer1Load  = TIMER_INTERVAL_TICKS;
  // 32-bit timer
  TIMER0->Timer1Ctrl  = 0x00000002;
  // periodic timer
  TIMER0->Timer1Ctrl |= 0x00000040;
  // enable interrupt
  TIMER0->Timer1Ctrl |= 0x00000020;
  // enable timer
  TIMER0->Timer1Ctrl |= 0x00000080;
  return;
}

/**
 * General interrupt controller config
 */
void configure_gic_rst() {
  // unmask all interrupts
  GICC0->PMR          = 0x000000F0;
  // enable timer interrupt
  GICD0->ISENABLER1  |= 0x00000010;
  // enable GIC interface
  GICC0->CTLR         = 0x00000001;
  // enable GIC distributor
  GICD0->CTLR         = 0x00000001;
  return;
}

/**
 * Configure system at start
 *  - initialise timer for pre-emptive multitasking
 */
void hilevel_handler_rst(ctx_t* ctx) {

  configure_timer_rst();
  configure_gic_rst();

  sched_rst(ctx);
  int_enable_irq();

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
 * See defined calls in libc.h
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
      const int fd   = (int)  (ctx->gpr[0]);
      const char* x  = (char*)(ctx->gpr[1]);
      const size_t n = (int)  (ctx->gpr[2]);

      ctx->gpr[0] = io_write(executing_process(), fd, x, n);
      break;
    }

    // int  read(int fd, void* x, size_t n)
    case SYS_READ: {
      const int fd   = (int)  (ctx->gpr[0]);
      char* x        = (char*)(ctx->gpr[1]);
      const size_t n = (int)  (ctx->gpr[2]);

      ctx->gpr[0] = io_read(executing_process(), fd, x, n);
      break;
    }

    // pid_t fork()
    case SYS_FORK: {
      sched_fork(ctx);
      break;
    }

    // void exec(const void* x)
    case SYS_EXEC: {
      // check for NULL pointer argument
      if (0 == ctx->gpr[0]) {
        // return -1 and do nothing else
        ctx->gpr[0] = -1;
      }
      else {
        // PC = x (PC goes where the pointer points at)
        ctx->pc = ctx->gpr[0];
        ctx->lr = 0;
        memset(ctx->gpr, 0, sizeof(ctx->gpr));

        // dirty reset of stack
        pcb_t* exec = update_pcb_of_executing_process(ctx);
        ctx->sp = mem_block_addr_end(exec->mem_base_addr);
      }
      break;
    }

    // void exit(int x)
    case SYS_EXIT: {
      int x = ctx->gpr[0];

      sched_terminate(executing_process(), ctx);
      break;
    }

    //int kill(pid_t pid, int x)
    case SYS_KILL: {
      pid_t pid = ctx->gpr[0];
      int signal = ctx->gpr[1];

      // return error code
      ctx->gpr[0] = sched_terminate(pid, ctx);
      break;
    }

    // int pipe(int* fd);
    case SYS_PIPE: {
      int* fd = (int *) (ctx->gpr[0]);

      ctx->gpr[0] = pipe_create(executing_process(), fd);
      break;
    }

    // int close(int fd)
    case SYS_CLOSE: {
      int fd = (int) ctx->gpr[0];

      ctx->gpr[0] = io_close(executing_process(), fd);
      break;
    }

    // unknown/unsupported
    default: {
      break;
    }
  }

  return;
}
