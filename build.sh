#!/bin/bash

if [ "$HPHP_HOME" != "" ]; then
    HPHPIZE=$HPHP_HOME/hphp/tools/hphpize/hphpize
else
    HPHPIZE=`which hphpize`
fi

if [ ! -e "$HPHPIZE" ]; then
    echo 'hphpize not found. Try setting the HPHP_HOME environment variable.'
    exit 1
fi

$HPHPIZE
cmake .
make
