for i in {0..50}; do 
    grep "nll S+B" nll_${i}.log > nlltemp 
    awk 'BEGIN { printf "%10s %10s","Column1:","Ratio:\n" } nlltemp
        { printf "%10s %10f\n" ,$1, ($4/$8) }'
done

    