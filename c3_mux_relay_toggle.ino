#include <EEPROM.h>
#define OE 6   //output enable
#define STCP 7 //latch
#define SHCP 8 //clock
#define DS 9   //data


//all off = 
//byte reg_0 = 0x4C; // 0100 1100
//byte reg_1 = 0x5E; // 0101 1100 
//byte reg_2 = 0x00; // 0000 0000

String state = "initial";
struct Params{
  int id;
  bool loop;
  int delay;
  int relay;
  int clicks;
  int autostart;
};
Params params;
Params defaults = {0x55,false,100,0,1,0};
void setup() {
  pinMode(OE,OUTPUT);
  pinMode(STCP,OUTPUT);
  pinMode(SHCP,OUTPUT);
  pinMode(DS,OUTPUT);
  digitalWrite(OE, HIGH);
  
  Serial.begin(115200);
  Serial.println("Star C3 MUX relay toggler");
  Serial.println("send <command>=<value> to change paramaters");
  Serial.println("commands: go, stop, defaults, params");
  if (EEPROM.read(0) == 0x55){
    EEPROM_readAnything(0, params); 
  }else{
     loadDefaults();
  }
}


void loadDefaults(){
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0xff);
  }
  Serial.println("\ndefaults loaded");
  showParams();
  EEPROM_writeAnything(0, defaults);
  params = defaults;
}

byte data[39][3] = {{0x00,0x00,0x00},{0xCC,0x56,0x00},{0x8C,0x5E,0x00},{0x0C,0x7E,0x00},{0x2C,0x5E,0x00},{0x1C,0x5E,0x00},{0x0E,0x5E,0x00},{0x4C,0x5B,0x00},{0x4C,0x4F,0x00},{0x4C,0xCE,0x00},{0x4C,0x4E,0x01},{0x4C,0x4E,0x40},{0x4D,0x4E,0x00},{0x6C,0x56,0x00},{0x44,0x7E,0x00},{0x5C,0x56,0x00},{0x4C,0x5A,0x01},{0x4C,0x9E,0x00},{0x4C,0x5A,0x40},{0x4A,0x5E,0x00},{0x58,0x5E,0x00},{0x68,0x5E,0x00},{0xC8,0x5E,0x00},{0x4D,0x5C,0x00},{0x4C,0x5C,0x40},{0x4C,0x5C,0x01},{0x4C,0x5D,0x00},{0x4C,0x5E,0x10},{0x48,0x7E,0x00},{0x46,0x5E,0x00},{0xC4,0x5E,0x00},{0x54,0x5E,0x00},{0x64,0x5E,0x00},{0x4C,0x5E,0x20},{0x4C,0xDC,0x00},{0x4D,0x1E,0x00},{0x4C,0x1F,0x00},{0x4C,0x1E,0x40},{0x4C,0x1E,0x01}};
//byte data[74][3] = {{0x0C,0x08,0x4C},{0x0C,0x5E,0x4C},{0x1C,0x5E,0xC4},{0x3C,0x1F,0xC4},{0x2C,0x1F,0x68},{0x0D,0x5C,0x68},{0x0D,0x5C,0x58},{0x4C,0x5C,0x58},{0x5C,0x5C,0x58},{0x7C,0x5C,0x58},{0x6C,0x7C,0x0C},{0x0C,0xEE,0x0C},{0x1C,0xEE,0x0C},{0x3C,0xEE,0x0C},{0x2C,0xCE,0x4A},{0x0C,0x5C,0x4B},{0x1C,0x5C,0x4B},{0x3C,0x5C,0x4B},{0x3C,0x5C,0x47},{0x3C,0x1E,0x47},{0x2C,0x3E,0x49},{0x0C,0xFC,0x48},{0x0C,0xDC,0x46},{0x0C,0x1E,0x47},{0x1C,0x3E,0x49},{0x3C,0xFC,0x48},{0x2C,0xDC,0x54},{0x4C,0x1E,0x54},{0x5C,0x1E,0x54},{0x7C,0x1E,0x54},{0x6C,0x1E,0xC8},{0x0C,0x5D,0xC8},{0x1C,0x5D,0xC8},{0x3C,0x5D,0xC8},{0x2C,0x5D,0xC4},{0x0C,0x1F,0xC4},{0x0C,0x1F,0x1C},{0x4C,0x4E,0x1C},{0x5C,0x4E,0x1C},{0x7C,0x4E,0x1C},{0x6C,0x4E,0x0E},{0x0C,0x4E,0x0F},{0x1C,0x4E,0x0F},{0x3C,0x4E,0x0F},{0x2C,0x4E,0x65},{0x0D,0x1E,0x64},{0x1D,0x1E,0x64},{0x3D,0x1E,0x64},{0x2D,0x16,0x5C},{0x4C,0x52,0x5C},{0x5C,0x52,0x5C},{0x7C,0x52,0x5C},{0x6C,0x52,0xCC},{0x0C,0x53,0xCC},{0x1C,0x53,0xCC},{0x3C,0x53,0xCC},{0x2C,0x7B,0x44},{0x0C,0xBE,0x44},{0x1C,0xBE,0x44},{0x3C,0xBE,0x44},{0x2C,0x96,0x6C},{0x0D,0x52,0x6C},{0x0D,0x5A,0x8C},{0x0C,0x4F,0x8C},{0x1C,0x4F,0x8C},{0x3C,0x4F,0x8C},{0x2C,0x4F,0x2C},{0x0D,0x4E,0x2C},{0x1D,0x4E,0x2C},{0x3D,0x4E,0x2C},{0x3D,0x4E,0x68},{0x3D,0x5C,0x68},{0x00,0x08,0x4C},{0x00,0x5E,0x4C}};


