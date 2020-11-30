from M_Thread import M_Thread
import struct

class Data_Manager:
    def __init__(self):
        self.M_Thread_Array = []

    def add_measurement(self, num_thread, num_cycles, run_time, file_name):
        found = None
        for i in self.M_Thread_Array:
            if (i.id == num_thread):
                found = i
            
        if (found):
            found.add_measurement(num_cycles,run_time,file_name)
        else:
            self.M_Thread_Array.append(M_Thread(num_thread).add_measurement(num_cycles,run_time,file_name))
    
    def get_data(self,path):
        num_thread = 0
        num_cycles = 0
        run_time = 0

        with open(path,"rb") as file:
            counter = 0
            data = file.read(4)

            while data != b'':
                if (counter == 1):
                    # print(struct.unpack('i',data)[0])
                    num_cycles = struct.unpack('i',data)[0]

                    data = file.read(8)
                    counter += 1
                elif (counter == 2):
                    # print(struct.unpack('d',data)[0])
                    run_time = struct.unpack('d',data)[0]

                    #procesar los datos aqui
                    self.add_measurement(num_thread,num_cycles,run_time,path)

                    data = file.read(4)
                    counter = 0
                else:
                    # print(struct.unpack('i',data)[0])
                    num_thread = struct.unpack('i',data)[0]

                    data = file.read(4)
                    counter += 1

        return self