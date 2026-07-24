/* The following sites were used to help calculate values used in these tests:
 *   https://www.hhhh.org/wiml/proj/nmeaxor.html
 *   https://andrew.hedges.name/experiments/convert_lat_long/
 *   https://gpsd.gitlab.io/gpsd/NMEA.html
 */

#include "gps.h"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <cstddef>

static constexpr int INVALID_RESULT = 0xFFFF;

TEST_CASE("Init TPV", "[gps]") {
    struct gps_tpv tpv;

    gps_init_tpv(&tpv);

    REQUIRE(tpv.mode == GPS_MODE_UNKNOWN);
    REQUIRE(tpv.altitude == GPS_INVALID_VALUE);
    REQUIRE(tpv.latitude == GPS_INVALID_VALUE);
    REQUIRE(tpv.longitude == GPS_INVALID_VALUE);
    REQUIRE(tpv.track == GPS_INVALID_VALUE);
    REQUIRE(tpv.speed == GPS_INVALID_VALUE);
    REQUIRE(std::string_view(tpv.time) == "0000-00-00T00:00:00.000Z");
    REQUIRE(std::string_view(tpv.talker_id) == "");
}

TEST_CASE("Encode valid message", "[gps][encode]") {
    const char msg[] = "PMTK251,38400";
    char buf[32];
    char *end = NULL;

    end = gps_encode(buf, msg);

    constexpr std::string_view expected("$PMTK251,38400*27\r\n");
    REQUIRE(end != NULL);
    REQUIRE((buf + expected.size()) == end);
    REQUIRE(buf == expected);
}

TEST_CASE("Encode empty message", "[gps][encode]") {
    const char msg[] = "";
    char buf[32];
    char *end = NULL;

    end = gps_encode(buf, msg);

    constexpr std::string_view expected("$*00\r\n");
    REQUIRE(end != NULL);
    REQUIRE((buf + expected.size()) == end);
    REQUIRE(buf == expected);
}

TEST_CASE("Decode valid GGA message", "[gps][decode]") {
    struct gps_tpv tpv;
    int result = INVALID_RESULT;
    char nmea[] = "$GPGGA,172814.0,3723.46587704,N,12202.26957864,W,2,6,1.2,18.893,M,-25.669,M,2.0,0031*4F\r\n";

    gps_init_tpv(&tpv);
    result = gps_decode(&tpv, nmea);

    REQUIRE(result == GPS_OK);
    REQUIRE(std::string_view(tpv.talker_id) == "GP");
    REQUIRE(std::string_view(tpv.time) == "0000-00-00T17:28:14.000Z");
    REQUIRE(tpv.latitude == 37391097);
    REQUIRE(tpv.longitude == -122037826);
    REQUIRE(tpv.altitude == 18893);
}

TEST_CASE("Decode valid GLL message", "[gps][decode]") {
    struct gps_tpv tpv;
    int result = INVALID_RESULT;
    char nmea[] = "$GPGLL,3704.229,N,07647.090,W,153030.311,A*23\r\n";

    gps_init_tpv(&tpv);
    result = gps_decode(&tpv, nmea);

    REQUIRE(result == GPS_OK);
    REQUIRE(std::string_view(tpv.talker_id) == "GP");
    REQUIRE(std::string_view(tpv.time) == "0000-00-00T15:30:30.311Z");
    REQUIRE(tpv.latitude == 37070483);
    REQUIRE(tpv.longitude == -76784833);
}

TEST_CASE("Decode valid GSA message", "[gps][decode]") {
    struct gps_tpv tpv;
    int result = INVALID_RESULT;
    char nmea[] = "$GPGSA,A,3,01,04,07,16,20,,,,,,,,3.6,2.2,2.7*35\r\n";

    gps_init_tpv(&tpv);
    result = gps_decode(&tpv, nmea);

    REQUIRE(result == GPS_OK);
    REQUIRE(std::string_view(tpv.talker_id) == "GP");
    REQUIRE(tpv.mode == GPS_MODE_3D_FIX);
}

TEST_CASE("Decode valid RMC message", "[gps][decode]") {
    struct gps_tpv tpv;
    int result = INVALID_RESULT;
    char nmea[] = "$GPRMC,023044,A,3907.3840,N,12102.4692,W,0.0,156.1,131102,15.3,E,A*37\r\n";

    gps_init_tpv(&tpv);
    result = gps_decode(&tpv, nmea);

    REQUIRE(result == GPS_OK);
    REQUIRE(std::string_view(tpv.talker_id) == "GP");
    REQUIRE(std::string_view(tpv.time) == "2002-11-13T02:30:44.000Z");
    REQUIRE(tpv.latitude == 39123066);
    REQUIRE(tpv.longitude == -121041153);
    REQUIRE(tpv.track == 156100);
    REQUIRE(tpv.speed == 0);
}

