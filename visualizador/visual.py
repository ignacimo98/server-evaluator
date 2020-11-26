import struct

def getData(path):
    file = open(path,"r")
    data = file.readline()

    for i in range (len(data)//chunk_size):
        cycles.append(data[0:chunk_size])
        data = data.replace(cycles[i],'')

    for i in range (len(cycles)):
        subcycle = []
        temp = cycles[i]

        #number of threads
        subcycle.append(int(temp[0:32],2))
        temp = temp.replace(temp[0:32],'')
        #number of cycles
        subcycle.append(int(temp[0:32],2))
        temp = temp.replace(temp[0:32],'')
        #total time of the cycles
        subcycle.append(bin_to_float(temp[0:32]))
        temp = temp.replace(temp[0:32],'')
        #average time of the cycles
        subcycle.append(bin_to_float(temp[0:32]))
        temp = temp.replace(temp[0:32],'')

        cycles[i] = subcycle

        print(cycles[i])


def bin_to_float(numBin):
    numBin = int(numBin,2)
    return struct.unpack('f', struct.pack('I', numBin))[0]

if __name__ == "__main__":
    chunk_size = 128
    cycles = []
    getData("cycles.bin")