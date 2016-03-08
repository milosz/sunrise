/* http://blog.sleeplessbeastie.eu/2013/05/21/how-to-determine-the-sunrise-and-sunset-times/
 * milosz at sleeplessbeastie.eu 
 */

#include <stdio.h>

#include "julian.h"
#include "sun.h"

int main()
{
    double jdn; /* Julian Day Number */

    double ln, lw; /* position */
    ln =  54.4;    /* north latitude */
    lw = -18.5;    /* west longitude */

    /* time modifier */
    int gmt = +1;
    int dst = +0; /* it is very simplified as this is only an example */
    int time_mod = gmt + dst;

    int year = 2016, month = 1;
    double ta, l, delta;
    double transit, sunrise, sunset;
    double temp;
    int i;

    printf("\nSunrise and sunset calculations for %d/%d (GMT=%d, DST=%d)\n\n", month, year, gmt, dst);
    printf("%5s %10s %9s %9s %9s\n", "Day", "JDN", "Sunrise", "Transit", "Sunset");

    for (i = julian_jdn(1, month, 2013); i < julian_jdn(1, month + 1, 2013); i++) {
        ta    = sun_earth_true_anomaly(i);
        l     = sun_sun_ecliptic_longitude(ta);
        delta = sun_earth_declination(l);

        printf("%5d %10d ", i + 1 - julian_jdn(1, month, 2013), i);

        sunrise = sun_sun_sunrise(ln, delta, i, lw, ta);
        temp    = (sunrise - (int) sunrise) > 0.5 ? (sunrise - (int) sunrise) - 0.5 : (sunrise - (int) sunrise) + 0.5;
        printf("%6d:%02d ", (int)(temp * 24) + time_mod, (int)(temp * 24 * 60) % 60);

        transit = sun_sun_transit(i, lw, ta);
        temp    = (transit - (int) transit) > 0.5 ? (transit - (int) transit) - 0.5 : (transit - (int) transit) + 0.5;

        printf("%6d:%02d ", (int)(temp * 24) + time_mod, (int)(temp * 24 * 60) % 60);

        sunset  = sun_sun_sunset(ln, delta, i, lw, ta);
        temp    = (sunset - (int) sunset) > 0.5 ? (sunset - (int) sunset) - 0.5 : (sunset - (int) sunset) + 0.5;
        printf("%6d:%02d\n", (int)(temp * 24) + time_mod , (int)(temp * 24 * 60) % 60);
    }

    year = 2016;
    printf("\nAstronomical seasons %d\n\n", year);
    printf("%5s %10s\n", "Day", "JDN");

    for (i = julian_jdn(1, 1, year); i < julian_jdn(1, 1, year + 1); i++) {
        if (sun_is_winter(i) == 0) { /* -1 - no, 0 - last day, 1 - yes */
            printf("%5d %10d %20s\n", i + 1 - julian_jdn(1, 1, year) + 1, i + 1, "first day of spring");
        } else if (sun_is_spring(i) == 0) {
            printf("%5d %10d %20s\n", i + 1 - julian_jdn(1, 1, year) + 1, i + 1, "first day of summer");
        } else if (sun_is_summer(i) == 0) {
            printf("%5d %10d %20s\n", i + 1 - julian_jdn(1, 1, year) + 1, i + 1, "first day of autumn");
        } else if (sun_is_autumn(i) == 0) {
            printf("%5d %10d %20s\n", i + 1 - julian_jdn(1, 1, year) + 1, i + 1, "first day of winter");
        }
    }


    printf("\nDay and night\n\n");

    int date[2][3] = {
        {21, 5, 2013},
        {22, 5, 2013}
    };

    double time[4][2] = {
        { -0.5 , 0  },  /* 00:00 hour */
        { -0.25, 6  },  /* 06:00 hour */
        {  0   , 12 },  /* 12:00 hour */
        {  0.25, 18 }   /* 18:00 hour */
    };

    int day_and_night[360];

    int k, j;
    int h, r, s;


    printf("%17s %12s  %60s\n", "Date & time", "JDN", "Day and night");

    for (j = 0; j < (sizeof(date) / sizeof(date[0])); j++) {
        for (k = 0; k < (sizeof(time) / sizeof(time[0])); k++) {
            jdn = julian_jdn(date[j][0], date[j][1], date[j][2]);
            jdn += time[k][0];

            ta = sun_earth_true_anomaly(jdn);

            printf(" %02d/%02d/%4d %02.0f:00 %12.2f  ", date[j][0], date[j][1], date[j][2], time[k][1], jdn);

            for (i = -180; i <= 180; i++) {
                l     = sun_sun_ecliptic_longitude(ta);
                delta = sun_earth_declination(l);

                s = sun_earth_sideral_time(jdn, -i);
                r = sun_earth_right_ascension(l);
                h = sun_earth_hour_angle(s, r);

                day_and_night[i + 180] =  sun_earth_altitude(h, delta, ln);
            }

            for (i = 0; i < 60; i++) {
                temp = (day_and_night[6 * i] +
                        day_and_night[6 * i + 1] +
                        day_and_night[6 * i + 2] +
                        day_and_night[6 * i + 3] +
                        day_and_night[6 * i + 4] +
                        day_and_night[6 * i + 5]) / 6;
                temp > 0 ? printf("+") : printf("-");
            }

            printf("\n");
        }
    }

    return 0;

}
