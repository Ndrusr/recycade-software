#define PEDAL_PIN 22
#define DIR_POT   14

#define X_LIMIT  750
#define Y_LIMIT  670

#define POTMIN 520
#define POTMAX 540
#define POTAVG 530

#define BUFF_SIZE 50

#define angl      15

byte velMsg[10]{(byte)'G', 0, 0, 0, 0, 0, 0, 0, 0, 0x0A };

int WORK;
#define debounceDelay 50 
unsigned long lastDebounceTime = 0;

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
  int lastState = LOW;
  while(true){
    if(Serial.available() >= 8){
      return;
    }
    WORK = digitalRead(PEDAL_PIN);
    if (WORK != lastState) {
    // reset the debouncing timer
      lastDebounceTime = millis();
    }
      if ((millis() - lastDebounceTime) > debounceDelay) {
        if(WORK){
          break;
        }
      }
    }
  Serial.print("ZA00000\n");
  return;
}
