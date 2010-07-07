#!/bin/sh

set -e

KERNEL=$(uname -s)
RELEASE=$(uname -r)
MACHINE=$(uname -m)

FILENAME=results-$(date '+%d-%m-%y').html
USERNAME="beket"
HOSTNAME="leaf.dragonflybsd.org"
REMOTELOGDIR="public_html/logs/$KERNEL/$RELEASE/$MACHINE"
REMOTEGRAPHSDIR="public_html/graphs/$KERNEL/$RELEASE/$MACHINE"

echo ">>> Uploading ATF results to $USERNAME@$HOSTNAME:~/$REMOTEDIR"
scp results.html "$USERNAME@$HOSTNAME:~/$REMOTELOGDIR/$FILENAME"
echo "DONE"

echo ">>> Uploading graphs to $USERNAME@$HOSTNAME:~/$REMOTEDIR"
scp ../etc/graphs/*.png "$USERNAME@$HOSTNAME:~/$REMOTEGRAPHSDIR/"
echo "DONE"
