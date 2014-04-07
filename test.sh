#!/bin/sh

DIRNAME=`dirname $0`
REALPATH=`which realpath`
if [ ! -z "${REALPATH}" ]; then
  DIRNAME=`realpath ${DIRNAME}`
fi

export TEST_PHP_EXECUTABLE=$0

${HPHP_HOME}/hphp/hhvm/hhvm -vDynamicExtensions.0=${DIRNAME}/geoip.so $@
