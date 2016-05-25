less limits.txt | grep '2.5%' |  awk '{print $5}' > 2p5.dat
less limits.txt | grep '16.0%' |  awk '{print $5}' > 16p0.dat
less limits.txt | grep '50.0%' |  awk '{print $5}' > 50p0.dat
less limits.txt | grep '84.0%' |  awk '{print $5}' > 84p0.dat
less limits.txt | grep '97.5%' |  awk '{print $5}' > 97p5.dat
less limits.txt | grep 'Observed' |  awk '{print $5}' > observed.dat
pr -mts',' cfvalues.txt 2p5.dat 16p0.dat 50p0.dat 84p0.dat 97p5.dat observed.dat > limits.dat
echo "cf,twosigdown,onesigdown,exp,onesigup,twosigup,obs" > limits_1.dat
cat limits.dat >>limits_1.dat