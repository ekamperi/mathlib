#!/bin/sh

plot1dfile()
{
    cat > 1dscript.plot <<EOF
set terminal png
set out '$1.png'
set logscale x
set xlabel "x"
set ylabel "t"
plot '$1'
EOF
gnuplot 1dscript.plot
}

plot2dfile()
{
    cat > 2dscript.plot <<EOF
set terminal png
set out '$1.png'
set logscale x
set logscale y
set xlabel "x"
set ylabel "y"
set zlabel "t"
set view 60,30
set style data lines
set dgrid3d 50,50,50
splot '$1'
EOF
gnuplot 2dscript.plot
}

for i in *.csv;
do
    {
	read header
	if [ "$header" = "#1" ];
	then
	    plot1dfile "$i"
	fi

	if [ "$header" = "#2" ];
	then
	    plot2dfile "$i"
	fi
    } < "$i"
done
