if !(test -d ../data) then
    mkdir ../data
fi
adb push ./trace /data/local/tmp/
adb shell /data/local/tmp/trace $1 $2 /data/local/tmp/cpufreq_results.csv
adb pull /data/local/tmp/cpufreq_results.csv ../data/