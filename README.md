## GeoIP (geoip) Extension for HHVM

This is a port of the PECL geoip extension for HipHop Virtual Machine.

### Prerequisites

This extension requires the `<GeoIP.h>` and `<GeoIPCity.h>` header files and `libGeoIP` library installed.

On Ubuntu 12.04, you need to install the "libgeoip-dev" and "libgeoip1" packages.

### Building & Installation

Requires HHVM 3.0 or later.

~~~
$ export HPHP_HOME=/path/to/hhvm
$ cd /path/to/extension
$ ./build.sh
~~~

This will produce a `geoip.so` file, the dynamically-loadable extension.

To enable the extension, you need to have the following section in your PHP ini file:

~~~
hhvm.dynamic_extension_path = /path/to/hhvm/extensions
hhvm.dynamic_extensions[geoip] = geoip.so
~~~

Where `/path/to/hhvm/extensions` is a folder containing all aphp extensions,
and `geoip.so` is in it. This will cause the extension to be loaded when the
virtual machine starts up.

### Testing

To run the test suite:

~~~
$ cd /path/to/extension
$ ./test.sh run-tests.php
~~~
