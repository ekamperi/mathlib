#!/bin/sh

KERNEL=$(uname -s)
RELEASE=$(uname -r)
MACHINE=$(uname -m)

FILENAME=results-$(date '+%d-%m-%y').html
USERNAME="beket"
HOSTNAME="leaf.dragonflybsd.org"
REMOTEDIR="public_html/logs/$KERNEL/$RELEASE/$MACHINE"

echo ">>> Uploading git archive to $USERNAME@$HOSTNAME:~/$REMOTEDIR"
mv tmp2.html "$FILENAME"
scp "$FILENAME" "$USERNAME@$HOSTNAME:~/$REMOTEDIR"
echo "DONE"
