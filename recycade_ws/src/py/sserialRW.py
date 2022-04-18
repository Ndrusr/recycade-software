import serial 

serUNO = serial.Serial('/dev/ttyUSB1', 9600)
serMega = serial.Serial('/dev/ttyUSB0', 9600)

megaRead = bytes(7)
unoRead = bytes(7)

actionmsg = "ZA00000\n"
pushmsg = "ZB00000\n"
acceptmsg = "Zaccept"
rejectmsg = "Zreject"

while(True):
    print("Cycle begin")
    megaRead = serMega.readline()

    if(megaRead == "ZA00000"):
        serUNO.write(actionmsg)
        while(not unoRead[0] == 'Z'):
            unoRead = serUNO.readline()
        if()
        serMega.write(pushmsg)



    
