/* source: http://aa.quae.nl/en/reken/zonpositie.html 
 * milosz at sleeplessbeastie.eu 
 */

#include "sun.h"

/* convert degrees to radians */
double sun_to_rad(double angle)
{
    return (M_PI * angle / 180);
}

/* convert radians to degrees */
double sun_to_deg(double angle)
{
    return (180 * angle / M_PI);
}

/* draw a full circle (370' to (370'- 360') = 10') */
double sun_full_circle(double angle)
{
    return (angle - (int)(angle / 360) * 360);
}

/* Earth - mean anomaly */
double sun_earth_mean_anomaly(double jdn)
{
    return sun_full_circle(357.5291 + 0.98560028 * (jdn - 2451545));
}

/* Earth - the equation of center */
double sun_earth_equation_of_center(double mean_anomaly)
{
    return 1.9148 * sin(sun_to_rad(mean_anomaly)) +
           0.0200 * sin(2 * sun_to_rad(mean_anomaly)) +
           0.0003 * sin(3 * sun_to_rad(mean_anomaly));
}

/* Earth - true anomaly */
double sun_earth_true_anomaly(double jdn)
{
    return sun_earth_mean_anomaly(jdn) +
           sun_earth_equation_of_center(sun_earth_mean_anomaly(jdn));
}

/* Ecliptic longitude of the sun */
double sun_sun_ecliptic_longitude(double true_anomaly)
{
    return sun_full_circle(true_anomaly + 102.9372 + 180);
}

/* check for specific season using ecliptic longitude of the sun*/
/* for northern hemisphere */
/* 0   - 90    spring */
/* 90  - 180   summer */
/* 180 - 270   autumn */
/* 270 - 360   winter */
int sun_check_season(int jdn, int a, int b)
{
    double el_curr = sun_sun_ecliptic_longitude(sun_earth_true_anomaly(jdn));
    double el_prev = sun_sun_ecliptic_longitude(sun_earth_true_anomaly(jdn - 1));


    if ((el_prev <= b && el_curr >= b) || (b == 360 && el_prev >= a && el_prev <= b && el_curr >= 0 && el_curr < a))
        return 0;
    else if ((el_prev >= a && el_prev < b) && (el_prev >= a && el_prev < b))
        return 1;
    else
        return -1;
}

/* check if JDN is spring */
/*  1 - true              */
/*  0 - true, last day    */
/* -1 - false             */
int sun_is_spring(int jdn)
{
    return sun_check_season(jdn, 0, 90);
}

/* check if JDN is summer */
/*  1 - true              */
/*  0 - true, last day    */
/* -1 - false             */
int sun_is_summer(int jdn)
{
    return sun_check_season(jdn, 90, 180);
}


/* check if JDN is autumn */
/*  1 - true              */
/*  0 - true, last day    */
/* -1 - false             */
int sun_is_autumn(int jdn)
{
    return sun_check_season(jdn, 180, 270);
}


/* check if JDN is spring */
/*  1 - true              */
/*  0 - true, last day    */
/* -1 - false             */
int sun_is_winter(int jdn)
{
    return sun_check_season(jdn, 270, 360);
}

/* Earth - right ascension */
double sun_earth_right_ascension(double ecliptic_longitude)
{
    return sun_to_deg(
               atan2(
                   sin(sun_to_rad(ecliptic_longitude)) * cos(sun_to_rad(23.45)),
                   cos(sun_to_rad(ecliptic_longitude))
               )
           );
}

/* Earth - declination */
double sun_earth_declination(double ecliptic_longitude)
{
    return sun_to_deg(
               asin(
                   sin(sun_to_rad(ecliptic_longitude)) * sin(sun_to_rad(23.45))
               )
           );
}

/* Earth - sidereal time */
double sun_earth_sideral_time(double jdn, double lw)
{
    return sun_full_circle(
               280.1600 + 360.9856235 * (jdn - 2451545) - lw
           );
}

/* Earth - hour angle */
double sun_earth_hour_angle(double sideral_time, double right_ascension)
{
    return sideral_time - right_ascension;
}

/* Earth - azimuth */
double sun_earth_azimuth(double hour_angle, double declination,  double ln)
{
    return sun_full_circle(
               sun_to_deg(
                   atan2(
                       sin(sun_to_rad(hour_angle)),
                       cos(sun_to_rad(hour_angle)) * sin(sun_to_rad(ln)) -
                       tan(sun_to_rad(declination)) * cos(sun_to_rad(ln))
                   )
               )
           );
}

/* Earth - altitude */
double sun_earth_altitude(double hour_angle, double declination, double ln)
{
    return sun_full_circle(
               sun_to_deg(
                   asin(
                       sin(sun_to_rad(ln)) * sin(sun_to_rad(declination)) +
                       cos(sun_to_rad(ln)) * cos(sun_to_rad(declination)) *
                       cos(sun_to_rad(hour_angle))
                   )
               )
           );
}

/* Sun -  transtit */
double sun_sun_transit(double jdn, double lw, double mean_anomaly)
{
    return  2451545 + 0.0009 + (lw / 360) +
            (int)((jdn - 2451545 - 0.0009) / 1 - lw / 360) +
            0.0053 * sin(sun_to_rad(mean_anomaly)) -
            0.0069 * sin(sun_to_rad(2 * sun_full_circle(mean_anomaly + 102.9372 + 180)));
}

/* Sun - sunrise */
double sun_sun_sunrise(double ln, double declination, double jdn, double lw, double mean_anomaly)
{
    double h;

    h = -sun_full_circle(
            sun_to_deg(
                acos(
                    (
                        sin(sun_to_rad(-0.83)) -
                        sin(sun_to_rad(ln)) * sin(sun_to_rad(declination))
                    ) / (
                        cos(sun_to_rad(ln)) * cos(sun_to_rad(declination))
                    )
                )
            )
        );

    return 2451545 + 0.0009 + (h + lw) / 360 +
           (int)((jdn - 2451545 - 0.0009) / 1 - (h + lw) / 360) +
           0.0053 * sin(sun_to_rad(mean_anomaly)) -
           0.0069 * sin(sun_to_rad(2 * sun_full_circle(mean_anomaly + 102.9372 + 180)));

}

/* Sun - sunset */
double sun_sun_sunset(double ln, double declination, double jdn, double lw, double mean_anomaly)
{
    double h;

    h = sun_full_circle(
            sun_to_deg(
                acos(
                    (
                        sin(sun_to_rad(-0.83)) -
                        sin(sun_to_rad(ln)) * sin(sun_to_rad(declination))
                    ) / (
                        cos(sun_to_rad(ln)) * cos(sun_to_rad(declination))
                    )
                )
            )
        );

    return 2 + 2451545 + 0.0009 + (h + lw) / 360 +
           (int)((jdn - 2451545 - 0.0009) / 1 - (h + lw) / 360) +
           0.0053 * sin(sun_to_rad(mean_anomaly)) -
           0.0069 * sin(sun_to_rad(2 * sun_full_circle(mean_anomaly + 102.9372 + 180)));
}
