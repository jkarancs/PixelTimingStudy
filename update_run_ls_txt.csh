# This file explains how to download and create a text file with
# the offical instlumi/pileup values for 2015 Data ordered by Run, Ls
# Official Twiki page you can check for latest info
# http://cms-service-lumi.web.cern.ch/cms-service-lumi/brilwsdoc.html

# install brilcalc
lxplus
ssh -YX lxplus.cern.ch
setenv PATH $HOME/.local/bin:/afs/cern.ch/cms/lumi/brilconda-1.0.3/bin:$PATH
pip install --install-option="--prefix=$HOME/.local" brilws

# create txt file
brilcalc lumi --byls -u /nb --begin 3819 --end 4647 | grep "STABLE BEAMS" | sed "s;:; ;g;" | awk '{ printf "%d %d %f %f\n", $2, $5, $18, $22 }' > ! run_ls_instlumi_pileup_2015.txt
