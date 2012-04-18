#!/bin/tcsh

set DIR = $1

set USERDIR_EOS = "jkarancs/crab"
set USERDIR_KFKI = "jkarancs"

eos ls eos/cms/store/caf/user/$USERDIR_EOS/$DIR | grep .root | awk '{ print "lcg-cp -v -b -D srmv2 --vo cms srm://srm-eoscms.cern.ch:8443/srm/v2/server\\?SFN=/eos/cms/store/caf/user/'$USERDIR_EOS'/'$DIR'/"$1" srm://grid143.kfki.hu:8446/srm/managerv2\\?SFN=/dpm/kfki.hu/home/cms/phedex/store/user/'$USERDIR_KFKI'/'$DIR'/"$1 }'



