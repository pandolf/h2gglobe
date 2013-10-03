
cat datacard_thq_leptonic.txt | sed 's/observation 0/observation 2/' >! datacard_thq_leptonic_obs2.txt
cat datacard_thq_hadronic.txt | sed 's/observation 0/observation 2/' >! datacard_thq_hadronic_obs2.txt

combine -M HybridNew datacard_thq_leptonic_obs2.txt --significance --saveToys --fullBToys --saveHybridResult -T 10000 -i 13 -s13 >&! log_signif_obs2_leptonicChannel.log
combine -M HybridNew datacard_thq_hadronic_obs2.txt --significance --saveToys --fullBToys --saveHybridResult -T 10000 -i 13 -s13 >&! log_signif_obs2_hadronicChannel.log

