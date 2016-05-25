def fFileExist(filename):
    '''Test if file exists'''
    try:
        oFile = open(filename,'r')
    except IOError:
        return 0
    else:
        oFile.close()
        return 1

filename = "tmp.txt"
if (fFileExist(filename) == 0):
	print 'missing ' + filename
else:
	fobj = open(filename, "r")	
	file = [] 
	for line in fobj:
		file.append(line)
	fout = open('csv.txt', "w")
	j=0
	for i in range(0,len(file)):
		if file[i].startswith('# x; y'):
			fout.write(str(j))
			fout.write("\t")
			j = j+1
			words=file[i+1].split()
			for i in range(0, len(words)):
				fout.write(words[i])
				fout.write("\t")
			fout.write("\n")

