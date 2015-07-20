--TEST--
Checking geoip_record_by_name
--SKIPIF--
<?php if (!extension_loaded("geoip") || !geoip_db_avail(GEOIP_COUNTRY_EDITION)) print "skip"; ?>
--POST--
--GET--
--FILE--
<?php

var_dump(geoip_record_by_name(null));
var_dump(geoip_record_by_name(''));
var_dump(geoip_record_by_name('127.0.0.1'));
var_dump(geoip_record_by_name('localhost'));
var_dump(geoip_record_by_name('128.100.132.238'));
var_dump(geoip_record_by_name('utoronto.ca'));

?>
--EXPECTF--
bool(false)
bool(false)
bool(false)
bool(false)
array(11) {
  ["continent_code"]=>
  string(2) "NA"
  ["country_code"]=>
  string(2) "CA"
  ["country_code3"]=>
  string(3) "CAN"
  ["country_name"]=>
  string(6) "Canada"
  ["region"]=>
  string(2) "ON"
  ["city"]=>
  string(7) "Toronto"
  ["postal_code"]=>
  string(3) "M5S"
  ["latitude"]=>
  float(43.66189956665)
  ["longitude"]=>
  float(-79.395202636719)
  ["dma_code"]=>
  int(0)
  ["area_code"]=>
  int(0)
}
array(11) {
  ["continent_code"]=>
  string(2) "NA"
  ["country_code"]=>
  string(2) "CA"
  ["country_code3"]=>
  string(3) "CAN"
  ["country_name"]=>
  string(6) "Canada"
  ["region"]=>
  string(2) "ON"
  ["city"]=>
  string(7) "Toronto"
  ["postal_code"]=>
  string(3) "M5S"
  ["latitude"]=>
  float(43.66189956665)
  ["longitude"]=>
  float(-79.395202636719)
  ["dma_code"]=>
  int(0)
  ["area_code"]=>
  int(0)
}
