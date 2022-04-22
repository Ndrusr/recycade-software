import serial 

serTeensy = serial.Serial('/dev/ttyACM0', 115200, timeout = 2)
serUNO = serial.Serial('/dev/ttyUSB1', 9600)
serMega = serial.Serial('/dev/ttyUSB0', 9600, timeout = 2)

actionmsg = b"ZA00000\n"
pushmsg = b"ZB00000\n"
gamemsg = b"ZG00000\n"

idlemsg = b"ZI00000\n"
acceptmsg = b"Zaccept\n"
rejectmsg = b"Zreject\n"

def game():
    serMega.write(gamemsg)
    serTeensy.write(gamemsg)
    megaRead = serMega.readline()
    teensyRead = serTeensy.readline()
    if(megaRead == b"ZG10000\n" and teensyRead == b"ZG10000\n"):
        print("Game running")
        while(megaRead != b"ZG20000\n" and megaRead != b"ZG30000\n"):
            megaRead = serMega.readline()
            while len(megaRead) < 8:
                megaRead = serMega.readline()
                print(megaRead)
            if(megaRead == b"ZG20000\n"):
                print("GAME OVER")
                break
            elif(megaRead == b"ZG30000\n"):
                print("GAME OVER. YOU WIN!")
                break
            if(megaRead[0] == b'G'):
                serTeensy.write(megaRead)
                print("mega -> teensy")
                while(teensyRead[0] != b'G'):
                    teensyRead = serTeensy.readline()
                    
                    if(teensyRead[0] == b'G'):
                        serMega.write(teensyRead)
                        print("teensy->mega")
                    else:
                        print(teensyRead)
            
            else:
                print(megaRead)
            
    else:
        print("ERROR: RESETTING MACHINE!")
        print(megaRead)
        print(teensyRead)
    serTeensy.write(b"ZG00000\n")
    
    serTeensy.flush()
    serMega.flush()


# unoRead = serUNO.read_until('\n')
isTeensyIdle = False

print("Cycle begin")

while(True):
    if not isTeensyIdle:
        serTeensy.write(idlemsg)
        isTeensyIdle = True
        
    teensyRead = serTeensy.readline()
    megaRead = serMega.readline()

    if(teensyRead == actionmsg):
        serMega.flush()
        serTeensy.flush()
        isTeensyIdle = False
        print("serial get!")
        serUNO.write(actionmsg)
        unoRead = serUNO.readline()
        while(not (unoRead[0] == 'Z' and len(unoRead) == 8)):
            unoRead = serUNO.readline()
            if(unoRead == b"Zcan000\n" or unoRead == b"Zbottle\n" or unoRead == rejectmsg):
                print("signal get")
                if(unoRead == b"Zcan000\n" or unoRead == b"Zbottle\n"):
                    if(unoRead == b"Zcan000\n"):
                        print("can")
                        serMega.write(unoRead)
                    elif(unoRead == b"Zbottle\n"):
                        print("bottle")
                    serMega.write(unoRead)
                    serMega.write(pushmsg)
                    print('accepting')
                    while(megaRead != b"ZB10000\n"):
                        megaRead = serMega.readline()
                        
                    game()
                    break
                else:
                    serMega.write(b"Z000000\n")
                    print('rejected')
                    break
                
            else:
                print(unoRead)
    else:
        print(megaRead)
        print(teensyRead)



    