TEST_CASE("Decode valid VTG message", "[gps][decode]") {
    struct gps_tpv tpv;
    int result = INVALID_RESULT;
    char nmea[] = "$GPVTG,176.90,T,,M,3.68,N,6.81,K,A*36\r\n";

    gps_init_tpv(&tpv);
    result = gps_decode(&tpv, nmea);

    REQUIRE(result == GPS_OK);
    REQUIRE(std::string_view(tpv.talker_id) == "GP");
    REQUIRE(tpv.track == 176900);
    REQUIRE(tpv.speed == 1891);
}

TEST_CASE("Decode valid ZDA message", "[gps][decode]") {
    struct gps_tpv tpv;
    int result = INVALID_RESULT;
    char nmea[] = "$GPZDA,050306,29,10,2003,,*43\r\n";

    gps_init_tpv(&tpv);
    result = gps_decode(&tpv, nmea);

    REQUIRE(result == GPS_OK);
    REQUIRE(std::string_view(tpv.talker_id) == "GP");
    REQUIRE(std::string_view(tpv.time) == "2003-10-29T05:03:06.000Z");
}

TEST_CASE("Decode empty message", "[gps][decode]") {
    struct gps_tpv tpv;
    int result = INVALID_RESULT;
    char nmea[] = "";

    gps_init_tpv(&tpv);
    result = gps_decode(&tpv, nmea);

    REQUIRE(result == GPS_ERROR_HEAD);
}

TEST_CASE("Decode invalid header", "[gps][decode]") {
    struct gps_tpv tpv;
    int result = INVALID_RESULT;
    char nmea[] = "?GPGGA,092751.000,5321.6802,N,00630.3371,W,1,8,1.03,61.7,M,55.3,M,,*75\r\n";

    gps_init_tpv(&tpv);
    result = gps_decode(&tpv, nmea);

    REQUIRE(result == GPS_ERROR_HEAD);
}

TEST_CASE("Decode invalid footer", "[gps][decode]") {
    struct gps_tpv tpv;
    int result = INVALID_RESULT;
    char nmea[] = "$GPGGA,092751.000,5321.6802,N,00630.3371,W,1,8,1.03,61.7,M,55.3,M,,*75??";

    gps_init_tpv(&tpv);
    result = gps_decode(&tpv, nmea);

    REQUIRE(result == GPS_ERROR_FOOT);
}

TEST_CASE("Decode invalid checksum", "[gps][decode]") {
    struct gps_tpv tpv;
    int result = INVALID_RESULT;
    char nmea[] = "$GPGGA,092751.000,5321.6802,N,00630.3371,W,1,8,1.03,61.7,M,55.3,M,,*??\r\n";

    gps_init_tpv(&tpv);
    result = gps_decode(&tpv, nmea);

    REQUIRE(result == GPS_ERROR_CHECKSUM);
}

TEST_CASE("Decode mismatch checksum", "[gps][decode]") {
    struct gps_tpv tpv;
    int result = INVALID_RESULT;
    char nmea[] = "$GPGGA,092751.000,5321.6802,N,00630.3371,W,1,8,1.03,61.7,M,55.3,M,,*FF\r\n";

    gps_init_tpv(&tpv);
    result = gps_decode(&tpv, nmea);

    REQUIRE(result == GPS_ERROR_CHECKSUM);
}

TEST_CASE("Decode truncated message", "[gps][decode]") {
    struct gps_tpv tpv;
    int result = INVALID_RESULT;
    char nmea[] = "$GPGGA,092751.000,5321.6802,N,0063";

    gps_init_tpv(&tpv);
    result = gps_decode(&tpv, nmea);

    REQUIRE(result == GPS_ERROR_TRUNCATED);
}

TEST_CASE("Decode unsupported message", "[gps][decode]") {
    struct gps_tpv tpv;
    int result = INVALID_RESULT;
    char nmea[] = "$PGRME,15.0,M,22.5,M,15.0,M*1B\r\n";

    gps_init_tpv(&tpv);
    result = gps_decode(&tpv, nmea);

    REQUIRE(result == GPS_ERROR_UNSUPPORTED);
}

TEST_CASE("Error string for OK", "[gps][error]") {
    const char *msg;

    msg = gps_error_string(GPS_OK);

    REQUIRE(std::string_view(msg) == "No error while parsing NMEA");
}

TEST_CASE("Error string for out-of-range value", "[gps][error]") {
    const char *msg;

    msg = gps_error_string(9999);

    REQUIRE(std::string_view(msg) == "Unknown error");
}
