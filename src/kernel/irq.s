global irq1_handler
extern keyboard_irq_handler
extern pic_send_eoi

irq1_handler:
    push ds
    push es
    push fs
    push gs
    pusha

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call keyboard_irq_handler

    push dword 1 
    call pic_send_eoi
    add esp, 4

    popa
    pop gs
    pop fs
    pop es
    pop ds
    sti
    iretd
