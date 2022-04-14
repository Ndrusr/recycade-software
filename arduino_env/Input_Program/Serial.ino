void serial_comm(char msg[]){
  //while(Serial.availableForWrite()<8){}
  Serial.write(byte(90));
  Serial.write(msg);
  Serial.write("\n");

  return;
}

bool read_pedal(){
  const int BUFF_SIZE = 10;
  byte buff[BUFF_SIZE];
  if (Serial.available()){
    //Serial.println("Reading Serial");
    incomingByte = Serial.read();
    //Serial.println(incomingByte);
    
    if(incomingByte==90){
      int rlen = Serial.readBytesUntil('\n', buff, BUFF_SIZE);

      if (buff[0]==65){
        return true;
      }
    }    
  }

  return false;
  
  
}