void loop() {
  if(state == "stop") return;
  if(state == "initial" && !params.autostart) return;
  if(params.relay == 0) toggleAll();
  if(params.relay > 0) toggleRelay(params.relay);
  if(!params.loop){
    state = "stop";
    Serial.println("\nStop\n");
  }
}


void showParams(){
  Serial.print("\ndelay=");
  Serial.print(params.delay,DEC);
  Serial.println("  min: 100, max:5000");
  Serial.print("loop=");
  Serial.print( (params.loop)?"true":"false");
  Serial.println("  true/false");
  Serial.print("autostart=");
  Serial.print( (params.autostart)?"true":"false");
  Serial.println("  true/false");
  Serial.print("relay=");
  Serial.print(params.relay);
  Serial.println("  0=all/ min:0 max:38");
  Serial.print("clicks=");
  Serial.print(params.clicks, DEC);
  Serial.println("  min:1 max:10\n");
}



void serialEvent(){
  String cmd;
  while (Serial.available()){
    cmd = Serial.readString();
    cmd.trim();   
  }
  int eq = cmd.indexOf("=");
  if(eq > -1){
      //edit params
      String param = cmd.substring(0,eq);
      String value = cmd.substring(eq+1);
      if(param == "delay") setDelayTo(value);
      if(param == "loop") setLoopTo(value);
      if(param == "relay") setRelayTo(value);    
       if(param == "clicks") setClicksTo(value);  
      if(param == "autostart") setAutostartTo(value);
      if(state == "stop" || state == "initial") showParams();
    }
    else if(cmd == "go") state = "go";
    else if(cmd == "stop"){
     state = "stop";
      Serial.println("\nStop\n");
    }
    else if(cmd == "defaults") loadDefaults();
    else if(cmd == "params") showParams();
    else Serial.println("unknown command: "+cmd);
    EEPROM_writeAnything(0, params);
}

void setDelayTo(String value){
    int dly = value.toInt();
    if(dly < 100)dly = 100;
    if(dly > 5000)dly = 5000;
    params.delay = dly;
}

void setLoopTo(String value){
    if(value == "true"){
      params.loop = true;
    }
    if(value == "false"){
      params.loop = false;
    }
}

void setRelayTo(String value){
    int val = value.toInt();
    if(val < 0) value = "0";
    if(val > 38) value = 38;
    params.relay = val; 
}

void setClicksTo(String value){
    int val = value.toInt();
    if(val < 1) val = 1;
    if(val > 10) val = 10;
    params.clicks = val;
}
void setAutostartTo(String value){
    if(value == "true")params.autostart = true;
    if(value == "false")params.autostart = false;
}





void clickClack(int clicks,int pauseLength){
    for(int i = 0; i < clicks; i++){
      delay(pauseLength);
      digitalWrite(OE, LOW);
      delay(pauseLength);
      digitalWrite(OE, HIGH);
      delay(pauseLength);
      if(Serial.available()) break;
    }
    delay(params.delay);
}

void toggleAll(){
    for(int i = 1; i < 39; i++){
       toggleRelay(i);
       if(Serial.available()) break;
    }
}

void toggleRelay(int relay){
  Serial.print("toggle: ");
  Serial.println(relay,DEC);
  digitalWrite(STCP, LOW);
  shiftOut(DS,SHCP,MSBFIRST, 0x00);
  shiftOut(DS,SHCP,MSBFIRST, 0x00);
  shiftOut(DS,SHCP,MSBFIRST, 0x00);
  shiftOut(DS,SHCP,MSBFIRST, 0x00);
  shiftOut(DS,SHCP,MSBFIRST, data[relay][2]);
  shiftOut(DS,SHCP,MSBFIRST, data[relay][1]);
  shiftOut(DS,SHCP,MSBFIRST, data[relay][0]);
  digitalWrite(STCP, HIGH);
  clickClack(params.clicks, 100);
}

template <class T> int EEPROM_writeAnything(int ee, const T& value)
{
    const byte* p = (const byte*)(const void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
          EEPROM.write(ee++, *p++);
    return i;
}

template <class T> int EEPROM_readAnything(int ee, T& value)
{
    byte* p = (byte*)(void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
          *p++ = EEPROM.read(ee++);
    return i;
}
