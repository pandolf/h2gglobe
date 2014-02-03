#combine -M HybridNew datacard_thq_leptonic_${1}_noLDcut.txt --significance  --expectedFromGrid=0.5  --saveToys --fullBToys --saveHybridResult -T 2000 -i 10 -s13 >&! log_signif_thq_leptonic_${1}_noLDcut.log
combine -M HybridNew datacard_thq_leptonic_${1}.txt --significance  --expectedFromGrid=0.5  --saveToys --fullBToys --saveHybridResult -T 2000 -i 10 -s13 >&! log_signif_thq_leptonic_${1}.log
combine -M HybridNew datacard_thq_leptonic_${1}_extraTTH.txt --significance  --expectedFromGrid=0.5  --saveToys --fullBToys --saveHybridResult -T 2000 -i 10 -s13 >&! log_signif_thq_leptonic_${1}_extraTTH.log
#combine -M HybridNew datacard_thq_hadronic_${1}.txt --significance  --expectedFromGrid=0.5  --saveToys --fullBToys --saveHybridResult -T 2000 -i 10 -s13 >&! log_signif_thq_hadronic_${1}.log
#combine -M HybridNew datacard_thq_hadronic_${1}_metCut.txt --significance  --expectedFromGrid=0.5  --saveToys --fullBToys --saveHybridResult -T 2000 -i 10 -s13 >&! log_signif_thq_hadronic_${1}_metCut.log

# +1 sigma
combine -M HybridNew datacard_thq_leptonic_${1}.txt --significance  --expectedFromGrid=0.84  --saveToys --fullBToys --saveHybridResult -T 2000 -i 10 -s13 >&! log_signif_thq_leptonic_${1}_p1s.log
combine -M HybridNew datacard_thq_leptonic_${1}_extraTTH.txt --significance  --expectedFromGrid=0.84  --saveToys --fullBToys --saveHybridResult -T 2000 -i 10 -s13 >&! log_signif_thq_leptonic_${1}_extraTTH_p1s.log

# -1 sigma
combine -M HybridNew datacard_thq_leptonic_${1}.txt --significance  --expectedFromGrid=0.16  --saveToys --fullBToys --saveHybridResult -T 2000 -i 10 -s13 >&! log_signif_thq_leptonic_${1}_m1s.log
combine -M HybridNew datacard_thq_leptonic_${1}_extraTTH.txt --significance  --expectedFromGrid=0.16  --saveToys --fullBToys --saveHybridResult -T 2000 -i 10 -s13 >&! log_signif_thq_leptonic_${1}_extraTTH_m1s.log
