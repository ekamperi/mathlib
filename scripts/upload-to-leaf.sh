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

usage()
{
    cat <<EOF
Usage: `basename $0` all | graphs | results
At least one of the above options must be specified.
EOF

    exit 1
}

case "$1" in
    "all")
	all=1
	;;
    "graphs")
	graphs=1
	;;
    "results")
	results=1
	;;
    "")
	usage
	;;    
esac

# At least one argument must be given
[ -z "$all"] && [ -z "$graphs" ] && [ -z "$results" ] && usage

if [ ! -z "$results" ] || [ -z "all" ]; then
    echo ">>> Uploading ATF results to $USERNAME@$HOSTNAME:~/$REMOTEDIR"
    scp results.html "$USERNAME@$HOSTNAME:~/$REMOTELOGDIR/$FILENAME"
    echo "DONE"
fi

if [ ! -z "$graphs" ] || [ -z "all" ]; then
    echo ">>> Uploading graphs to $USERNAME@$HOSTNAME:~/$REMOTEDIR"
    scp ../etc/graphs/*.png "$USERNAME@$HOSTNAME:~/$REMOTEGRAPHSDIR/"
    echo "DONE"
fi
