#include "screen.h"
#include "scheduler.h"

void processA();
void processB();
void processC();
void processD();

void kernel_main() {
    process_t p1, p2, p3, p4;

    screen_init();
    process_init();
    scheduler_init();
    print("Welcome to 539kernel!");
    println();
    print("We are now in Protected-mode");
    println();
    printi(539);
    println();

    process_create(&processA, &p1);
    process_create(&processB, &p2);
    process_create(&processC, &p3);
    process_create(&processD, &p4);

    while (1)
        ;
}

void print(char *str) {
    int currCharLocationInVidMem, currColorLocationInVidMem;

    while (*str != '\0') {
        currCharLocationInVidMem = nextTextPos * 2;
        currColorLocationInVidMem = currCharLocationInVidMem + 1;

        video[currCharLocationInVidMem] = *str;
        video[currColorLocationInVidMem] = 15;

        nextTextPos++;

        str++;
    }
}

void println() { nextTextPos = ++currLine * 80; }

void printi(int number) {
    char *digitToStr[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

    if (number >= 0 && number <= 9) {
        print(digitToStr[number]);
        return;
    } else {
        int remaining = number % 10;
        number = number / 10;

        printi(number);
        printi(remaining);
    }
}

void interrupt_handler(int interrupt_number) {
    println();
    print("Interrupt Received");
    printi(interrupt_number);
}

void processA() {
    print("Process A,");

    while (1) {
        asm("mov $5390, %eax");
    }
}

void processB() {
    print("Process B,");

    while (1) {
        asm("mov $5391, %eax");
    }
}

void processC() {
    print("Process C,");

    while (1) {
        asm("mov $5392, %eax");
    }
}

void processD() {
    print("Process D,");

    while (1) {
        asm("mov $5393, %eax");
    }
}
