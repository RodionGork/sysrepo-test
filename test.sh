#!/bin/bash

set -e

sysrepoctl -i test-module.yang
sysrepocfg -I test-module-sample.xml
sysrepocfg -X -m test-module

gcc test.c -lsysrepo

./a.out & sleep 1 && sysrepocfg -X -m test-module -d operational

sysrepoctl -u test-module
