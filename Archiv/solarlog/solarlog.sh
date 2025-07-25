#!/bin/sh
LD_LIBRARY_PATH=/usr/local/addons/cuxd curl -X POST -H 'Content-Type: application/json' -d '{"801":{"170":null}}' http://solar-log/getjp
