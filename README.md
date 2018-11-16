## GeoIP (geoip) Extension for HHVM

This is a port of the PECL geoip extension for HipHop Virtual Machine.

Note: you will need to build this extension from source. Due to the
number of platforms supported by HHVM, I am unable to provide binaries.

### Prerequisites

This extension requires the `<GeoIP.h>` and `<GeoIPCity.h>` header files and `libGeoIP` library installed.

On Ubuntu 12.04, you need to install the "libgeoip-dev" and "libgeoip1" packages.

### Building & Installation

Requires HHVM 3.0 or later.

If you have already built hhvm from source, build the `geoip.so` file (the dynamically-loadable extension) using:

~~~
$ export HPHP_HOME=/path/to/hhvm
$ cd /path/to/extension
$ ./build.sh
~~~

To build this extension without the hhvm source, try:

~~~
$ sudo apt-get install hhvm-dev libgeoip-dev libboost-all-dev libdouble-conversion-dev libgoogle-glog-dev libtbb-dev
$ sudo chmod +x /usr/bin/hphpize
$ git clone https://github.com/vipsoft/hhvm-ext-geoip.git
$ cd hhvm-ext-geoip
$ hphpize
$ cmake .
$ make
~~~

To enable the extension, you need to have the following section in your PHP ini file:

~~~
hhvm.dynamic_extension_path = /path/to/hhvm/extensions
hhvm.dynamic_extensions[geoip] = geoip.so
~~~

Note: To enable the extension on older HHVM versions, you will instead have the following section in your HHVM config file:

~~~
DynamicExtensionPath = /path/to/hhvm/extensions
DynamicExtensions {
        * = geoip.so
}
~~~

Where `/path/to/hhvm/extensions` is a folder containing all HHVM extensions,
and `geoip.so` is in it. This will cause the extension to be loaded when the
virtual machine starts up.

### Testing

To run the test suite:

~~~
$ cd /path/to/extension
$ ./test.sh run-tests.php
~~~
