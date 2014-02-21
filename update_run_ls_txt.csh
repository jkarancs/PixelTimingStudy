# This file explains how to download and create a text file with
# the offical instlumi/pileup values for 2012 Data ordered by Run, Ls
# Official Twiki page you can check for latest info
# https://twiki.cern.ch/twiki/bin/view/CMS/LumiCalc

cd ../..
cvs co -r V04-02-03 RecoLuminosity/LumiDB
scram b
cmsenv

# Run lumiCalc2.py script and reformat output to
# [Run] [LumiSection] [InstLumi (delivered - nb-1/LS)] [Pileup]
RecoLuminosity/LumiDB/scripts/lumiCalc2.py --begin "01/01/10 00:00:00" --end "05/09/13 12:00:00" --nowarning -b stable overview | grep -v WARNING | grep -v n/a | tail -n+13 | head -n-6 | sed "s;:; ;;s;(/mb);0.001;;s;(/ub);1;;s;(/nb);1000;;s;(/pb);1000000;" | awk '{ printf "%d %f\n", $2, $7*$8 }' > ! DPGAnalysis/PixelTimingStudy/run_ls_instlumi_pileup_2012.txt

cd -
