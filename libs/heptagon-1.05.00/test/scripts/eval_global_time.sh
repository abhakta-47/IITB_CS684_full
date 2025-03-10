#!/bin/bash

HEPTC="heptc -target c -target ctrln"
LOGFILE=eval_global_time.log
cp /dev/null $LOGFILE
plotcomma="plot"
plotcommand=""
for script in $*; do
    echo "Script $script..." | tee -a $LOGFILE
    
    base=`basename $script .py`
    timefile="${base}.time"

    cp /dev/null $timefile
    
    n=10
    a=10
    lasttime=0
    while ((${lasttime%%.*} < 60)); do
        echo "  n = $n..." | tee -a $LOGFILE
        heptname="${base}_${n}.ept"
        echo "    Generation..." | tee -a $LOGFILE
        echo "      \$ $script $n > $heptname" | tee -a $LOGFILE
        $script $n > $heptname
        echo "    Compilation..." | tee -a $LOGFILE
        echo "      \$ $HEPTC $heptname" | tee -a $LOGFILE
        /usr/bin/time -f "%e" -o tmptime $HEPTC $heptname > $LOGFILE
        lasttime=`cat tmptime`
        echo "      ${lasttime}s" | tee -a $LOGFILE
        echo "$n	$lasttime" >> $timefile
        if (( $n == ($a * 10) )); then
            a=$(($a * 10))
        fi
        n=$(($n + $a))
    done
    plotcommand="${plotcommand}${plotcomma} \"${timefile}\" with linespoints title \"${base}\""
    plotcomma=","
done
echo "  Plotting..."
gnuplot <<EOF
set term png
set out "plot.png"
set xlabel "n"
set ylabel "exec time (s)"
$plotcommand
set out "plot_log.png"
set logscale xy
$plotcommand
EOF
cat > result.html <<EOF
<!DOCTYPE html PUBLIC "-//w3c//dtd html 4.0 transitional//en">
<html>
  <head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
  </head>
<body>
  <h1>Global time evaluation</h1>
  <p><img src="plot.png"/></p>
  <p><img src="plot_log.png"/></p>
</body>
</html>
EOF
xdg-open result.html &
