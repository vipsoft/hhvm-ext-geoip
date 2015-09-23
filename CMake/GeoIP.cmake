INCLUDE(CheckLibraryExists)
INCLUDE(CheckFunctionExists)
INCLUDE(CheckCSourceCompiles)
INCLUDE(CheckCSourceRuns)

set(LIBGEOIP_VERSION 0)

CHECK_LIBRARY_EXISTS(GeoIP GeoIP_open /usr/lib HAVE_LIBGEOIP)

if(${HAVE_LIBGEOIP})
    # Find library that contains version number in the file name.
    execute_process(
        COMMAND find /usr/lib -name "libGeoIP.*.*.*.*"
        COMMAND cut -d . -f 2-5
        COMMAND sort
        COMMAND tail -1
        OUTPUT_VARIABLE GEOIP_LIB_SUFFIX
    )

    if(${GEOIP_LIB_SUFFIX})
        # Extract version from library name, e.g., libGeoIP.so.1.4.8.
        string(REGEX MATCHALL "([0-9]+)" GEOIP_LIB_VERSION ${GEOIP_LIB_SUFFIX})

        foreach(X ${GEOIP_LIB_VERSION})
            math(EXPR LIBGEOIP_VERSION "${LIBGEOIP_VERSION} * 1000 + ${X}")
        endforeach()
    endif()
endif()

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

    CHECK_FUNCTION_EXISTS("GeoIP_addr_to_num" GEOIP_HAS_ADDR_TO_NUM)

    if(${GEOIP_HAS_ADDR_TO_NUM})
        set(LIBGEOIP_VERSION 1004007)
    endif()

    CHECK_C_SOURCE_COMPILES("#include <GeoIP.h>
        int main() {
            int i = GEOIP_NETSPEED_EDITION_REV1;
        }" GEOIP_HAS_NETSPEED_EDITION_REV1)

    if(${GEOIP_HAS_NETSPEED_EDITION_REV1})
        set(LIBGEOIP_VERSION 1004008)
    endif()

    CHECK_C_SOURCE_COMPILES("#include <GeoIP.h>
        int main() {
            int i = GEOIP_ACCURACYRADIUS_EDITION;
        }" GEOIP_HAS_ACCURACYRADIUS_EDITION)

    if(${GEOIP_HAS_ACCURACYRADIUS_EDITION})
        set(LIBGEOIP_VERSION 1005000)
    endif()
endif()

add_definitions(-DLIBGEOIP_VERSION=${LIBGEOIP_VERSION})
