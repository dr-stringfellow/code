def fFileExist(filename):
    '''Test if file exists'''
    try:
        oFile = open(filename,'r')
    except IOError:
        return 0
    else:
        oFile.close()
        return 1

filename = "tmp_0.txt"
if (fFileExist(filename) == 0):
	print 'missing ' + filename
else:
	fobj = open(filename, "r")	
	file = [] 
	for line in fobj:
		file.append(line)
	fout = open('limits.txt', "w")
	j=0
	for i in range(0,len(file)):
		if file[i].startswith('Observed Limit:'):
			fout.write(str(j))
                        fout.write("\n")
			fout.write(file[i])
			fout.write(file[i+1])
			fout.write(file[i+2])
			fout.write(file[i+3])
			fout.write(file[i+4])
			fout.write(file[i+5])
			j = j+1
			fout.write("\n")

