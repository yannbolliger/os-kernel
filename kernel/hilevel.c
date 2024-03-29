
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

int restart_on_fatal(int err_code) {
  if (err_code == FATAL_CODE)  {
    kernel_write_error("\n\n\nFATAL. Restarting system.\n\n\n\n\n", 33);

    // the program never returns from the reset; it restarts the system
    mmu_unable();
    lolevel_handler_rst();
  }

  return err_code;
}


/**
 * Configure system at start
 *  - initialise timer for pre-emptive multitasking
 */
void hilevel_handler_rst(ctx_t* ctx) {

  configure_timer_rst();
  configure_gic_rst();

  page_rst();
  mem_rst();
  pipe_rst();
  restart_on_fatal(sched_rst(ctx));

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

    if (sched_need_resched()) restart_on_fatal(sched(ctx));

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
      restart_on_fatal(sched(ctx));
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
      sched_exec(ctx);
      break;
    }

    // void exit(int x)
    case SYS_EXIT: {
      int x = ctx->gpr[0];

      restart_on_fatal(sched_terminate(executing_process(), ctx));
      break;
    }

    //int kill(pid_t pid, int x)
    case SYS_KILL: {
      pid_t pid = ctx->gpr[0];
      int signal = ctx->gpr[1];

      // return error code
      ctx->gpr[0] = restart_on_fatal(sched_terminate(pid, ctx));
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

    // void nice(pid_t pid, int x);
    case SYS_NICE: {
      pid_t pid = ctx->gpr[0];
      int prio = ctx->gpr[1];

      sched_nice(pid, prio);
      break;
    }

    // unknown/unsupported
    default: {
      break;
    }
  }

  return;
}

void terminate_mem_fault_process(ctx_t* ctx) {
  kernel_write_error("'SEGV': Terminating faulting process.\n", 30);

  restart_on_fatal(sched_terminate(executing_process(), ctx));
  return;
}

/**
 * If memory faults happen, we know it is a process's fault because the
 * kernel can read everywhere (cf. page table), except for the text page.
 * We therefore assume the current process tries to access forbidden memory
 * and therefore terminate it.
 */
void hilevel_handler_pab(ctx_t* ctx) {
  kernel_write_error("Pre-fetch abort excpetion.\n", 27);
  terminate_mem_fault_process(ctx);
  return;
}

void hilevel_handler_dab(ctx_t* ctx) {
  kernel_write_error("Data abort excpetion.\n", 22);
  terminate_mem_fault_process(ctx);
  return;
}
