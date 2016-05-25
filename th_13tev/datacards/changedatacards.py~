def fFileExist(filename):
    '''Test if file exists'''
    try:
        oFile = open(filename,'r')
    except IOError:
        return 0
    else:
        oFile.close()
        return 1

filename = "tH_comb.txt"
if (fFileExist(filename) == 0):
	print 'missing ' + filename
else:
	fobj = open(filename, "r")	
	file = [] 
	for line in fobj:
		file.append(line)
	fobj.close()
	fout = open('tH_comb.txt', "w")
	for i in range(0,len(file)):
		words=file[i].split()
		if len(words)>1:
			if words[1] == "shape":
				fout.write(words[0])
				fout.write("\t")		
				fout.write(words[1])
				for i in range(2, len(words)):
					fout.write("\t0.5")
				fout.write("\n")
			else:
				fout.write(file[i])
#			for i in range(0, len(words)):
#				fout.write(words[i])
#				fout.write("\t")
#			fout.write("\n")
#		if file[i].startswith('[INFO]  workdir') and i!=len(file)-1:
#			out = int(file[i+1].split()[0])/1000.
#		        fout.write(str(out))
#		        fout.write("\t")

