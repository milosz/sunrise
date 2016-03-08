
/* convert Gregorian calendar date to Julian Day Number */
/* source: http://en.wikipedia.org/wiki/Julian_day      */
int julian_jdn(int day, int month, int year)
{
    int a, y, m;

    a = (14 - month) / 12;
    y = year + 4800 - a;
    m = month + 12 * a - 3;
    return (day + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 - 32045);
}

