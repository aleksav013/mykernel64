.section .text

.global load_pt_lvl4
load_pt_lvl4:
    push %rbp
    mov %rsp, %rbp

    mov %rdi, %cr3

    mov %rbp, %rsp
    pop %rbp
    ret
