#include <avr/pgmspace.h>
#include <Usb.h>
#include <usbhub.h>
#include <avr/pgmspace.h>
#include <hidboot.h>
#include <hiduniversal.h>
#include <Keyboard.h>
#include <Mouse.h>

//----------------------------------------//
USB Usb;
HIDUniversal               HidMouse(&Usb);

//-----------------------------------------------------------------------------------//
struct {
    uint8_t buttons;
    int8_t x;
    int8_t y;
} mouseReport;
//----------------------------------------//
const byte buffSize = 32;
char inputSeveral[buffSize];

byte maxChars;

int inputInt = 0;
float inputFloat = 0.0;
char inputCsvString[12];
int x;
int y;
int shoot;
//-----------------------------------------------------------------------------------//
void MoveMouseToXY(long x, long y) {
  long max = max(abs(x), abs(y));
  int count = (int) (max / 127);
  signed char stepX = x / (count + 1);
  signed char stepY = y / (count + 1);
  for (int i = 0; i < count; i++) {
    Mouse.begin();
    Mouse.move(stepX, stepY);
    Mouse.end();
  }
  signed char resX = x - (stepX * count);
  signed char resY = y - (stepY * count);
  if (resX != 0 || resY != 0) {
    Mouse.begin();
    Mouse.move(resX, resY);
    Mouse.end();
  }
}
//-----------------------------------------------------------------------------------//

class MouseRptParser : public MouseReportParser {
protected:
    virtual void OnMouseMove        (MOUSEINFO* mi);
    virtual void OnLeftButtonUp     (MOUSEINFO* mi);
    virtual void OnLeftButtonDown   (MOUSEINFO* mi);
    virtual void OnRightButtonUp    (MOUSEINFO* mi);
    virtual void OnRightButtonDown  (MOUSEINFO* mi);
    virtual void OnMiddleButtonUp   (MOUSEINFO* mi);
    virtual void OnMiddleButtonDown (MOUSEINFO* mi);
};
void MouseRptParser::OnMouseMove(MOUSEINFO* mi)        {
  MoveMouseToXY(mi->dX, mi->dY);
};
void MouseRptParser::OnLeftButtonUp(MOUSEINFO* mi)     {
  Mouse.begin();
  Mouse.release(MOUSE_LEFT);
  Mouse.end();
};
void MouseRptParser::OnLeftButtonDown(MOUSEINFO* mi)   {
  Mouse.begin();
  Mouse.press(MOUSE_LEFT);
  Mouse.end();
};
void MouseRptParser::OnRightButtonUp(MOUSEINFO* mi)    {
  Mouse.begin();
  Mouse.release(MOUSE_RIGHT);
  Mouse.end();
};
void MouseRptParser::OnRightButtonDown(MOUSEINFO* mi)  {
  Mouse.begin();
  Mouse.press(MOUSE_RIGHT);
  Mouse.end();
};
void MouseRptParser::OnMiddleButtonUp(MOUSEINFO* mi)   {
  Mouse.begin();
  Mouse.release(MOUSE_MIDDLE);
  Mouse.end();
};
void MouseRptParser::OnMiddleButtonDown(MOUSEINFO* mi) {
  Mouse.begin();
  Mouse.press(MOUSE_MIDDLE);
  Mouse.end();
};
MouseRptParser Prs;

void setup() {
  delay(5000);
  
  Mouse.begin();
  
  Serial.begin(115200);
  Serial.setTimeout(1);  
 
  Usb.Init();
  HidMouse.SetReportParser(0, &Prs);
}



//-----------------------------------------------------------------------------------//
void loop() {
    Usb.Task();
}
