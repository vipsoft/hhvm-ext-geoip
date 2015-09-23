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
   | Authors: Matthew Fonda, Olivier Hill, Anthon Pang, Adam Saponara     |
   +----------------------------------------------------------------------+
*/

#include "hphp/runtime/ext/extension.h"
#include "hphp/util/lock.h"
#include <cinttypes>
#include <GeoIP.h>
#include <GeoIPCity.h>

namespace HPHP {

// Database types recognized by geoip_database_info() geoip_db_avail(), and geoip_db_filename()
#define GEOIP_CONST(c) \
    const int64_t k_ ## c = c; \
    const StaticString s_ ## c (#c);
#include "constants.h"
#undef GEOIP_CONST

static Mutex filename_mutex;

// Return true if db_type is a valid geoip db type
static bool geoip_is_db_valid(int db_type) {
    return db_type >= 0 && db_type < NUM_DB_TYPES ? true : false;
}

// Safe wrapper for GeoIP_open_type
static GeoIP* geoip_open_db(int db_type, int db_type_fallback, int use_fallback) {
    GeoIP* gi;
    int db_used;
    gi = nullptr;
    if (!geoip_is_db_valid(db_type)
        || (use_fallback && !geoip_is_db_valid(db_type_fallback))
    ) {
        raise_warning("Database type given is out of bound.");
        return nullptr;
    } else if (GeoIP_db_avail(db_type)) {
        db_used = db_type;
        gi = GeoIP_open_type(db_type, GEOIP_STANDARD);
    } else if (use_fallback && GeoIP_db_avail(db_type_fallback)) {
        db_used = db_type_fallback;
        gi = GeoIP_open_type(db_type_fallback, GEOIP_STANDARD);
    } else {
        if (GeoIPDBFileName[db_type]) {
            raise_warning("Required database not available at %s.", GeoIPDBFileName[db_type]);
        } else {
            raise_warning("Required database not available.");
        }
        return nullptr;
    }
    if (!gi) {
        raise_warning("Unable to open database %s.", GeoIPDBFileName[db_used]);
    }
    return gi;
}

// Generic pattern for most functions: open db, callback, close db
static Variant geoip_generic(
    int db_type,
    int db_type_fallback,
    int use_fallback,
    std::function<Variant(GeoIP*)> callback_func
) {
    Lock lock(filename_mutex);
    GeoIP* gi;
    Variant retval;
    if (!(gi = geoip_open_db(db_type, db_type_fallback, use_fallback))) {
        return null_variant;
    }
    retval = callback_func(gi);
    GeoIP_delete(gi);
    return retval;
}

// Generic pattern for most string-return functions: Call geoip_func, maybe free
static Variant geoip_generic_string(
    int db_type,
    int db_type_fallback,
    int use_fallback,
    char* (*geoip_func)(GeoIP* gi, const char* host),
    const String& hostname,
    int do_free
) {
    return geoip_generic(db_type, db_type_fallback, use_fallback, [&] (GeoIP* gi) -> Variant {
        char* str = geoip_func(gi, hostname.c_str());
        if (!str) {
            return Variant(false);
        }
        Variant retval = Variant(String(str));
        if (do_free) free(str);
        return retval;
    });
}

// Generic pattern for record-return functions: Call geoip_func, build array,
// free
static Variant geoip_generic_record(
    int db_type,
    int db_type_fallback,
    int use_fallback,
    GeoIPRecord* (*geoip_func)(GeoIP* gi, const char* host),
    const String& hostname
) {
    return geoip_generic(db_type, db_type_fallback, use_fallback, [&] (GeoIP* gi) -> Variant {
        GeoIPRecord* gi_record = geoip_func(gi, hostname.c_str());
        if (!gi_record) {
            return Variant(false);
        }
        Array record = Array::Create();
#if LIBGEOIP_VERSION >= 1004003
        record.add(String("continent_code"), Variant(String((NULL == gi_record->continent_code) ? "" : gi_record->continent_code)));
#endif
        record.add(String("country_code"), Variant(String((NULL == gi_record->country_code) ? "" : gi_record->country_code)));
        record.add(String("country_code3"), Variant(String((NULL == gi_record->country_code3) ? "" : gi_record->country_code3)));
        record.add(String("country_name"), Variant(String((NULL == gi_record->country_name) ? "" : gi_record->country_name)));
        record.add(String("region"), Variant(String((NULL == gi_record->region) ? "" : gi_record->region)));
        record.add(String("city"), Variant(String((NULL == gi_record->city) ? "" : gi_record->city)));
        record.add(String("postal_code"), Variant(String((NULL == gi_record->postal_code) ? "" : gi_record->postal_code)));
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
    });
}

// Generic pattern for region-return functions: Call geoip_func, build array,
// free
static Variant geoip_generic_region(
    int db_type,
    int db_type_fallback,
    int use_fallback,
    GeoIPRegion* (*geoip_func)(GeoIP* gi, const char* host),
    const String& hostname
) {
    return geoip_generic(db_type, db_type_fallback, use_fallback, [&] (GeoIP* gi) -> Variant {
        GeoIPRegion* gi_region = geoip_func(gi, hostname.c_str());
        if (!gi_region) {
            return Variant(false);
        }
        Array region = Array::Create();
        region.add(String("country_code"), Variant(String((NULL == gi_region->country_code) ? "" : gi_region->country_code)));
        region.add(String("region"), Variant(String((NULL == gi_region->region) ? "" : gi_region->region)));
        GeoIPRegion_delete(gi_region);
        return Variant(region);
    });
}

// ===== HHVM_FUNCTIONs ========================================================

static Variant HHVM_FUNCTION(geoip_asnum_by_name, const String& hostname) {
    return geoip_generic_string(GEOIP_ASNUM_EDITION, 0, 0, GeoIP_name_by_name, hostname, 1);
}

static Variant HHVM_FUNCTION(geoip_country_code_by_name, const String& hostname) {
    return geoip_generic_string(GEOIP_COUNTRY_EDITION, 0, 0, (char* (*)(GeoIP*, const char*))GeoIP_country_code_by_name, hostname, 0);
}

static Variant HHVM_FUNCTION(geoip_country_code3_by_name, const String& hostname) {
    return geoip_generic_string(GEOIP_COUNTRY_EDITION, 0, 0, (char* (*)(GeoIP*, const char*))GeoIP_country_code3_by_name, hostname, 0);
}

static Variant HHVM_FUNCTION(geoip_country_name_by_name, const String& hostname) {
    return geoip_generic_string(GEOIP_COUNTRY_EDITION, 0, 0, (char* (*)(GeoIP*, const char*))GeoIP_country_name_by_name, hostname, 0);
}

static Variant HHVM_FUNCTION(geoip_continent_code_by_name, const String& hostname) {
    return geoip_generic(GEOIP_COUNTRY_EDITION, 0, 0, [&] (GeoIP* gi) -> Variant {
        int id = GeoIP_id_by_name(gi, hostname.c_str());
        if (!id) {
            return Variant(false);
        }
        return Variant(String(GeoIP_country_continent[id]));
    });
}

static Variant HHVM_FUNCTION(geoip_id_by_name, const String& hostname) {
    return geoip_generic(GEOIP_NETSPEED_EDITION, 0, 0, [&] (GeoIP* gi) -> Variant {
        return Variant((uint64_t)GeoIP_id_by_name(gi, hostname.c_str()));
    });
}

static Variant HHVM_FUNCTION(geoip_domain_by_name, const String& hostname) {
    return geoip_generic_string(GEOIP_DOMAIN_EDITION, 0, 0, GeoIP_name_by_name, hostname, 1);
}


static Variant HHVM_FUNCTION(geoip_isp_by_name, const String& hostname) {
    return geoip_generic_string(GEOIP_ISP_EDITION, 0, 0, GeoIP_name_by_name, hostname, 1);
}

#if LIBGEOIP_VERSION >= 1004008
static Variant HHVM_FUNCTION(geoip_netspeedcell_by_name, const String& hostname) {
    return geoip_generic_string(GEOIP_NETSPEED_EDITION_REV1, 0, 0, GeoIP_name_by_name, hostname, 1);
}
#endif

static Variant HHVM_FUNCTION(geoip_org_by_name, const String& hostname) {
    return geoip_generic_string(GEOIP_ORG_EDITION, 0, 0, GeoIP_name_by_name, hostname, 1);
}

static Variant HHVM_FUNCTION(geoip_record_by_name, const String& hostname) {
    return geoip_generic_record(GEOIP_CITY_EDITION_REV1, GEOIP_CITY_EDITION_REV0, 1, GeoIP_record_by_name, hostname);
}

static Variant HHVM_FUNCTION(geoip_region_by_name, const String& hostname) {
    return geoip_generic_region(GEOIP_REGION_EDITION_REV1, GEOIP_REGION_EDITION_REV0, 1, GeoIP_region_by_name, hostname);
}

#if LIBGEOIP_VERSION >= 1004005
static Variant HHVM_FUNCTION(geoip_asnum_by_name_v6, const String& hostname) {
    return geoip_generic_string(GEOIP_ASNUM_EDITION_V6, 0, 0, GeoIP_name_by_name_v6, hostname, 1);
}

static Variant HHVM_FUNCTION(geoip_country_code_by_name_v6, const String& hostname) {
    return geoip_generic_string(GEOIP_COUNTRY_EDITION_V6, 0, 0, (char* (*)(GeoIP*, const char*))GeoIP_country_code_by_name_v6, hostname, 0);
}

static Variant HHVM_FUNCTION(geoip_country_code3_by_name_v6, const String& hostname) {
    return geoip_generic_string(GEOIP_COUNTRY_EDITION_V6, 0, 0, (char* (*)(GeoIP*, const char*))GeoIP_country_code3_by_name_v6, hostname, 0);
}

static Variant HHVM_FUNCTION(geoip_country_name_by_name_v6, const String& hostname) {
    return geoip_generic_string(GEOIP_COUNTRY_EDITION_V6, 0, 0, (char* (*)(GeoIP*, const char*))GeoIP_country_name_by_name_v6, hostname, 0);
}

static Variant HHVM_FUNCTION(geoip_continent_code_by_name_v6, const String& hostname) {
    return geoip_generic(GEOIP_COUNTRY_EDITION_V6, 0, 0, [&] (GeoIP* gi) -> Variant {
        int id = GeoIP_id_by_name_v6(gi, hostname.c_str());
        if (!id) {
            return Variant(false);
        }
        return Variant(String(GeoIP_country_continent[id]));
    });
}

static Variant HHVM_FUNCTION(geoip_id_by_name_v6, const String& hostname) {
    return geoip_generic(GEOIP_NETSPEED_EDITION_REV1_V6, 0, 0, [&] (GeoIP* gi) -> Variant {
        return Variant((uint64_t)GeoIP_id_by_name_v6(gi, hostname.c_str()));
    });
}

static Variant HHVM_FUNCTION(geoip_domain_by_name_v6, const String& hostname) {
    return geoip_generic_string(GEOIP_DOMAIN_EDITION_V6, 0, 0, GeoIP_name_by_name_v6, hostname, 1);
}


static Variant HHVM_FUNCTION(geoip_isp_by_name_v6, const String& hostname) {
    return geoip_generic_string(GEOIP_ISP_EDITION_V6, 0, 0, GeoIP_name_by_name_v6, hostname, 1);
}

#if LIBGEOIP_VERSION >= 1004008
static Variant HHVM_FUNCTION(geoip_netspeedcell_by_name_v6, const String& hostname) {
    return geoip_generic_string(GEOIP_NETSPEED_EDITION_REV1_V6, 0, 0, GeoIP_name_by_name_v6, hostname, 1);
}
#endif

static Variant HHVM_FUNCTION(geoip_org_by_name_v6, const String& hostname) {
    return geoip_generic_string(GEOIP_ORG_EDITION_V6, 0, 0, GeoIP_name_by_name_v6, hostname, 1);
}

static Variant HHVM_FUNCTION(geoip_record_by_name_v6, const String& hostname) {
    return geoip_generic_record(GEOIP_CITY_EDITION_REV1_V6, GEOIP_CITY_EDITION_REV0_V6, 1, GeoIP_record_by_name_v6, hostname);
}

static Variant HHVM_FUNCTION(geoip_region_by_name_v6, const String& hostname) {
    return geoip_generic_region(GEOIP_REGION_EDITION_REV1, GEOIP_REGION_EDITION_REV0, 1, GeoIP_region_by_name_v6, hostname);
}

#endif

#if LIBGEOIP_VERSION >= 1004001
static Variant HHVM_FUNCTION(geoip_region_name_by_code, const String& country_code, const String& region_code) {
    const char* region_name;
    if (!country_code.length() || !region_code.length()) {
        raise_warning("geoip_region_name_by_code(): You need to specify the country and region codes.");
        return Variant(false);
    }
    region_name = GeoIP_region_name_by_code(country_code.c_str(), region_code.c_str());
    if (!region_name) {
        return Variant(false);
    }
    return Variant(String(region_name));
}
#endif

#if LIBGEOIP_VERSION >= 1004001
static Variant HHVM_FUNCTION(geoip_setup_custom_directory, const String& directory) {
    Lock lock(filename_mutex);
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
    const char* timezone;
    const char* region;
    if (!country_code.length()) {
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
    if (!timezone) {
        return Variant(false);
    }
    return Variant(String(timezone));
}
#endif

static Variant HHVM_FUNCTION(geoip_database_info, int64_t database /* = GEOIP_COUNTRY_EDITION */) {
    return geoip_generic(database, 0, 0, [&] (GeoIP* gi) -> Variant {
        char* db_info = GeoIP_database_info(gi);
        Variant retval = Variant(String(db_info));
        free(db_info);
        return retval;
    });
}

static Variant HHVM_FUNCTION(geoip_db_avail, int64_t database) {
    Lock lock(filename_mutex);
    if (!geoip_is_db_valid(database)) {
        raise_warning("geoip_db_avail(): Database type given is out of bound.");
        return null_variant;
    }
    return Variant((bool) GeoIP_db_avail(database));
}

static Variant HHVM_FUNCTION(geoip_db_filename, int64_t database) {
    Lock lock(filename_mutex);
    const char* filename;
    if (!geoip_is_db_valid(database)) {
        raise_warning("geoip_db_filename(): Database type given is out of bound.");
        return null_variant;
    }
    filename = GeoIPDBFileName[database];
    if (!filename) {
        return null_variant;
    }
    return Variant(String(filename));
}

static Array HHVM_FUNCTION(geoip_db_get_all_info) {
    Lock lock(filename_mutex);
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
#define GEOIP_CONST(c) \
    Native::registerConstant<KindOfInt64>((s_ ## c).get(), k_ ## c);
#include "constants.h"
#undef GEOIP_CONST

            HHVM_FE(geoip_asnum_by_name);
            HHVM_FE(geoip_continent_code_by_name);
            HHVM_FE(geoip_country_code_by_name);
            HHVM_FE(geoip_country_code3_by_name);
            HHVM_FE(geoip_country_name_by_name);
            HHVM_FE(geoip_domain_by_name);
            HHVM_FE(geoip_id_by_name);
            HHVM_FE(geoip_isp_by_name);
            HHVM_FE(geoip_org_by_name);
            HHVM_FE(geoip_record_by_name);
            HHVM_FE(geoip_region_by_name);
#if LIBGEOIP_VERSION >= 1004005
            HHVM_FE(geoip_asnum_by_name_v6);
            HHVM_FE(geoip_continent_code_by_name_v6);
            HHVM_FE(geoip_country_code_by_name_v6);
            HHVM_FE(geoip_country_code3_by_name_v6);
            HHVM_FE(geoip_country_name_by_name_v6);
            HHVM_FE(geoip_domain_by_name_v6);
            HHVM_FE(geoip_id_by_name_v6);
            HHVM_FE(geoip_isp_by_name_v6);
            HHVM_FE(geoip_org_by_name_v6);
            HHVM_FE(geoip_record_by_name_v6);
            HHVM_FE(geoip_region_by_name_v6);
#endif
            HHVM_FE(geoip_database_info);
            HHVM_FE(geoip_db_avail);
            HHVM_FE(geoip_db_filename);
            HHVM_FE(geoip_db_get_all_info);
#if LIBGEOIP_VERSION >= 1004001
            HHVM_FE(geoip_region_name_by_code);
            HHVM_FE(geoip_setup_custom_directory);
            HHVM_FE(geoip_time_zone_by_country_and_region);
#endif
#if LIBGEOIP_VERSION >= 1004008
            HHVM_FE(geoip_netspeedcell_by_name);
            HHVM_FE(geoip_netspeedcell_by_name_v6);
#endif

            loadSystemlib();

#if LIBGEOIP_VERSION >= 1004001
            char *custom_directory = (char *) s_geoip_globals->custom_directory.c_str();

            if (*custom_directory) {
                GeoIP_setup_custom_directory(custom_directory);
            }
#endif
            Lock lock(filename_mutex);

            GeoIP_db_avail(GEOIP_COUNTRY_EDITION);
        }

#if LIBGEOIP_VERSION >= 1004001
    private:
        static bool updateCustomDirectory(const std::string& value) {
            s_geoip_globals->custom_directory = value.data();
            char *custom_directory = (char *) s_geoip_globals->custom_directory.c_str();
            Lock lock(filename_mutex);

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
