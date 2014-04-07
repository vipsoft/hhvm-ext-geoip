<?hh

/**
 * geoip_continent_code_by_name() - Get the two letter continent code
 *
 * @param string $hostname
 *
 * @return mixed Returns the two letter continent code on success.
 *               Returns FALSE if the address cannot be found in the database.
 *               Returns NULL on error.
 */
<<__Native>> function geoip_continent_code_by_name (string $hostname): mixed;

/**
 * geoip_country_code_by_name() - Get the two letter country code
 *
 * @param string $hostname
 *
 * @return mixed Returns the two letter ISO country code on success.
 *               Returns FALSE if the address cannot be found in the database.
 *               Returns NULL on error.
 */
<<__Native>> function geoip_country_code_by_name (string $hostname): mixed;

/**
 * geoip_country_code3_by_name() - Get the three letter country code
 *
 * @param string $hostname
 *
 * @return mixed Returns the three letter country code on success.
 *               Returns FALSE if the address cannot be found in the database.
 *               Returns NULL on error.
 */
<<__Native>> function geoip_country_code3_by_name (string $hostname): mixed;

/**
 * geoip_country_name_by_name() - Get the full country name
 *
 * @param string $hostname
 *
 * @return mixed Returns the country name on success.
 *               Returns FALSE if the address cannot be found in the database.
 *               Returns NULL on error.
 */
<<__Native>> function geoip_country_name_by_name (string $hostname): mixed;

/**
 * geoip_database_info() - Get GeoIP Database information
 *
 * @param int $database Database type
 *
 * @return string Returns the database version on success.
 *                Returns NULL on error.                
 */
<<__Native>> function geoip_database_info (int $database = GEOIP_COUNTRY_EDITION): ?string;

/**
 * geoip_db_avail() - Determine if GeoIP Database is available
 *
 * @param int $database Database type
 *
 * @return bool Returns TRUE if the database is available.
 *              Returns FALSE if the database is not available.
 *              Returns NULL on error.
 */
<<__Native>> function geoip_db_avail (int $database): ?bool;

/**
 * geoip_db_filename() - Returns the filename of the corresponding GeoIP Database
 *
 * @param int $database Database type
 *
 * @return string Returns the database filename on success.
 *                Returns NULL on error.
 */
<<__Native>> function geoip_db_filename (int $database): ?string;

/**
 * geoip_db_get_all_info() - Returns detailed information about all GeoIP database types
 *
 * @return array Returns an array of associative arrays, each with the keys:
 *               "available" - boolean
 *               "description" - database description
 *               "filename" - database filename
 */
<<__Native>> function geoip_db_get_all_info (): array;

/**
 * geoip_id_by_name() - Get the Internet connection type
 *
 * @param string $hostname
 *
 * @return int Returns one of GEOIP_UNKNOWN_SPEED, GEOIP_DIALUP_SPEED,
 *             GEOIP_CABLEDSL_SPEED, or GEOIP_CORPORATE_SPEED on success.
 *             Returns NULL on error.
 */
<<__Native>> function geoip_id_by_name (string $hostname): ?int;

/**
 * geoip_isp_by_name() - Get the Internet Service Provider (ISP) name
 *
 * @param string $hostname
 *
 * @return mixed Returns the ISP name on success.
 *               Returns FALSE if the address cannot be found in the database.
 *               Returns NULL on error.
 */
<<__Native>> function geoip_isp_by_name (string $hostname): mixed;

/**
 * geoip_org_by_name() - Get the organization name
 *
 * @param string $hostname
 *
 * @return mixed Returns the organization name on success.
 *               Returns FALSE if the address cannot be found in the database.
 *               Returns NULL on error.
 */
<<__Native>> function geoip_org_by_name (string $hostname): mixed;

/**
 * geoip_record_by_name() - Returns the detailed City information found in the GeoIP City Database
 *
 * @param string $hostname
 *
 * @return mixed Returns an associative array with the keys:
 *               "continent_code" - two letter continent code
 *               "country_code" - two letter ISO country code
 *               "country_code3" - three letter country code
 *               "country_name" - country name
 *               "region" - region code
 *               "city" - city
 *               "postal_code" - postal code, FSA, or zip code
 *               "latitude" - latitude
 *               "longitude" - longitude
 *               "dma_code" - Designated Market Area
 *               "area_code" - PSTN area code
 *               Returns FALSE if host not found.
 *               Returns NULL on error.
 */
<<__Native>> function geoip_record_by_name (string $hostname): mixed;

/**
 * geoip_region_by_name() - Get the country code and region
 *
 * @param string $hostname
 *
 * @return mixed Returns an associative array with the keys:
 *               "country_code" - two letter ISO country code
 *               "region" - region code.
 *               Returns FALSE if host not found.
 *               Returns NULL on error.
 */
<<__Native>> function geoip_region_by_name (string $hostname): mixed;

/**
 * geoip_region_name_by_code() - Returns the region name for some country and region code combo
 *
 * @param string $country_code
 * @param string $region_code
 *
 * @return mixed Returns the region name on success.
 *               Returns FALSE if country and region code combo not found.
 *               Returns NULL on error.
 */
<<__Native>> function geoip_region_name_by_code (string $country_code, string $region_code): mixed;

/**
 * geoip_time_zone_by_country_and_region() - Returns the time zone for some country and region code combo
 *
 * @param string $country_code
 * @param string $region_code
 *
 * @return mixed Returns the timezone on success.
 *               Returns FALSE if country and region code combo not found.
 *               Returns NULL on error.
 */
<<__Native>> function geoip_time_zone_by_country_and_region (string $country_code, ?string $region_code = NULL): mixed;
