
void notFound(AsyncWebServerRequest *request){request->send(404, "text/html", errorCode);}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch (type) 
  {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED: {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
      }
      break;
    case WStype_TEXT:
      Serial.printf("[%u] get Text: %s\n", num, payload);
      String message = String((char*)( payload));
      Serial.println(message);
     DynamicJsonDocument doc(200);
    // deserialize the data
    DeserializationError error = deserializeJson(doc, message);
    // parse the parameters we expect to receive (TO-DO: error handling)
      // Test if parsing succeeds.
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }else{
    if(doc["relay_1"] == 0){
      digitalWrite(relay_1, 0);
      delay(10);
      sendRelayData(1, 0);
    }else if(doc["relay_1"] == 1){
      digitalWrite(relay_1, 1);
      delay(10);
      sendRelayData(1, 1);
    }
    if(doc["relay_2"] == 0){
      digitalWrite(relay_2, 0);
      delay(10);
      sendRelayData(2,0);
    }else if(doc["relay_2"] == 1){
      digitalWrite(relay_2, 1);
      delay(10);
      sendRelayData(2,1);
    }
    if(doc["relay_3"] == 0){
      digitalWrite(relay_3, 0);
      delay(10);
      sendRelayData(3,0);
    }else if(doc["relay_3"] == 1){
      digitalWrite(relay_3, 1);
      delay(10);
      sendRelayData(3,1);
    }
    if(doc["relay_4"] == 0){
      digitalWrite(relay_4, 0);
      delay(10);
      sendRelayData(4,0);
    }else if(doc["relay_4"] == 1){
      digitalWrite(relay_4, 1);
      delay(10);
      sendRelayData(4,1);
    }
    }
  }
}

void sendRelayData(int relayNumber, int Status){
  String JSON_Data = "{\"relay_";
         JSON_Data += relayNumber;
         JSON_Data += "\":";
         JSON_Data += Status;
         JSON_Data += "}";
  Serial.print("From esp to send all end point ");
  Serial.println(JSON_Data);
  webSocket.broadcastTXT(JSON_Data);
}

void sendData(){
  // JSON_Data = {"relay_1": 0,"relay_2":0,"relay_3":0,"relay_4":0}
  String JSON_Data = "{\"relay_1\":";
         JSON_Data += digitalRead(relay_1);
         JSON_Data += ",\"relay_2\":";
         JSON_Data += digitalRead(relay_2);
         JSON_Data += ",\"relay_3\":";
         JSON_Data += digitalRead(relay_3);
         JSON_Data += ",\"relay_4\":";
         JSON_Data += digitalRead(relay_4);
         JSON_Data += "}";
  Serial.println(JSON_Data);
  webSocket.broadcastTXT(JSON_Data);
}
