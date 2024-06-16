#include "misc.h"
#include "idt/idt.h"
#include "terminal/print.h"

void* isr80h_command0_sum(struct interrupt_frame* frame)
{
    print("isr80h_command0_sum: invoked\n");

    return 0;
}