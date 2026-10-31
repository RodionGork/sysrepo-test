#!/bin/bash

if [ $# -lt 4 ]; then
  ./a.out
  exit
fi

set -e

sysrepoctl -i test-module.yang
#sysrepocfg -I test-module-sample.xml

sleep 1

gcc test.c -lsysrepo

time ./a.out $@ &

until [ -f proceed.flag ]; do
  sleep 1
done

time sysrepocfg -Xtest.res -m test-module -d operational

until [ -f complete.flag ]; do
  sleep 1
done

echo going to unregister test-module

sysrepoctl -u test-module

rm -f *.flag

echo "lines in exported file:"
wc -l test.res
