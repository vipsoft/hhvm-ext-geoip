--TEST--
Checking geoip_domain_by_name
--SKIPIF--
<?php
ini_set('geoip.custom_directory', __DIR__ . '/data');

if (!extension_loaded("geoip") || !geoip_db_avail(GEOIP_DOMAIN_EDITION)) print "skip";
?>
--POST--
--GET--
--FILE--
<?php

ini_set('geoip.custom_directory', __DIR__ . '/data');

var_dump(geoip_domain_by_name(null));
var_dump(geoip_domain_by_name(''));
var_dump(geoip_domain_by_name('127.0.0.1'));
var_dump(geoip_domain_by_name('localhost'));
var_dump(geoip_domain_by_name('67.43.156.0'));

?>
--EXPECTF--
bool(false)
bool(false)
bool(false)
bool(false)
string(12) "shoesfin.NET"
