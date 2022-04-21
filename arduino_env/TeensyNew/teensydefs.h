#define PEDAL_PIN 13
#define DIR_POT   14

#define X_LIMIT  750
#define Y_LIMIT  670

#define POTMIN 520
#define POTMAX 540
#define POTAVG 530

#define BUFF_SIZE 50

#define angl      15

byte velMsg[10]{(byte)'G', 0, 0, 0, 0, 0, 0, 0, 0, 0x0A };

class Ramp
{
public:
  Ramp(int x, int y, int dir):
  xStart(x), yStart(y), dir(dir)
  {
    endX = (int)floor(len*cos((angl*71)/4068));
  }
  int checkPos(int x){
    return (int)floor(dir*(x-xStart)*tan(radians(angl))+ yStart);
  }

  int getX(){
    return xStart;
  }
  
private:
  int xStart;
  int yStart;
  int dir;
  const int len{700};
  int endX;
  
};
void idling(){
  Serial.println("Idled");
  Serial.flush();
  while(!digitalRead(PEDAL_PIN));
  Serial.print("ZA00000\n");
  
}
