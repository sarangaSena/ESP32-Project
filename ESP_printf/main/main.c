/*
 * main.c
 *
 * Created: 5/6/2024 12:51:35 PM
 * Author : Saranga Senarathna
 */ 

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>


void app_main(void) {
    // Print a message to the console
    printf("Hello, ESP32!\n");

    // Print an integer
    int value = 42;
    printf("The value is: %d\n", value);

    // Print a float with 2 decimal places
    float temperature = 23.45;
    printf("The temperature is: %.2f°C\n", temperature);
}


/* Format Specifiers:
Format specifiers are used within the format string to tell printf how to interpret and display the corresponding argument. Here are some commonly used format specifiers:

Specifier	Description
%d	Signed decimal integer
%u	Unsigned decimal integer
%f	Floating-point number
%s	String
%c	Single character
%x	Unsigned hexadecimal integer (lowercase)
%X	Unsigned hexadecimal integer (uppercase)
%p	Pointer (memory address)
%%	Literal % character */