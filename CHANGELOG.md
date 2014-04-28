# Changelog

## Version 1.0.9-dev

* Fix [geoip.custom_directory gets ignored when set via ini_set](https://bugs.php.net/bug.php?id=61607)
* Fix [tests/014.phpt does not work for GeoIP library version 1.4.5 or later](https://bugs.php.net/bug.php?id=61834)
* Increase test coverage and test data
* Sync with 1.0.9-dev of pecl extension
  - Remove notice for "Host %s not found"
  - Add geoip_asnum_by_name(), geoip_domain_by_name(), geoip_setup_custom_directory()
* Fix [Add support for ASNum, Domain and NetSpeedCell databases.](https://bugs.php.net/bug.php?id=67121)
* Fix [Segfault with geoip extension and GeoIP API 1.5](https://bugs.php.net/bug.php?id=64692)

## Version 1.0.8

* HHVM port of [@olivierypg][]'s [geoip PECL extension](http://pecl.php.net/package/geoip)

[@olivierypg]: https://github.com/olivierypg
[@robocoder]: https://github.com/robocoder
