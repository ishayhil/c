//
// Created by Ishay Hil on 05/11/2019.
//

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>


void reverseInput() {
    int num;
    scanf("%d", &num);
    int tempNum = num;
    int log10 = 0;
    while (tempNum != 0) { // log
        log10++;
        tempNum /= 10;
    }

    int newNum = 0;
    while (log10 >= 0) {
        int d = num % 10;
        int power = 1;
        for (int i = 0; i < log10 - 1; ++i) { // power
            power *= 10;
        }
        newNum += d * power;
        log10--;
        num /= 10;
    }
    printf("%d\n", newNum);
}

int main(int argc, char *argv[]) {
    printf("%d", strcmp("12", "9"));
}

// 1222/10 = 122.2 / 10 = 12.22 / 10 = 1.222 / 10 = 0
//