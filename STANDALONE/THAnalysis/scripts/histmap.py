import json 
import os
import shutil

def find(string, dir):
    myfile = open(dir+'/myvarlist.json', 'a+')
    try:
        myjson = json.load(myfile)
    except ValueError:
        myjson = []
    myfile.close()

    already = False
    for sublist in myjson:
        if string == sublist[0]:
            already = True

    return already

def map(string, dir, min, max, nbins):
    myfile = open(dir+'/myvarlist.json', 'a+')
    try:
        myjson = json.load(myfile)
    except ValueError:
        myjson = []

#    myfile.close()
    result = []

    already = False

    for sublist in myjson:
        if string == sublist[0]:
            result = sublist
            already = True

    if already == False:
        result = [string, min, max, nbins, string]
        myjson.append(result)

    myfile.close()            
    myfile2 = open(dir+'/myvarlist2.json', 'w')
    json.dump(myjson, myfile2, sort_keys=True, indent=0, separators=(',', ': '))
    myfile2.close()
    shutil.move(dir+'/myvarlist2.json', dir+'/myvarlist.json')
    return result




    
    
