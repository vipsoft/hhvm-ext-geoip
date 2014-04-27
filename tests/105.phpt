--TEST--
Checking geoip_isp_by_name
--SKIPIF--
<?php
ini_set('geoip.custom_directory', __DIR__ . '/data');

if (!extension_loaded("geoip") || !geoip_db_avail(GEOIP_ISP_EDITION)) print "skip";
?>
--POST--
--GET--
--FILE--
<?php

ini_set('geoip.custom_directory', __DIR__ . '/data');

var_dump(geoip_isp_by_name(null));
var_dump(geoip_isp_by_name(''));
var_dump(geoip_isp_by_name('127.0.0.1'));
var_dump(geoip_isp_by_name('localhost'));
var_dump(geoip_isp_by_name('12.87.118.0'));

?>
--EXPECTF--
Notice: geoip_isp_by_name(): Host  not found in %s
bool(false)

Notice: geoip_isp_by_name(): Host  not found in %s
bool(false)

Notice: geoip_isp_by_name(): Host %s not found in %s
bool(false)

Notice: geoip_isp_by_name(): Host %s not found in %s
bool(false)
string(13) "AT&T Services"
