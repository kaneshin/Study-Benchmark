#!/bin/sh
# ==============================================================================
# File:        worker.sh
# Maintainer:  Shintaro Kaneko <kaneshin0120@gmail.com>
# Last Change: 01-Dec-2014.
# License:     The MIT License
# ==============================================================================
# Description:
#
# Usage:
# ./worker.sh
# ==============================================================================

USERS=$(ls -al .. | awk '{print $9}')

_run()
{
    user=$1
    cd ../$user
    make bench
}

for OPT in $USERS
do
    case $OPT in
        '.'|'..'|'worker')
            ;;
        *)
            _run $OPT
            ;;
    esac
    shift
done

