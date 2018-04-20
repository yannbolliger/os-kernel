SECTIONS {
  /* assign load address (per  QEMU) */
  .       =     0x70010000;


  /* TEXT PAGE (R-only for all) 0x700 */
  /* place text segment(s)           */
  .text : { kernel/lolevel.o(.text) *(.text .rodata) }


  /* KERNEL DATA/BSS page (R/W for kernel, no access for user) 0x701 */
  .  = 0x70100000;
  /* place data segment(s)           */
  .kernel_data : { kernel/*.o(.data        ) }
  /* place bss  segment(s)           */
  .kernel_bss  : { kernel/*.o(.bss         ) }
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



  /* User DATA/BSS (mainly libc!) page (R/W for all) 0x702 */
  .  = 0x70200000;
  /* place data segment(s)           */
  .data : {                         *(.data        ) }
  /* place bss  segment(s)           */
  .bss  : {                         *(.bss         ) }



  /* USER memory PAGES 0x702 and above */
  .  = 0x70300000;
  mem_lo = .;
  . = 0x90000000;
  mem_hi = .;

}
