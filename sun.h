#include <math.h>

/* basic math*/
double sun_to_rad(double);
double sun_to_deg(double);
double sun_full_circle(double);

/* basic caclulations */
double sun_earth_mean_anomaly(double);
double sun_earth_equation_of_center(double);
double sun_earth_true_anomaly(double);

/* seasons */
double sun_sun_ecliptic_longitude(double);
int sun_check_season(int, int, int);
int sun_is_spring(int);
int sun_is_summer(int);
int sun_is_autumn(int);
int sun_is_winter(int);

double sun_earth_right_ascension(double);
double sun_earth_declination(double);

double sun_earth_sideral_time(double, double);
double sun_earth_hour_angle(double, double);
double sun_earth_azimuth(double, double, double);
double sun_earth_altitude(double, double, double);

double sun_sun_transit(double, double, double);
double sun_sun_sunrise(double, double, double, double, double);
double sun_sun_sunset(double, double, double, double, double);

