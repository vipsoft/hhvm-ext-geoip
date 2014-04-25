--TEST--
Checking timezone info with (some) empty fields
--SKIPIF--
<?php if (!extension_loaded("geoip")) print "skip"; ?>
--POST--
--GET--
--FILE--
<?php

var_dump(geoip_time_zone_by_country_and_region('CF',''));
var_dump(geoip_time_zone_by_country_and_region('CF',NULL));
var_dump(geoip_time_zone_by_country_and_region('CF'));
var_dump(geoip_time_zone_by_country_and_region(NULL,''));
var_dump(geoip_time_zone_by_country_and_region(NULL,NULL));

?>
--EXPECTF--
string(%d) "Africa/%s"
string(%d) "Africa/%s"
string(%d) "Africa/%s"

Warning: geoip_time_zone_by_country_and_region(): You need to specify at least the country code. in %s on line %d
bool(false)

Warning: geoip_time_zone_by_country_and_region(): You need to specify at least the country code. in %s on line %d
bool(false)
