find_library(libGeoIP REQUIRED)
set(LIBGEOIP_LIBRARIES GeoIP.so)

include_directories(${LIBGEOIP_INCLUDE_DIR})

HHVM_EXTENSION(geoip ext_geoip.cpp)
HHVM_SYSTEMLIB(geoip ext_geoip.php)

target_link_libraries(geoip ${LIBGEOIP_LIBRARIES})

include(./CMake/GeoIP.cmake)
