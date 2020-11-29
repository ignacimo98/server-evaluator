import random
import struct

def openfile(path,cicles):
    binFile = open(path, "w")
    
    for i in range(cicles):
        binFile.write(generateCicle())

def generateCicle():
        cicle = int_to_bin(random.randint(1,100))
        cicle += int_to_bin(random.randint(1, 100))
        cicle += float_to_bin(random.random())
        cicle += float_to_bin(random.random())

        return cicle

# Funciones Auxiliares de Flaot y Binario
# Los numeros binarios se representan en el formato IEEE-754

def float_to_bin(numFloat):
    bits, = struct.unpack('!I', struct.pack('!f', numFloat))
    return "{:032b}".format(bits)

def bin_to_float(numBin):
    numBin = int(numBin,2)
    return struct.unpack('f', struct.pack('I', numBin))[0]

# Funciones Auxiliares de Int y Binario
# Los numeros binarios se representan en 32 bits

def int_to_bin(numInt):
    return '{:032b}'.format(numInt)


if __name__ == "__main__":
    openfile("cycles.bin",10)
