#include "gps.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

using Catch::Benchmark::Chronometer;

TEST_CASE("Benchmark decoding NMEA 0183 sentence", "[!benchmark]") {
    struct gps_tpv tpv;

    BENCHMARK_ADVANCED("Decode GGA sentence")(Chronometer meter) {
        char nmea[] = "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47\r\n";
        gps_init_tpv(&tpv);
        meter.measure([&tpv, &nmea] { return gps_decode(&tpv, nmea); });
    };

    BENCHMARK_ADVANCED("Decode GSA sentence")(Chronometer meter) {
        char nmea[] = "$GPGSA,A,3,04,05,,09,12,,,24,,,,,2.5,1.3,2.1*39\r\n";
        gps_init_tpv(&tpv);
        meter.measure([&tpv, &nmea] { return gps_decode(&tpv, nmea); });
    };

    BENCHMARK_ADVANCED("Decode RMC sentence")(Chronometer meter) {
        char nmea[] = "$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A\r\n";
        gps_init_tpv(&tpv);
        meter.measure([&tpv, &nmea] { return gps_decode(&tpv, nmea); });
    };

    BENCHMARK_ADVANCED("Decode GLL sentence")(Chronometer meter) {
        char nmea[] = "$GPGLL,4916.45,N,12311.12,W,225444,A,*1D\r\n";
        gps_init_tpv(&tpv);
        meter.measure([&tpv, &nmea] { return gps_decode(&tpv, nmea); });
    };

    BENCHMARK_ADVANCED("Decode VTG sentence")(Chronometer meter) {
        char nmea[] = "$GPVTG,054.7,T,034.4,M,005.5,N,010.2,K*48\r\n";
        gps_init_tpv(&tpv);
        meter.measure([&tpv, &nmea] { return gps_decode(&tpv, nmea); });
    };

    BENCHMARK_ADVANCED("Decode ZDA sentence")(Chronometer meter) {
        char nmea[] = "$GPZDA,201530.00,04,07,2002,00,00*60\r\n";
        gps_init_tpv(&tpv);
        meter.measure([&tpv, &nmea] { return gps_decode(&tpv, nmea); });
    };
}
