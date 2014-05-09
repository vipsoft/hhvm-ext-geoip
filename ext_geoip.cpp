/*
   GeoIP extension for HHVM.

   +----------------------------------------------------------------------+
   | PHP Version 5                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997-2005 The PHP Group                                |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.0 of the PHP license,       |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_0.txt.                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Authors: Matthew Fonda, Olivier Hill, Anthon Pang                    |
   +----------------------------------------------------------------------+
*/

#include "hphp/runtime/base/base-includes.h"
#include <cinttypes>
#include <GeoIP.h>
#include <GeoIPCity.h>
#include "GeoIP_internal.h"

namespace HPHP {

// Database types recognized by geoip_database_info() geoip_db_avail(), and geoip_db_filename()
const int64_t k_GEOIP_COUNTRY_EDITION = GEOIP_COUNTRY_EDITION;
const StaticString s_GEOIP_COUNTRY_EDITION("GEOIP_COUNTRY_EDITION");
const int64_t k_GEOIP_REGION_EDITION_REV0 = GEOIP_REGION_EDITION_REV0;
const StaticString s_GEOIP_REGION_EDITION_REV0("GEOIP_REGION_EDITION_REV0");
const int64_t k_GEOIP_CITY_EDITION_REV0 = GEOIP_CITY_EDITION_REV0;
const StaticString s_GEOIP_CITY_EDITION_REV0("GEOIP_CITY_EDITION_REV0");
const int64_t k_GEOIP_ORG_EDITION = GEOIP_ORG_EDITION;
const StaticString s_GEOIP_ORG_EDITION("GEOIP_ORG_EDITION");
const int64_t k_GEOIP_ISP_EDITION = GEOIP_ISP_EDITION;
const StaticString s_GEOIP_ISP_EDITION("GEOIP_ISP_EDITION");
const int64_t k_GEOIP_CITY_EDITION_REV1 = GEOIP_CITY_EDITION_REV1;
const StaticString s_GEOIP_CITY_EDITION_REV1("GEOIP_CITY_EDITION_REV1");
const int64_t k_GEOIP_REGION_EDITION_REV1 = GEOIP_REGION_EDITION_REV1;
const StaticString s_GEOIP_REGION_EDITION_REV1("GEOIP_REGION_EDITION_REV1");
const int64_t k_GEOIP_PROXY_EDITION = GEOIP_PROXY_EDITION;
const StaticString s_GEOIP_PROXY_EDITION("GEOIP_PROXY_EDITION");
const int64_t k_GEOIP_ASNUM_EDITION = GEOIP_ASNUM_EDITION;
const StaticString s_GEOIP_ASNUM_EDITION("GEOIP_ASNUM_EDITION");
const int64_t k_GEOIP_NETSPEED_EDITION = GEOIP_NETSPEED_EDITION;
const StaticString s_GEOIP_NETSPEED_EDITION("GEOIP_NETSPEED_EDITION");
const int64_t k_GEOIP_NETSPEED_EDITION_REV1 = GEOIP_NETSPEED_EDITION_REV1;
const StaticString s_GEOIP_NETSPEED_EDITION_REV1("GEOIP_NETSPEED_EDITION_REV1");
const int64_t k_GEOIP_DOMAIN_EDITION = GEOIP_DOMAIN_EDITION;
const StaticString s_GEOIP_DOMAIN_EDITION("GEOIP_DOMAIN_EDITION");

// Internet connection speed constants returned by geoip_id_by_name()
const int64_t k_GEOIP_UNKNOWN_SPEED = GEOIP_UNKNOWN_SPEED;
const StaticString s_GEOIP_UNKNOWN_SPEED("GEOIP_UNKNOWN_SPEED");
const int64_t k_GEOIP_DIALUP_SPEED = GEOIP_DIALUP_SPEED;
const StaticString s_GEOIP_DIALUP_SPEED("GEOIP_DIALUP_SPEED");
const int64_t k_GEOIP_CABLEDSL_SPEED = GEOIP_CABLEDSL_SPEED;
const StaticString s_GEOIP_CABLEDSL_SPEED("GEOIP_CABLEDSL_SPEED");
const int64_t k_GEOIP_CORPORATE_SPEED = GEOIP_CORPORATE_SPEED;
const StaticString s_GEOIP_CORPORATE_SPEED("GEOIP_CORPORATE_SPEED");

static Variant HHVM_FUNCTION(geoip_asnum_by_name, const String& hostname) {
    GeoIP *gi;
    char *asnum;

    if (GeoIP_db_avail(GEOIP_ASNUM_EDITION)) {
        gi = GeoIP_open_type(GEOIP_ASNUM_EDITION, GEOIP_STANDARD);
    } else {
        raise_warning("geoip_asnum_by_name(): Required database not available at %s.", GeoIPDBFileName[GEOIP_ASNUM_EDITION]);

        return null_variant;
    }

    asnum = GeoIP_name_by_name(gi, hostname.c_str());

    GeoIP_delete(gi);

    if (asnum == NULL) {
        return Variant(false);
    }

    Variant value = Variant(String(asnum));

    free(asnum);

    return value;
}

static Variant HHVM_FUNCTION(geoip_continent_code_by_name, const String& hostname) {
    GeoIP *gi;
    int id;

    if (GeoIP_db_avail(GEOIP_COUNTRY_EDITION)) {
        gi = GeoIP_open_type(GEOIP_COUNTRY_EDITION, GEOIP_STANDARD);
    } else {
        raise_warning("geoip_continent_code_by_name(): Required database not available at %s.", GeoIPDBFileName[GEOIP_COUNTRY_EDITION]);
    }

    id = GeoIP_id_by_name(gi, hostname.c_str());
    GeoIP_delete(gi);

    if (id == 0) {
        return Variant(false);
    }

    return Variant(String(GeoIP_country_continent[id]));
}

static Variant HHVM_FUNCTION(geoip_country_code_by_name, const String& hostname) {
    GeoIP *gi;
    const char *country_code;

    if (GeoIP_db_avail(GEOIP_COUNTRY_EDITION)) {
        gi = GeoIP_open_type(GEOIP_COUNTRY_EDITION, GEOIP_STANDARD);
    } else {
        raise_warning("geoip_country_code_by_name(): Required database not available at %s.", GeoIPDBFileName[GEOIP_COUNTRY_EDITION]);
    }

    country_code = GeoIP_country_code_by_name(gi, hostname.c_str());
    GeoIP_delete(gi);

    if (country_code == NULL) {
        return Variant(false);
    }

    return Variant(String(country_code));
}

static Variant HHVM_FUNCTION(geoip_country_code3_by_name, const String& hostname) {
    GeoIP *gi;
    const char *country_code3;

    if (GeoIP_db_avail(GEOIP_COUNTRY_EDITION)) {
        gi = GeoIP_open_type(GEOIP_COUNTRY_EDITION, GEOIP_STANDARD);
    } else {
        raise_warning("geoip_country_code3_by_name(): Required database not available at %s.", GeoIPDBFileName[GEOIP_COUNTRY_EDITION]);
    }

    country_code3 = GeoIP_country_code3_by_name(gi, hostname.c_str());
    GeoIP_delete(gi);

    if (country_code3 == NULL) {
        return Variant(false);
    }

    return Variant(String(country_code3));
}

static Variant HHVM_FUNCTION(geoip_country_name_by_name, const String& hostname) {
    GeoIP *gi;
    const char *country_name;

    if (GeoIP_db_avail(GEOIP_COUNTRY_EDITION)) {
        gi = GeoIP_open_type(GEOIP_COUNTRY_EDITION, GEOIP_STANDARD);
    } else {
        raise_warning("geoip_country_name_by_name(): Required database not available at %s.", GeoIPDBFileName[GEOIP_COUNTRY_EDITION]);
    }

    country_name = GeoIP_country_name_by_name(gi, hostname.c_str());
    GeoIP_delete(gi);

    if (country_name == NULL) {
        return Variant(false);
    }

    return Variant(String(country_name));
}

static Variant HHVM_FUNCTION(geoip_database_info, int64_t database /* = GEOIP_COUNTRY_EDITION */) {
    GeoIP *gi;
    char *db_info;

    if (database < 0 || database >= NUM_DB_TYPES) {
        raise_warning("geoip_database_info(): Database type given is out of bound.");

        return null_variant;
    }

    if ( ! GeoIP_db_avail(database)) {
        if (NULL != GeoIPDBFileName[database]) {
            raise_warning("geoip_database_info(): Required database not available at %s.", GeoIPDBFileName[GEOIP_COUNTRY_EDITION]);
        } else {
            raise_warning("geoip_database_info(): Required database not available.");
        }

        return null_variant;
    }

    gi = GeoIP_open_type(database, GEOIP_STANDARD);
    db_info = GeoIP_database_info(gi);
    GeoIP_delete(gi);

    Variant value = Variant(String(db_info));

    free(db_info);

    return value;
}

static Variant HHVM_FUNCTION(geoip_db_avail, int64_t database) {
    if (database < 0 || database >= NUM_DB_TYPES) {
        raise_warning("geoip_db_avail(): Database type given is out of bound.");

        return null_variant;
    }

    return Variant((bool) GeoIP_db_avail(database));
}

static Variant HHVM_FUNCTION(geoip_db_filename, int64_t database) {
    const char *filename;

    if (database < 0 || database >= NUM_DB_TYPES) {
        raise_warning("geoip_db_filename(): Database type given is out of bound.");

        return null_variant;
    }

    filename = GeoIPDBFileName[database];

    if (filename == NULL) {
        return null_variant;
    }

    return Variant(String(filename));
}

static Array HHVM_FUNCTION(geoip_db_get_all_info) {
    Array info = Array::Create();

    for (int i = 0; i < NUM_DB_TYPES; i++) {
        if (NULL != GeoIPDBDescription[i]) {
            Array row = Array::Create();

            row.add(String("available"), Variant((bool) GeoIP_db_avail(i)));

            if (GeoIPDBDescription[i]) {
                row.add(String("description"), Variant(String(GeoIPDBDescription[i])));
            }

            if (GeoIPDBFileName[i]) {
                row.add(String("filename"), Variant(String(GeoIPDBFileName[i])));
            }

            info.set(i, Variant(row));
        }
    }

    return info;
}

static Variant HHVM_FUNCTION(geoip_domain_by_name, const String& hostname) {
    GeoIP *gi;
    char *domain;

    if (GeoIP_db_avail(GEOIP_DOMAIN_EDITION)) {
        gi = GeoIP_open_type(GEOIP_DOMAIN_EDITION, GEOIP_STANDARD);
    } else {
        raise_warning("geoip_domain_by_name(): Required database not available at %s.", GeoIPDBFileName[GEOIP_DOMAIN_EDITION]);

        return null_variant;
    }

    domain = GeoIP_name_by_name(gi, hostname.c_str());

    GeoIP_delete(gi);

    if (domain == NULL) {
        return Variant(false);
    }

    Variant value = Variant(String(domain));

    free(domain);

    return value;
}

static Variant HHVM_FUNCTION(geoip_id_by_name, const String& hostname) {
    GeoIP *gi;
    int netspeed;

    if (GeoIP_db_avail(GEOIP_NETSPEED_EDITION)) {
        gi = GeoIP_open_type(GEOIP_NETSPEED_EDITION, GEOIP_STANDARD);
    } else {
        raise_warning("geoip_id_by_name(): Required database not available at %s.", GeoIPDBFileName[GEOIP_NETSPEED_EDITION]);

        return null_variant;
    }

    netspeed = GeoIP_id_by_name(gi, hostname.c_str());

    GeoIP_delete(gi);

    return Variant((uint64_t) netspeed);
}

static Variant HHVM_FUNCTION(geoip_isp_by_name, const String& hostname) {
    GeoIP *gi;
    char *isp;

    if (GeoIP_db_avail(GEOIP_ISP_EDITION)) {
        gi = GeoIP_open_type(GEOIP_ISP_EDITION, GEOIP_STANDARD);
    } else {
        raise_warning("geoip_isp_by_name(): Required database not available at %s.", GeoIPDBFileName[GEOIP_ISP_EDITION]);

        return null_variant;
    }

    isp = GeoIP_name_by_name(gi, hostname.c_str());

    GeoIP_delete(gi);

    if (isp == NULL) {
        return Variant(false);
    }

    Variant value = Variant(String(isp));

    free(isp);

    return value;
}

#if LIBGEOIP_VERSION >= 1004008
static Variant HHVM_FUNCTION(geoip_netspeedcell_by_name, const String& hostname) {
    GeoIP *gi;
    char *netspeedcell;

    if (GeoIP_db_avail(GEOIP_NETSPEED_EDITION_REV1)) {
        gi = GeoIP_open_type(GEOIP_NETSPEED_EDITION_REV1, GEOIP_STANDARD);
    } else {
        raise_warning("geoip_netspeedcell_by_name(): Required database not available at %s.", GeoIPDBFileName[GEOIP_NETSPEED_EDITION_REV1]);

        return null_variant;
    }

    netspeedcell = GeoIP_name_by_name(gi, hostname.c_str());

    GeoIP_delete(gi);

    if (netspeedcell == NULL) {
        return Variant(false);
    }

    Variant value = Variant(String(netspeedcell));

    free(netspeedcell);

    return value;
}
#endif

static Variant HHVM_FUNCTION(geoip_org_by_name, const String& hostname) {
    GeoIP *gi;
    char *org;

    if (GeoIP_db_avail(GEOIP_ORG_EDITION)) {
        gi = GeoIP_open_type(GEOIP_ORG_EDITION, GEOIP_STANDARD);
    } else {
        raise_warning("geoip_org_by_name(): Required database not available at %s.", GeoIPDBFileName[GEOIP_ORG_EDITION]);

        return null_variant;
    }

    org = GeoIP_name_by_name(gi, hostname.c_str());

    GeoIP_delete(gi);

    if (org == NULL) {
        return Variant(false);
    }

    Variant value = Variant(String(org));

    free(org);

    return value;
}

static Variant HHVM_FUNCTION(geoip_record_by_name, const String& hostname) {
    GeoIP *gi;
    GeoIPRecord *gi_record;

    if (GeoIP_db_avail(GEOIP_CITY_EDITION_REV0) || GeoIP_db_avail(GEOIP_CITY_EDITION_REV1)) {
        gi = GeoIP_open_type(GEOIP_CITY_EDITION_REV1, GEOIP_STANDARD);

        if (NULL == gi) {
            gi = GeoIP_open_type(GEOIP_CITY_EDITION_REV0, GEOIP_STANDARD);
        }
    } else {
        raise_warning("geoip_record_by_name(): Required database not available at %s.", GeoIPDBFileName[GEOIP_CITY_EDITION_REV0]);

        return null_variant;
    }

    gi_record = GeoIP_record_by_name(gi, hostname.c_str());

    GeoIP_delete(gi);

    if (NULL == gi_record) {
        return Variant(false);
    }

    Array record = Array::Create();

#if LIBGEOIP_VERSION >= 1004003
    record.add(String("continent_code"), Variant(String((gi_record->continent_code == NULL) ? "" : gi_record->continent_code)));
#endif
    record.add(String("country_code"), Variant(String((gi_record->country_code == NULL) ? "" : gi_record->country_code)));
    record.add(String("country_code3"), Variant(String((gi_record->country_code3 == NULL) ? "" : gi_record->country_code3)));
    record.add(String("country_name"), Variant(String((gi_record->country_name == NULL) ? "" : gi_record->country_name)));
    record.add(String("region"), Variant(String((gi_record->region == NULL) ? "" : gi_record->region)));
    record.add(String("city"), Variant(String((gi_record->city == NULL) ? "" : gi_record->city)));
    record.add(String("postal_code"), Variant(String((gi_record->postal_code == NULL) ? "" : gi_record->postal_code)));
    record.add(String("latitude"), Variant((double) gi_record->latitude));
    record.add(String("longitude"), Variant((double) gi_record->longitude));
#if LIBGEOIP_VERSION >= 1004005
    record.add(String("dma_code"), Variant((int64_t) gi_record->metro_code));
#else
    record.add(String("dma_code"), Variant((int64_t) gi_record->dma_code));
#endif
    record.add(String("area_code"), Variant((int64_t) gi_record->area_code));

    GeoIPRecord_delete(gi_record);

    return Variant(record);
}

static Variant HHVM_FUNCTION(geoip_region_by_name, const String& hostname) {
    GeoIP *gi;
    GeoIPRegion *gi_region;

    if (GeoIP_db_avail(GEOIP_REGION_EDITION_REV0) || GeoIP_db_avail(GEOIP_REGION_EDITION_REV1)) {
        gi = GeoIP_open_type(GEOIP_REGION_EDITION_REV1, GEOIP_STANDARD);

        if (NULL == gi) {
            gi = GeoIP_open_type(GEOIP_REGION_EDITION_REV0, GEOIP_STANDARD);
        }
    } else {
        raise_warning("geoip_region_by_name(): Required database not available at %s.", GeoIPDBFileName[GEOIP_REGION_EDITION_REV0]);

        return null_variant;
    }

    gi_region = GeoIP_region_by_name(gi, hostname.c_str());

    GeoIP_delete(gi);

    if (NULL == gi_region) {
        return Variant(false);
    }

    Array region = Array::Create();

    region.add(String("country_code"), Variant(String(gi_region->country_code)));
    region.add(String("region"), Variant(String(gi_region->region)));

    GeoIPRegion_delete(gi_region);

    return Variant(region);
}

#if LIBGEOIP_VERSION >= 1004001
static Variant HHVM_FUNCTION(geoip_region_name_by_code, const String& country_code, const String& region_code) {
    const char *region_name;

    if ( ! country_code.length() || ! region_code.length()) {
        raise_warning("geoip_region_name_by_code(): You need to specify the country and region codes.");

        return Variant(false);
    }

    region_name = GeoIP_region_name_by_code(country_code.c_str(), region_code.c_str());

    if (region_name == NULL) {
        return Variant(false);
    }

    return Variant(String(region_name));
}
#endif

#if LIBGEOIP_VERSION >= 1004001
static Variant HHVM_FUNCTION(geoip_setup_custom_directory, const String& directory) {
    char *custom_directory = (char *) directory.c_str();

#if LIBGEOIP_VERSION >= 1004007
    GeoIP_cleanup();
#else
    if (GeoIPDBFileName != NULL) {
        int i;

        for (i = 0; i < NUM_DB_TYPES; i++) {
            if (GeoIPDBFileName[i]) {
                free(GeoIPDBFileName[i]);
            }
        }

        free(GeoIPDBFileName);
        GeoIPDBFileName = NULL;
    }
#endif
    GeoIP_setup_custom_directory(*custom_directory ? custom_directory : NULL);
    GeoIP_db_avail(GEOIP_COUNTRY_EDITION);

    return null_variant;
}
#endif

#if LIBGEOIP_VERSION >= 1004001
static Variant HHVM_FUNCTION(geoip_time_zone_by_country_and_region, const String& country_code, const Variant& region_code) {
    const char *timezone;
    const char *region;

    if ( ! country_code.length()) {
        raise_warning("geoip_time_zone_by_country_and_region(): You need to specify at least the country code.");

        return Variant(false);
    }

    if (region_code.isNull()) {
        region = NULL;
    } else if (region_code.isString()) {
        region = region_code.toString().c_str();
    } else {
        raise_param_type_warning("geoip_time_zone_by_country_and_region", 2, KindOfString, region_code.getType());

        return null_variant;
    }

    timezone = GeoIP_time_zone_by_country_and_region(country_code.c_str(), region);

    if (timezone == NULL) {
        return Variant(false);
    }

    return Variant(String(timezone));
}
#endif

////////////////////////////////////////////////////////////////////////////////

struct geoipGlobals {
    std::string custom_directory;
};

IMPLEMENT_THREAD_LOCAL(geoipGlobals, s_geoip_globals);

class geoipExtension: public Extension {
    public:
        geoipExtension(): Extension("geoip", "1.1.1-dev") {}

