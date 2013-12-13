#combine -M HybridNew datacard_thq_leptonic_noLDcut.txt --significance  --expectedFromGrid=0.5  --saveToys --fullBToys --saveHybridResult -T 2000 -i 10 -s13 >&! log_signif_thq_leptonic_noLDcut.log
combine -M HybridNew datacard_thq_leptonic.txt --significance  --expectedFromGrid=0.5  --saveToys --fullBToys --saveHybridResult -T 2000 -i 10 -s13 >&! log_signif_thq_leptonic.log
combine -M HybridNew datacard_thq_hadronic.txt --significance  --expectedFromGrid=0.5  --saveToys --fullBToys --saveHybridResult -T 2000 -i 10 -s13 >&! log_signif_thq_hadronic.log
combine -M HybridNew datacard_thq_hadronic_metCut.txt --significance  --expectedFromGrid=0.5  --saveToys --fullBToys --saveHybridResult -T 2000 -i 10 -s13 >&! log_signif_thq_hadronic_metCut.log
