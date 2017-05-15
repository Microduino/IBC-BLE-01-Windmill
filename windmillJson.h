#include <aJSON.h>
aJsonStream serial_stream(&Serial);

String maxvalS = "";
String speedS = "";
String directionS = "";
String intervalS = "";
String projtctid = "";

String ID = "{\"ID\":\"590bc5e87bb3e300016e2d9d\"}";

void IDupload()
{
  Serial.print(ID.substring(0, 17));
  delay(30);
  Serial.println(ID.substring(17, 33));
  delay(30);
}

void Ctrldata(String _st, int _data)
{
  String send_data;
  int number;
  send_data = "{\"";
  send_data += _st;
  send_data += "\":\"";
  send_data += _data;
  send_data += "\"\}";
  number = send_data.length() / 17;

  if (number == 0)
  {
    Serial.println(send_data);
    delay(30);
  }
  else
  {
    while (number >= 0)
    {
      Serial.print(send_data.substring(0, 17));
      send_data = send_data.substring(17, send_data.length());
      delay(30);
      number--;
    }
    Serial.print("\n");
  }
}
//{"ID":"project"}
//{"micThreshold":"400"}
//{"speed":"200"}
//{"direction":"1"}
//{"interval":"5"}
void ComExecution(aJsonObject * msg)
{
  aJsonObject *IDval = aJson.getObjectItem(msg, "ID");
  if (IDval) {
    projtctid = IDval->valuestring;
    if (projtctid == "project")
    {
      IDupload();
      Ctrldata("micThreshold", VOICE_MIN);
      Ctrldata("speed", MOTOR_SPEED_MAX);
      Ctrldata("direction", DIRECTION);
      Ctrldata("interval", TIME_RUN);
    }
  }
  else {
#if DEBUG
    Serial.println("no ID data");
#endif
  }
  aJsonObject *maxval = aJson.getObjectItem(msg, "micThreshold");
  if (maxval) {
    maxvalS = maxval->valuestring;
    VOICE_MIN = maxvalS.toInt();
    allLedBlinkNum(1, 5, 100);
    write_eeprom();
  }
  else {
#if DEBUG
    Serial.println("no micThreshold data");
#endif
  }
  aJsonObject *speeddata = aJson.getObjectItem(msg, "speed");
  if (speeddata) {
    speedS = speeddata->valuestring;
    MOTOR_SPEED_MAX = speedS.toInt();
    allLedBlinkNum(1, 5, 100);
    write_eeprom();
  }
  else {
#if DEBUG
    Serial.println("no speed data");
#endif
  }
  aJsonObject *directiondata = aJson.getObjectItem(msg, "direction");
  if (directiondata) {
    directionS = directiondata->valuestring;
    DIRECTION = directionS.toInt();
    allLedBlinkNum(1, 5, 100);
    write_eeprom();
  }
  else {
#if DEBUG
    Serial.println("no direction data");
#endif
  }
  aJsonObject *runtimedata = aJson.getObjectItem(msg, "interval");
  if (runtimedata) {
    intervalS = runtimedata->valuestring;
    TIME_RUN = intervalS.toInt();
    allLedBlinkNum(1, 5, 100);
    write_eeprom();
  }
  else {
#if DEBUG
    Serial.println("no interval data");
#endif
  }
#if DEBUG
  Serial.println("--------------");
  Serial.print("ID DATA: ");
  Serial.println(projtctid);
  Serial.print("MIC DATA: ");
  Serial.println(VOICE_MIN);
  Serial.print("SPEED DATA: ");
  Serial.println(MOTOR_SPEED_MAX);
  Serial.print("DIRECTION DATA: ");
  Serial.println(DIRECTION);
  Serial.print("RUNTIME DATA: ");
  Serial.println(TIME_RUN);
  Serial.println("--------------");
#endif
}

void windmilljson_data()
{
  if (serial_stream.available()) {
    serial_stream.skip();
  }
  if (serial_stream.available()) {
    aJsonObject *msg = aJson.parse(&serial_stream);
    ComExecution(msg);
    aJson.deleteItem(msg);
  }
}

