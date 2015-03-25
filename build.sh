#!/bin/bash

if [ "$HPHPIZE" == "" ]; then
	HPHPIZE=hphpize
fi
if [ "$HPHP_HOME" != "" ] && [ "$HPHPIZE" == "hphpize" ]; then
	HPHPIZE=$HPHP_HOME/hphp/tools/hphpize/hphpize
fi

if ! type "$HPHPIZE" > /dev/null; then
	echo "$HPHPIZE doesn't exist, please specify with HPHPIZE=<path to hphpize> or use HPHP_HOME"
	exit 127;
fi

$HPHPIZE
cmake .
make
