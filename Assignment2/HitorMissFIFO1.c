#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

int main()
{
    int frames;
    int page;
    scanf("%d\n", &frames);
    int framearray[frames];
    char input;

    for (int i = 0; i < frames; i++)
    {
        framearray[i] = -1;
    }
    int pagefaults = 0;
    int index = 0; // Where to insert the next page
    scanf("%c", &input);
    while (input != '\n')
    {
        if (input == ' ')
        {
            scanf("%c", &input);
            continue;
        }
        // Clock algorithm implementation
        int page = input - '0';
        int flag = 0;
        for (int i = 0; i < frames; i++)
        {
            if (framearray[i] == page)
            {
                flag = 1;
                index = (index + 1) % frames; // in clock if you
                break;
            }
        }
        if (flag == 0)
        {
            framearray[index] = page;
            index = (index + 1) % frames;
            pagefaults++;
        }
        scanf("%c", &input);
    }

    printf("%d\n", pagefaults);

    return 0;
}
