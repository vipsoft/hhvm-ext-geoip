--TEST--
Checking geoip_region_by_name
--SKIPIF--
<?php
ini_set('geoip.custom_directory', __DIR__ . '/data');

if (!extension_loaded("geoip") || !geoip_db_avail(GEOIP_REGION_EDITION_REV0) || !geoip_db_avail(GEOIP_REGION_EDITION_REV1)) print "skip";
?>
--POST--
--GET--
--FILE--
<?php

ini_set('geoip.custom_directory', __DIR__ . '/data');

var_dump(geoip_region_by_name(null));
var_dump(geoip_region_by_name(''));
var_dump(geoip_region_by_name('127.0.0.1'));
var_dump(geoip_region_by_name('localhost'));
var_dump(geoip_region_by_name('64.17.254.223'));

?>
--EXPECTF--
Notice: geoip_region_by_name(): Host  not found in %s
bool(false)

Notice: geoip_region_by_name(): Host  not found in %s
bool(false)
array(2) {
  ["country_code"]=>
  string(0) ""
  ["region"]=>
  string(0) ""
}
array(2) {
  ["country_code"]=>
  string(0) ""
  ["region"]=>
  string(0) ""
}
array(2) {
  ["country_code"]=>
  string(2) "US"
  ["region"]=>
  string(2) "CA"
}
