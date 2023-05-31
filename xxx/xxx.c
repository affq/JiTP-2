#include <stdio.h>

#define dlugi long
#define calkowity int
#define znak char
#define zmiennoprzecinkowy float
#define krotki short
#define podwojny double
#define glowny main
#define stala const
#define pusty void
#define struktura struct
#define wyliczenie enum
#define zewnetrzny extern
#define statyczny static
#define automatyczny auto
#define rejestrowy register
#define zwroc return
#define wskaznik *
#define wskaznik_na_wskaznik **
#define wskaznik_na_funkcje (*funkcja)()
#define wskaznik_na_tablice []
#define wskaznik_na_strukture ->
#define wypisz printf
#define wczytaj scanf
#define dla for
#define dopoki while
#define jesli if


//fibonacci
calkowity fibonacci (calkowity n)
{
    jesli (n == 0)
        zwroc 0;
    jesli (n == 1)
        zwroc 1;
    zwroc fibonacci(n-1) + fibonacci(n-2);
}

calkowity glowny(calkowity argc, znak *argv[])
{
    calkowity i;
    dla (i = 0; i < 99999999; i++)
        wypisz("%d\n", fibonacci(i));
    zwroc 0;
}