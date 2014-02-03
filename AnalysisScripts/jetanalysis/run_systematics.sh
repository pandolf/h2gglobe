#!/bin/bash

export DISPLAY=""

dir="batchOutput_qJetEtaFix_newLD_v1"
mkdir $dir

#tunes="TuneD6T TuneD6TUEOFF TuneP0 TuneP0UEOFF TuneProPT0 TuneProPT0UEOFF TuneProQ20 TuneProQ20UEOFF TuneUEOFF TuneZ2Star"
### tunes="TuneP0 TuneProPT0 TuneUEOFF TuneZ2Star TuneD6T TuneProQ20"
#
#for tune in $tunes; do
#    rm jetanalysis/tmp_datafiles_mvavbftag_tunes.dat{,.pevents}
#    ./mk_fitter.py -i jetanalysis/datafiles_mvavbftag_tunes.dat -n 5 -l ${tune} -o ${dir}/mavtag_${tune}/sub &&  \
#     	./submit_fitter.py -q 8nh -d ${dir}/mavtag_${tune}
#    ### rm jetanalysis/tmp_datafiles_cutbasedvbftag_tunes.dat{,.pevents}
#    ### ./mk_fitter.py -i jetanalysis/datafiles_cutbasedvbftag_tunes.dat -n 5 -l ${tune} -o ${dir}/cutbtag_${tune}/sub && \
#    ###	./submit_fitter.py -q 8nh -d ${dir}/cutbtag_${tune}
#done

## dir="yr3_systematics_v1"
## mkdir $dir

#jecUncs=""
## jecUncs="jerCentral"
jecUncs="jecUp jecDown jerUp jerDown jerCentral"

for syst in $jecUncs; do
    ./mk_fitter.py -i jetanalysis/datafiles_cubased_legacy_fp_jec.dat -n 6 -l ${syst} -o ${dir}/${syst}/sub && \
    ./submit_fitter.py -q 8nh -d ${dir}/${syst}
done
