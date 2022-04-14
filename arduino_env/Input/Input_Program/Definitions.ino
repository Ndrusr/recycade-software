void openDoor(int m_speed){
  Serial.print("Motors Moving... Speed:");
  Serial.println(m_speed);
  analogWrite(enA, m_speed);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  delay(5000);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
}

void closeDoor(int m_speed){
  analogWrite(enA, m_speed);
  Serial.println("Door Closing...");
  while(digitalRead(door)){
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  }
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
}
