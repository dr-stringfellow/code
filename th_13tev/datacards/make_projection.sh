for i in 5000 7500 10000 15000 30000 50000 100000 150000 300000 1000000 3000000; do 
echo ./lumi_datacards.sh 0 3m $i
echo ./lumi_datacards.sh 0 4m $i
echo ./lumi_datacards.sh 12 3m $i
echo ./lumi_datacards.sh 12 4m $i

done