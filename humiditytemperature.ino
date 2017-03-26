void currTempHumidity() {


  // start working...
  Serial.println("=================================");
  Serial.println("Sample DHT11...");

  // read without samples.
  byte temperature = 0;
  byte humidity = 0;
  if (dht11.read(pinDHT11, &temperature, &humidity, NULL)) {
    Serial.print("Read DHT11 failed.");
    return;
  }

  Serial.print("Sample OK:");
  Serial.print((int)temperature); Serial.print(" *C, ");
  Serial.print((int)humidity); Serial.println(" %");
  //
  String temp = String(temperature);
  //
  String humi = String(humidity);

  if (!client.connected()) {
    Serial.print("trying to publish after connecting...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("esp8266", "hello world");
    }
  } else {
    Serial.print("already connected and trying  to publish ...");
    client.publish("temperature", temp);
    client.publish("humidity", humi);

  }

  //  client.publish("temperature", (char *)temperature);
  //  client.publish("humidity", (char *)humidity);

  // DHT11 sampling rate is 1HZ.
  //delay(3000);
  //webSocket.broadcastTXT("ConnectionAlive");
  //webSocket.broadcastTXT("Temperature " + (String)temperature + "Humidity " + (String)humidity);

}
