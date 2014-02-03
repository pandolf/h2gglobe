
cat datacard_thq_leptonic_${1}.txt | sed 's/observation 0/observation 1/' >! datacard_thq_leptonic_${1}_obs1.txt
cat datacard_thq_leptonic_${1}.txt | sed 's/observation 0/observation 2/' >! datacard_thq_leptonic_${1}_obs2.txt
cat datacard_thq_leptonic_${1}.txt | sed 's/observation 0/observation 3/' >! datacard_thq_leptonic_${1}_obs3.txt
cat datacard_thq_leptonic_${1}.txt | sed 's/gmN 1/gmN 0/' >! datacard_thq_leptonic_${1}_0events.txt
cat datacard_thq_leptonic_${1}_0events.txt | sed 's/observation 0/observation 2/' >! datacard_thq_leptonic_${1}_0events_obs2.txt
cat datacard_thq_hadronic_${1}.txt | sed 's/observation 0/observation 4/' >! datacard_thq_hadronic_${1}_obs4.txt

#combine -M HybridNew datacard_thq_leptonic_${1}_0events_obs2.txt --significance --saveToys --fullBToys --saveHybridResult -T 10000 -i 13 -s13 >&! log_signif_obs2_leptonic_${1}_0events.log
combine -M HybridNew datacard_thq_leptonic_${1}_obs1.txt --significance --saveToys --fullBToys --saveHybridResult -T 10000 -i 13 -s13 >&! log_signif_obs1_leptonic_${1}.log
combine -M HybridNew datacard_thq_leptonic_${1}_obs2.txt --significance --saveToys --fullBToys --saveHybridResult -T 10000 -i 13 -s13 >&! log_signif_obs2_leptonic_${1}.log
combine -M HybridNew datacard_thq_leptonic_${1}_obs3.txt --significance --saveToys --fullBToys --saveHybridResult -T 10000 -i 13 -s13 >&! log_signif_obs3_leptonic_${1}.log
#combine -M HybridNew datacard_thq_hadronic_${1}_obs4.txt --significance --saveToys --fullBToys --saveHybridResult -T 10000 -i 13 -s13 >&! log_signif_obs4_hadronic_${1}.log

