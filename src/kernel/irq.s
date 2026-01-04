.globl irq0_handler
.globl irq1_handler
.globl isr0
.extern keyboard_irq_handler
.extern timer_irq_handler
.extern isr_handler

isr0:
    cli
    pushl $0 
    pushl $0 
    jmp isr_common_stub

isr_common_stub:
    pusha
    push %ds
    push %es
    push %fs
    push %gs

    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    push %esp
    call isr_handler
    add $4, %esp

    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa
    add $8, %esp
    iret

irq0_handler:
    cli
    pusha
    push %ds
    push %es
    push %fs
    push %gs

    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    call timer_irq_handler

    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa

    iret

irq1_handler:
    cli
    pusha
    push %ds
    push %es
    push %fs
    push %gs

    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    call keyboard_irq_handler

    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa

    iret
