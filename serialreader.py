import serial
import time
import csv

ser = serial.Serial('COM3', '9600', timeout=1)              #Tells ser where to look for the serial data
time.sleep(2)

data =[]                                                    # empty list to store the data
for i in range(5):

    b = ser.readline()                                      # read a byte string
    string_n = b.decode()                                   # decode byte string into Unicode  
    string = string_n.rstrip()                              # remove \n and \r

                                                            #variable 'string' now contains one row of data.

    data.append(str(time.time()) + ','+ string)             #appends that row of data to a list containing all data collected so far

    time.sleep(1)
ser.close()                                                 #closes the connection to the port to make it accessible from other programs



with open('collected_data.csv', 'w', newline='') as file:   #creates a CSV file to write into
    writer = csv.writer(file)                               
    writer.writerow(["time,temperature,pressure,humidity"]) 
    for i in range(len(data)):                              #writes each line of data into the CSV file, 1 at a time.
        print(data[i])
        
        writer.writerow([data[i]])
