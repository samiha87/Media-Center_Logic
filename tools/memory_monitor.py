import subprocess
import time
from datetime import datetime

command = "top -n3 | grep AV_Logic"
# memory data and timestam
memory_list = []
datetime_list = []

# Analyzes array and checks if major increase for memory has happen
def analyzeArray(list1, list2):
    if len(list2) < 10:
        return
    first_data_point = list1[0]
    last_data_point = list1[len(list1) - 1]
    first_date = list2[0]
    last_date = list2[len(list2) - 1]
    data1 = float(first_data_point)
    data2 = float(last_data_point)
    change = 1 - (data1/data2)
    if change > 0.1 :
        print("Memory leak")
        f = open("log.txt", "w+")
        f.write("We have memory leak, change = " + str(change) + str(datetime.now()))
        f.close()

while 1:
    storage = []
    try:
        res = subprocess.check_output(['bash', '-c', command])
    except:
        continue
    linex = res.splitlines()
    for line in linex[0].split(" "):
        if line != "":
            storage.append(line)
            # Store Physical memory and a time stamp, RES, fifth from parsed
    line = linex[0].split(" ")
    memory_list.append(storage[6])
    datetime_list.append(datetime.now())
    time.sleep(2)
    analyzeArray(memory_list, datetime_list)