        virtual void threadInit() override {
            IniSetting::Bind(
                this,
                IniSetting::PHP_INI_ALL,
                "geoip.custom_directory",
                "",
                IniSetting::SetAndGet<std::string>(
#if LIBGEOIP_VERSION >= 1004001
                    updateCustomDirectory,
#else
                    nullptr,
#endif
                    nullptr
                ),
                &s_geoip_globals->custom_directory
            );
        }

        virtual void moduleInit() override {
            Native::registerConstant<KindOfInt64>(s_GEOIP_COUNTRY_EDITION.get(), k_GEOIP_COUNTRY_EDITION);
            Native::registerConstant<KindOfInt64>(s_GEOIP_REGION_EDITION_REV0.get(), k_GEOIP_REGION_EDITION_REV0);
            Native::registerConstant<KindOfInt64>(s_GEOIP_CITY_EDITION_REV0.get(), k_GEOIP_CITY_EDITION_REV0);
            Native::registerConstant<KindOfInt64>(s_GEOIP_ORG_EDITION.get(), k_GEOIP_ORG_EDITION);
            Native::registerConstant<KindOfInt64>(s_GEOIP_ISP_EDITION.get(), k_GEOIP_ISP_EDITION);
            Native::registerConstant<KindOfInt64>(s_GEOIP_CITY_EDITION_REV1.get(), k_GEOIP_CITY_EDITION_REV1);
            Native::registerConstant<KindOfInt64>(s_GEOIP_REGION_EDITION_REV1.get(), k_GEOIP_REGION_EDITION_REV1);
            Native::registerConstant<KindOfInt64>(s_GEOIP_PROXY_EDITION.get(), k_GEOIP_PROXY_EDITION);
            Native::registerConstant<KindOfInt64>(s_GEOIP_ASNUM_EDITION.get(), k_GEOIP_ASNUM_EDITION);
            Native::registerConstant<KindOfInt64>(s_GEOIP_NETSPEED_EDITION.get(), k_GEOIP_NETSPEED_EDITION);
            Native::registerConstant<KindOfInt64>(s_GEOIP_NETSPEED_EDITION_REV1.get(), k_GEOIP_NETSPEED_EDITION_REV1);
            Native::registerConstant<KindOfInt64>(s_GEOIP_DOMAIN_EDITION.get(), k_GEOIP_DOMAIN_EDITION);
            Native::registerConstant<KindOfInt64>(s_GEOIP_UNKNOWN_SPEED.get(), k_GEOIP_UNKNOWN_SPEED);
            Native::registerConstant<KindOfInt64>(s_GEOIP_DIALUP_SPEED.get(), k_GEOIP_DIALUP_SPEED);
            Native::registerConstant<KindOfInt64>(s_GEOIP_CABLEDSL_SPEED.get(), k_GEOIP_CABLEDSL_SPEED);
            Native::registerConstant<KindOfInt64>(s_GEOIP_CORPORATE_SPEED.get(), k_GEOIP_CORPORATE_SPEED);

            HHVM_FE(geoip_asnum_by_name);
            HHVM_FE(geoip_continent_code_by_name);
            HHVM_FE(geoip_country_code_by_name);
            HHVM_FE(geoip_country_code3_by_name);
            HHVM_FE(geoip_country_name_by_name);
            HHVM_FE(geoip_database_info);
            HHVM_FE(geoip_db_avail);
            HHVM_FE(geoip_db_filename);
            HHVM_FE(geoip_db_get_all_info);
            HHVM_FE(geoip_domain_by_name);
            HHVM_FE(geoip_id_by_name);
            HHVM_FE(geoip_isp_by_name);
#if LIBGEOIP_VERSION >= 1004008
            HHVM_FE(geoip_netspeedcell_by_name);
#endif
            HHVM_FE(geoip_org_by_name);
            HHVM_FE(geoip_record_by_name);
            HHVM_FE(geoip_region_by_name);
#if LIBGEOIP_VERSION >= 1004001
            HHVM_FE(geoip_region_name_by_code);
            HHVM_FE(geoip_setup_custom_directory);
            HHVM_FE(geoip_time_zone_by_country_and_region);
#endif

            loadSystemlib();

#if LIBGEOIP_VERSION >= 1004001
            char *custom_directory = (char *) s_geoip_globals->custom_directory.c_str();

            if (*custom_directory) {
                GeoIP_setup_custom_directory(custom_directory);
            }
#endif

            GeoIP_db_avail(GEOIP_COUNTRY_EDITION);
        }

#if LIBGEOIP_VERSION >= 1004001
    private:
        static bool updateCustomDirectory(const std::string& value) {
            s_geoip_globals->custom_directory = value.data();
            char *custom_directory = (char *) s_geoip_globals->custom_directory.c_str();

#if LIBGEOIP_VERSION >= 1004007
            GeoIP_cleanup();
#else
            if (GeoIPDBFileName != NULL) {
                int i;

                for (i = 0; i < NUM_DB_TYPES; i++) {
                    if (GeoIPDBFileName[i]) {
                        free(GeoIPDBFileName[i]);
                    }
                }

                free(GeoIPDBFileName);
                GeoIPDBFileName = NULL;
            }
#endif
            GeoIP_setup_custom_directory(*custom_directory ? custom_directory : NULL);
            GeoIP_db_avail(GEOIP_COUNTRY_EDITION);

            return true;
        }
#endif
} s_geoip_extension;

HHVM_GET_MODULE(geoip);

} /* namespace HPHP */
