//
// Created by Ishay Hil on 05/11/2019.
//

#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    char row[] = "a b c";
    char a[50], b[50], c[50];

    sscanf(row, "%[^\t]\t%[^\t]\t%[^\t]", a, b, c);

    puts(a);
    puts(b);
    puts(c);
}