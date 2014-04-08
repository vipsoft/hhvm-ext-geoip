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
   | Authors: Olivier Hill, Anthon Pang                                   |
   +----------------------------------------------------------------------+
*/

#include "hphp/runtime/base/base-includes.h"
#include <cinttypes>
#include <GeoIP.h>
#include <GeoIPCity.h>

namespace HPHP {

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
const int64_t k_GEOIP_DOMAIN_EDITION = GEOIP_DOMAIN_EDITION;
const StaticString s_GEOIP_DOMAIN_EDITION("GEOIP_DOMAIN_EDITION");
const int64_t k_GEOIP_UNKNOWN_SPEED = GEOIP_UNKNOWN_SPEED;
const StaticString s_GEOIP_UNKNOWN_SPEED("GEOIP_UNKNOWN_SPEED");
const int64_t k_GEOIP_DIALUP_SPEED = GEOIP_DIALUP_SPEED;
const StaticString s_GEOIP_DIALUP_SPEED("GEOIP_DIALUP_SPEED");
const int64_t k_GEOIP_CABLEDSL_SPEED = GEOIP_CABLEDSL_SPEED;
const StaticString s_GEOIP_CABLEDSL_SPEED("GEOIP_CABLEDSL_SPEED");
const int64_t k_GEOIP_CORPORATE_SPEED = GEOIP_CORPORATE_SPEED;
const StaticString s_GEOIP_CORPORATE_SPEED("GEOIP_CORPORATE_SPEED");

static Variant HHVM_FUNCTION(geoip_continent_code_by_name, const String& hostname) {
    return null_variant;
}

static Variant HHVM_FUNCTION(geoip_country_code_by_name, const String& hostname) {
    return null_variant;
}

static Variant HHVM_FUNCTION(geoip_country_code3_by_name, const String& hostname) {
    return null_variant;
}

static Variant HHVM_FUNCTION(geoip_country_name_by_name, const String& hostname) {
    return null_variant;
}

static Variant HHVM_FUNCTION(geoip_database_info, int64_t database /* = GEOIP_COUNTRY_EDITION */) {
    return null_variant;
}

static Variant HHVM_FUNCTION(geoip_db_avail, int64_t database) {
    return null_variant;
}

static Variant HHVM_FUNCTION(geoip_db_filename, int64_t database) {
    return null_variant;
}

static Array HHVM_FUNCTION(geoip_db_get_all_info) {
}

static Variant HHVM_FUNCTION(geoip_id_by_name, const String& hostname) {
    return null_variant;
}

static Variant HHVM_FUNCTION(geoip_isp_by_name, const String& hostname) {
    return null_variant;
}

static Variant HHVM_FUNCTION(geoip_org_by_name, const String& hostname) {
    return null_variant;
}

static Variant HHVM_FUNCTION(geoip_record_by_name, const String& hostname) {
    return null_variant;
}

static Variant HHVM_FUNCTION(geoip_region_by_name, const String& hostname) {
    return null_variant;
}

static Variant HHVM_FUNCTION(geoip_region_name_by_code, const String& country_code, const String& region_code) {
    return null_variant;
}

static Variant HHVM_FUNCTION(geoip_time_zone_by_country_and_region, const String& country_code, const String& region_code) {
    return null_variant;
}

////////////////////////////////////////////////////////////////////////////////

class geoipExtension: public Extension {
public:
  geoipExtension(): Extension("geoip", "1.0.8") {}
  virtual void moduleInit() {
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
    Native::registerConstant<KindOfInt64>(s_GEOIP_DOMAIN_EDITION.get(), k_GEOIP_DOMAIN_EDITION);
    Native::registerConstant<KindOfInt64>(s_GEOIP_UNKNOWN_SPEED.get(), k_GEOIP_UNKNOWN_SPEED);
    Native::registerConstant<KindOfInt64>(s_GEOIP_DIALUP_SPEED.get(), k_GEOIP_DIALUP_SPEED);
    Native::registerConstant<KindOfInt64>(s_GEOIP_CABLEDSL_SPEED.get(), k_GEOIP_CABLEDSL_SPEED);
    Native::registerConstant<KindOfInt64>(s_GEOIP_CORPORATE_SPEED.get(), k_GEOIP_CORPORATE_SPEED);
    HHVM_FE(geoip_continent_code_by_name);
    HHVM_FE(geoip_country_code_by_name);
    HHVM_FE(geoip_country_code3_by_name);
    HHVM_FE(geoip_country_name_by_name);
    HHVM_FE(geoip_database_info);
    HHVM_FE(geoip_db_avail);
    HHVM_FE(geoip_db_filename);
    HHVM_FE(geoip_db_get_all_info);
    HHVM_FE(geoip_id_by_name);
    HHVM_FE(geoip_isp_by_name);
    HHVM_FE(geoip_org_by_name);
    HHVM_FE(geoip_record_by_name);
    HHVM_FE(geoip_region_by_name);
    HHVM_FE(geoip_region_name_by_code);
    HHVM_FE(geoip_time_zone_by_country_and_region);

    loadSystemlib();
  }
} s_geoip_extension;

HHVM_GET_MODULE(geoip);

}
