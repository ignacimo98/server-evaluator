from Measurement import Measurement

class M_Thread:
    def __init__(self, id):
        self.id = id
        self.measurements = []


    def add_measurement(self,cycle,time,file_name):
        found = None

        for i in self.measurements:
            if i.file_name == file_name:
                found = i
        
        if (found):
            found.add_measurement(cycle, time)
        else:
            self.measurements.append(Measurement(file_name).add_measurement(cycle,time))
        
        return self