import processing.serial.*;       

int xpos=90; // set x servo's value to mid point (0-180);
int ypos=90; // and the same here
String string;
Serial myPort; // The serial port we will be using

void setup()
{
  size(510, 510);
//  frameRate(100);
  myPort = new Serial(this, Serial.list()[0], 9600);
  println(Serial.list());
  delay(1000);
}

void draw()
{
  fill(175);
  rect(0,0,510,510);
  fill(255,0,0); //rgb value so RED
  rect(0, 500, mouseX, 10); //xpos, ypos, width, height
  fill(0,255,0); // and GREEN
  rect(0, 500, 10, mouseY-510);
  sendPos((100+mouseX)/5, (510-mouseY)/4);
//  MouseControl2();
}

void sendPos(int _posY, int _posZ)
{
  println( "Stretch: " + _posY + "  Height: " + _posZ);
  byte[] send = 
  {
    byte(0xFF),
    byte(0xAA),
    byte(_posY),
    byte(_posZ)
  };
     myPort.write(0xFF);
     delay(30);
     myPort.write(0xAA);
     delay(30);
     myPort.write(_posY);
     delay(30);
     myPort.write(_posZ);
     delay(30);
}

void MouseControl2()
{
  if(myPort.available() > 0)
  {
    int rxBuff = myPort.read();
    println(rxBuff);
  }
}
