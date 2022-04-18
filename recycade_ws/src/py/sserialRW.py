import serial 

serUNO = serial.Serial('/dev/ttyUSB1', 9600)
serMega = serial.Serial('/dev/ttyUSB0', 9600)

actionmsg = "ZA00000\n"
pushmsg = "ZB00000\n"
acceptmsg = "Zaccept"
rejectmsg = "Zreject"

print("Cycle begin")

while(True):
    
    megaRead = serMega.readline()

    if(megaRead == actionmsg):
        print("serial get!")
        serUNO.write(actionmsg)
        while(not unoRead[0] == 'Z' and len(unoRead) == 7):
            unoRead = serUNO.readline()
            if(not unoRead[0] == 'Z'):
                print(unoRead)
            elif(len(unoRead) == 7 ):
                print("signal get")
                if(unoRead == acceptmsg):
                    serMega.write(pushmsg)
                    print('accepting')
                    break
                else:
                    print('rejected')
                    break
    else:
        print(megaRead)



    
