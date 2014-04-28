--TEST--
Checking geoip_netspeedcell_by_name
--SKIPIF--
<?php
ini_set('geoip.custom_directory', __DIR__ . '/data');

if (!extension_loaded("geoip") || !geoip_db_avail(GEOIP_NETSPEED_EDITION_REV1)) print "skip";
?>
--POST--
--GET--
--FILE--
<?php

ini_set('geoip.custom_directory', __DIR__ . '/data');

var_dump(geoip_netspeedcell_by_name(null));
var_dump(geoip_netspeedcell_by_name(''));
var_dump(geoip_netspeedcell_by_name('127.0.0.1'));
var_dump(geoip_netspeedcell_by_name('localhost'));
var_dump(geoip_netspeedcell_by_name('12.87.118.0'));

?>
--EXPECTF--
bool(false)
bool(false)
bool(false)
bool(false)
string(9) "Cable/DSL"
