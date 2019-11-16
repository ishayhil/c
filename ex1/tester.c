//
// Created by Ishay Hil on 05/11/2019.
//

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int howManyChars(int n)
{
    int i = 1;
    while ((n = n / 10))
    {
        i++;
    }
    return i;
}

void decode(const char *text)
{
    int i = 0;
    while (i < (int) strlen(text))
    {
        if (isdigit(text[i]))
        {
            int n = atoi(text + i);
            int ch_cnt = howManyChars(n);
//            printf("%d %d\n", n, ch_cnt);
            for (int k = 0; k < n; ++k)
            {
                printf("%c", text[ch_cnt + i]);
            }
            i += ch_cnt + 1;

        }
        else
        {
            printf("%c", text[i]);
            i += 1;
        }
    }
}



int main(int argc, char *argv[])
{
    decode("1a2b3c50d");
}