SECTIONS {
  /* assign load address (per  QEMU) */
  .       =     0x70010000;


  /* TEXT PAGE (R-only for all) 0x700 */
  /* place text segment(s)           */
  .text : { kernel/lolevel.o(.text) *(.text .rodata) }


  /* KERNEL DATA/BSS page (R/W for kernel, R-only for user) 0x701 */
  .  = 0x70100000;
  /* place data segment(s)           */
  .data : {                         *(.data        ) }
  /* place bss  segment(s)           */
  .bss  : {                         *(.bss         ) }
  /* align       address (per AAPCS) */
  .       = ALIGN( 8 );
  /* allocate stack for irq mode     */
  .       = . + 0x00001000;
  tos_irq = .;
  /* allocate stack for abt mode     */
  .       = . + 0x00001000;
  tos_abt = .;
  /* allocate stack for svc mode     */
  .       = . + 0x00001000;
  tos_svc = .;


  /* USER memory PAGES 0x702 and above */
  end_of_fixed_mem = .;
  .  = 0x70200000;
  mem_lo = .;
  . = 0x90000000;
  mem_hi = .;

  ASSERT((end_of_fixed_mem < mem_lo), "Error: Too little text/data/bss memory")
}
