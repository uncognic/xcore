.globl irq0_handler
.globl irq1_handler
.globl isr0
.globl isr14_handler
.extern keyboard_irq_handler
.extern timer_irq_handler
.extern isr_handler

isr0:
    cli
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

    push $0
    call isr_handler
    add $4, %esp

    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa

    sti
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
isr14_handler:
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

    push %esp
    mov 8(%esp), %eax
    mov %cr2, %edx
    push %edx
    push %eax
    call page_fault_handler
    add $8, %esp

    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa
    sti
    iret