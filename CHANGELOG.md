# Changelog

## Version 1.0.9-dev

* geoip.custom_directory can now be set via ini_set; fixes https://bugs.php.net/bug.php?id=61607
* update tests/014.phpt to reflect change in libGeoIP 1.4.5+ where GeoIP_time_zone_by_country_and_region() requires a region when country='CA'; fixes https://bugs.php.net/bug.php?id=61834
* increase test coverage

## Version 1.0.8

* HHVM port of [@olivierypg][]'s [geoip PECL extension](http://pecl.php.net/package/geoip)

[@olivierypg]: https://github.com/olivierypg
[@robocoder]: https://github.com/robocoder
