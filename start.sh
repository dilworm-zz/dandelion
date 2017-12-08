#!/bin/bash

set -o errexit
root=$(echo $(pwd))

svrname=$1

cd $root/src/skynet
./skynet $root/conf/config.$svrname


