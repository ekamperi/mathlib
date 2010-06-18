#!/bin/sh

KERNEL=$(uname -s)
RELEASE=$(uname -r)
MACHINE=$(uname -m)

FILENAME=results-$(date '+%d-%m-%y').html
USERNAME="beket"
HOSTNAME="leaf.dragonflybsd.org"
REMOTEDIR="public_html/logs/$KERNEL/$RELEASE/$MACHINE"

echo ">>> Uploading ATF results to $USERNAME@$HOSTNAME:~/$REMOTEDIR"
scp results.html "$USERNAME@$HOSTNAME:~/$REMOTEDIR/$FILENAME"
echo "DONE"
