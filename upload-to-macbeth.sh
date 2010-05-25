#!/bin/sh

FILENAME=atf-soc2010.01-`date '+%d-%m-%y'`.tar.gz
USERNAME="stathis"
HOSTNAME="mac-beth.duskware.de"
REMOTEDIR=""

echo ">>> Creating git archive with name $FILENAME"
git archive --format=tar HEAD | gzip > "$FILENAME"
echo "DONE"

echo ">>> Uploading git archive to $USERNAME@$HOSTNAME:~/$REMOTEDIR"
scp "$FILENAME" "$USERNAME@$HOSTNAME:~/$REMOTEDIR"
echo "DONE"
