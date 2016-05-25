less tmp_combine_0.txt | grep '2.5%' |  awk '{print $5}' > 2p5.dat
less tmp_combine_0.txt | grep '16.0%' |  awk '{print $5}' > 16p0.dat
less tmp_combine_0.txt | grep '50.0%' |  awk '{print $5}' > 50p0.dat
less tmp_combine_0.txt | grep '84.0%' |  awk '{print $5}' > 84p0.dat
less tmp_combine_0.txt | grep '97.5%' |  awk '{print $5}' > 97p5.dat
pr -mts' ' lumivalues.txt 2p5.dat 16p0.dat 50p0.dat 84p0.dat 97p5.dat > limits_0.dat
less tmp_combine_12.txt | grep '2.5%' |  awk '{print $5}' > 2p5.dat
less tmp_combine_12.txt | grep '16.0%' |  awk '{print $5}' > 16p0.dat
less tmp_combine_12.txt | grep '50.0%' |  awk '{print $5}' > 50p0.dat
less tmp_combine_12.txt | grep '84.0%' |  awk '{print $5}' > 84p0.dat
less tmp_combine_12.txt | grep '97.5%' |  awk '{print $5}' > 97p5.dat
pr -mts' ' lumivalues.txt 2p5.dat 16p0.dat 50p0.dat 84p0.dat 97p5.dat > limits_12.dat

