class Measurement:
    def __init__(self, file_name):
        self.file_name = file_name
        self.cycle = []
        self.time = []

    def add_measurement(self,cycle,time):
        self.cycle.append(cycle)
        self.time.append(time)
        return self