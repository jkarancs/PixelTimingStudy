#!/bin/bash
BADROC=0
PHM=1

echo " ---------------------------------------------------"
for arg in "$@"; do
    if [[ ! $arg == *".py" ]]; then
        echo "$arg is not a crab script"
    else
	CRAB_CFG=$arg
	# read infos from crab config file
        LFN=`grep "config.Data.outLFNDirBase" $CRAB_CFG | sed "s;';;g" | awk '{ print $3 }' | sed 's;/$;;'`
        TASKNAME=`grep "config.Data.outputDatasetTag" $CRAB_CFG | sed "s;';;g" | awk '{ print $3 }'`
	AUTONAME=`echo $CRAB_CFG | sed "s;crab3_;;;s;.py;;"`
	# Set input/output directories
        DL_DIR=`echo $LFN/$TASKNAME | sed "s;/store/user;/data/gridout;"`
	cd /data/jkarancs/CMSSW/PixelHistoMaker_git
	echo
	echo "Using crab config file: $CRAB_CFG"
	echo "  Input dir:   $DL_DIR/*.root"
	echo
	if (( $BADROC )); then
	    echo "- Start creating BADROC list"
	    echo "  Output file: PHM_out/HitEffMonitoring_"$AUTONAME"_BADROC.root"
	    echo "  Log file:    PHM_out/HitEffMonitoring_"$AUTONAME"_BADROC.log"
	    if (( $PHM )); then
		# Wait for BADROC list to finish, before running plotting
		./PixelHistoMaker -o PHM_out/HitEffMonitoring_"$AUTONAME"_BADROC.root "$DL_DIR/*.root" -b > PHM_out/HitEffMonitoring_"$AUTONAME"_BADROC.log 2>&1
	    else
		# Otherwise run in background
		nohup ./PixelHistoMaker -o PHM_out/HitEffMonitoring_"$AUTONAME"_BADROC.root "$DL_DIR/*.root" -b > PHM_out/HitEffMonitoring_"$AUTONAME"_BADROC.log 2>&1 &
	    fi
	fi
	if (( $PHM )); then
	    echo "- Start running PixelHistoMaker (in background)"
	    echo "  Output file: PHM_out/HitEffMonitoring_"$AUTONAME".root"
	    echo "  Log file:    PHM_out/HitEffMonitoring_"$AUTONAME".log"
	    nohup ./PixelHistoMaker -o PHM_out/HitEffMonitoring_"$AUTONAME".root "$DL_DIR/*.root" > PHM_out/HitEffMonitoring_"$AUTONAME".log 2>&1 &
	fi
	echo
	echo " ---------------  "$AUTONAME" Done.  -----------------"
	echo
	cd - > /dev/null
    fi
done
