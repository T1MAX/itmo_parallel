#include <stdio.h>

bool is_sep(char sym)
{
    return sym == ' ' || sym == '\n' || sym == '\t';
}

int main(int argc, char **argv)
{
    int j = 0, sum = 0;
    char last_sym = 'a';
    while(argv[1][j] != '\0')
    {
        if(is_sep(argv[1][j]) && !is_sep(last_sym))
            sum++;
        last_sym = argv[1][j];
        j++;
    }
    if(!is_sep(last_sym))
        sum++;
	printf("%d\n", sum);
}


