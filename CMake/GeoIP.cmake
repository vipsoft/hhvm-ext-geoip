INCLUDE(CheckFunctionExists)
INCLUDE(CheckCSourceCompiles)
INCLUDE(CheckCSourceRuns)

# Get full path to the library.
execute_process(
    COMMAND ldconfig -p
    COMMAND awk -- "/libGeoIP[.]/ { print }"
    COMMAND sed -e "s/.*=> //"
    COMMAND head -1
    OUTPUT_VARIABLE GEOIP_LIB_PATH
)

# Get directory name only.
execute_process(
    COMMAND dirname ${GEOIP_LIB_PATH}
    OUTPUT_VARIABLE GEOIP_LIB_DIRECTORY
)

# Strip trailing newline.
string(REPLACE "\n" "" GEOIP_LIB_DIRECTORY ${GEOIP_LIB_DIRECTORY})

# Find library that contains version number in the file name.
execute_process(
    COMMAND find ${GEOIP_LIB_DIRECTORY} -name "libGeoIP.*.*.*.*"
    COMMAND cut -d . -f 2-5
    COMMAND sort
    COMMAND tail -1
    OUTPUT_VARIABLE GEOIP_LIB_SUFFIX
)

# Extract version from library name, e.g., libGeoIP.so.1.4.8.
# Theoretically, this should work on MacOSX if it is named libGeoIP.1.4.8.dylib.
string(REGEX MATCHALL "([0-9]+)" GEOIP_LIB_VERSION ${GEOIP_LIB_SUFFIX})

set(LIBGEOIP_VERSION 0)
foreach(X ${GEOIP_LIB_VERSION})
    math(EXPR LIBGEOIP_VERSION "${LIBGEOIP_VERSION} * 1000 + ${X}")
endforeach()

# Fallback to feature detection
if(${LIBGEOIP_VERSION} EQUAL 0)
    set(CMAKE_REQUIRED_LIBRARIES "GeoIP.so")

    CHECK_FUNCTION_EXISTS("GeoIP_setup_custom_directory" GEOIP_HAS_SETUP_CUSTOM_DIRECTORY)

    if(${GEOIP_HAS_SETUP_CUSTOM_DIRECTORY})
        set(LIBGEOIP_VERSION 1004001)
    endif()

    CHECK_FUNCTION_EXISTS("GeoIP_set_charset" GEOIP_HAS_SET_CHARSET)

    if(${GEOIP_HAS_SET_CHARSET})
        set(LIBGEOIP_VERSION 1004003)
    endif()

    CHECK_FUNCTION_EXISTS("GeoIP_continent_by_id" GEOIP_HAS_CONTINENT_BY_ID)

    if(${GEOIP_HAS_CONTINENT_BY_ID})
        set(LIBGEOIP_VERSION 1004005)
    endif()

    CHECK_FUNCTION_EXISTS("GeoIP_lib_version" GEOIP_HAS_LIB_VERSION)

    if(${GEOIP_HAS_LIB_VERSION})
        set(LIBGEOIP_VERSION 1004007)
    endif()

    CHECK_C_SOURCE_COMPILES("#include <GeoIP.h>
        int main() {
            int i = GEOIP_NETSPEED_EDITION_REV1_V6;
        }" GEOIP_HAS_NETSPEED_V6)

    if(${GEOIP_HAS_NETSPEED_V6})
        set(LIBGEOIP_VERSION 1004008)
    endif()

    CHECK_C_SOURCE_COMPILES("#include <GeoIP.h>
        int main() {
            const char *path = GeoIP_custom_directory;
        }" GEOIP_HAS_CUSTOM_DIRECTORY)

    if(${GEOIP_HAS_CUSTOM_DIRECTORY})
        set(LIBGEOIP_VERSION 1005000)
    endif()
endif()

add_definitions(-DLIBGEOIP_VERSION=${LIBGEOIP_VERSION})
