
cat datacard_thq_leptonic.txt | sed 's/observation 0/observation 1/' >! datacard_thq_leptonic_obs1.txt
cat datacard_thq_leptonic.txt | sed 's/observation 0/observation 2/' >! datacard_thq_leptonic_obs2.txt
cat datacard_thq_leptonic.txt | sed 's/observation 0/observation 3/' >! datacard_thq_leptonic_obs3.txt
cat datacard_thq_leptonic.txt | sed 's/gmN 1/gmN 0/' >! datacard_thq_leptonic_0events.txt
cat datacard_thq_leptonic_0events.txt | sed 's/observation 0/observation 2/' >! datacard_thq_leptonic_0events_obs2.txt
cat datacard_thq_hadronic.txt | sed 's/observation 0/observation 4/' >! datacard_thq_hadronic_obs4.txt

#combine -M HybridNew datacard_thq_leptonic_0events_obs2.txt --significance --saveToys --fullBToys --saveHybridResult -T 10000 -i 13 -s13 >&! log_signif_obs2_leptonicChannel_0events.log
combine -M HybridNew datacard_thq_leptonic_obs1.txt --significance --saveToys --fullBToys --saveHybridResult -T 10000 -i 13 -s13 >&! log_signif_obs1_leptonicChannel.log
combine -M HybridNew datacard_thq_leptonic_obs2.txt --significance --saveToys --fullBToys --saveHybridResult -T 10000 -i 13 -s13 >&! log_signif_obs2_leptonicChannel.log
combine -M HybridNew datacard_thq_leptonic_obs3.txt --significance --saveToys --fullBToys --saveHybridResult -T 10000 -i 13 -s13 >&! log_signif_obs3_leptonicChannel.log
#combine -M HybridNew datacard_thq_hadronic_obs4.txt --significance --saveToys --fullBToys --saveHybridResult -T 10000 -i 13 -s13 >&! log_signif_obs4_hadronicChannel.log

