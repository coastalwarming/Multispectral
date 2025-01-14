void Update_Temp() {
  if (millis() - lastTempRequest >= 2000) // waited long enough??
  {
    temperature = sensors.getTempCByIndex(0);

    /*
        Serial.print(" Temperature: ");
        Serial.println(temperature, 1);
    */
    sensors.requestTemperatures();//Ou entao peço outra medida a 12bits.
    lastTempRequest = millis(); //Atualizo o instante em que pedi.

    if (Prev_temp == -1) {//So é -1 quando fez reset.
      display.setTextColor(WHITE);
      display.setTextSize(1);
      //display.setCursor(32, 55);
      display.setCursor(32, 55);
      display.println("C");
      display.display();
    }


    if (Prev_temp != temperature) {
      //Apagar Temp:
      if ((Prev_temp > 84 ) || (Prev_temp < -120)) {
        display.setTextColor(BLACK);
        display.setTextSize(1);
        display.setCursor(5, 55);
        display.println("XXX");
        display.display();
      } else {
        display.setTextColor(BLACK);
        display.setTextSize(1);
        display.setCursor(5, 55);
        display.println(Prev_temp, 1);
        display.display();
      }
      //Escrever Temp:
      if ((temperature > 84 ) || (temperature < -120)) {
        display.setTextColor(WHITE);
        display.setTextSize(1);
        display.setCursor(5, 55);
        display.println("XXX");
        display.display();
      } else {
        display.setTextColor(WHITE);
        display.setTextSize(1);
        display.setCursor(5, 55);
        display.println(temperature, 1);
        display.display();

      }
      Prev_temp = temperature;

    }

    FAN_Control(temperature);

  }

}

void FAN_Control(float temperature) {
  static int Fan_perc = 0;
  if (Stage == 2) {
     digitalWrite(FAN_GPIO, HIGH);
    
  //  if (temperature > profileSetpoint[Index_letter_screen]) {
  //    digitalWrite(FAN_GPIO, HIGH);
      /* Fan_perc = Fan_perc + 1;
            if (Fan_perc > 100)Fan_perc = 100;
            Serial.print("Fan_perc++ -> : "); Serial.println(Fan_perc);*/
  //  } else digitalWrite(FAN_GPIO, LOW);





    /*
        if ((temperature < ((profileSetpoint[Index_letter_screen]) - 0.5)) && Fan_perc > 0) {
          Fan_perc = Fan_perc - 1;
          if (Fan_perc < 0)Fan_perc = 0;
          Serial.print("Fan_perc-- -> : "); Serial.println(Fan_perc);

        }
    */
    /*
    Serial.print("temperature: "); Serial.println(temperature);
    Serial.print("Fan_perc: "); Serial.println(Fan_perc);

    int gap = map(Fan_perc, 0, 100, 0, 255);
    if (gap > 255)gap = 255;
    if (gap < 1)gap = 0;

    Serial.print("gap(0-255): "); Serial.println(gap);

    ledcWrite(ledChannel, gap);
    */
    //
  } else digitalWrite(FAN_GPIO, LOW);
}
