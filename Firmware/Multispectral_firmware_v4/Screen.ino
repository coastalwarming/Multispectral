void Print_Screen_0() {
  static byte Prev_Stage = -1;
  static byte Prev_Index_letter_screen = 255;

  if (Sw1_Sw2_detached) {
    attachInterrupt(Btn1_GPIO, &Ext_INT1_ISR, FALLING);//RISING, FALLING,CHANGE, LOW .
    attachInterrupt(Btn2_GPIO, &Ext_INT2_ISR, FALLING);//RISING, FALLING,CHANGE, LOW .
    Sw1_Sw2_detached = false;
  }

  //
  if (!Screen_0_Drawn) {
    display.setTextColor(WHITE);
    display.clearDisplay();
    Prev_temp = -1;

    display.setTextSize(1);
    display.setCursor(0, 5);
    display.println("Choose Program:");
    display.display();

    display.setCursor(65, 25);
    display.println("Steps");
    display.display();

    display.setCursor(108, 57);
    display.setTextSize(1);
    char Temp[10];
    snprintf_P(Temp, sizeof(Temp), PSTR("v%i"), Version);
    display.println(Temp);
    display.display();

    Prev_Stage = -1;
    Prev_Index_letter_screen = 255;
    Screen_0_Drawn = true;
    Switch3_foi_pressionado_3s = false;
  }
  //

  //Atualizar Temperatura:
  Update_Temp();
  //

  //Re-Desenhar o texto movel:
  if (Prev_Index_letter_screen != Index_letter_screen) {
    //Apagar:
    if (Prev_Index_letter_screen != 255) {//So para filtrar o escrever a preto a primeira letra de todas que nao existe no 255.
      //Apagar letra grande:
      display.setTextColor(BLACK);
      display.setTextSize(2);
      display.setCursor(15, 25);
      display.println(profileLetters[Prev_Index_letter_screen]);
      display.display();
      //Apagar steps :
      display.setTextSize(1);
      display.setCursor(50, 25);
      display.println(profileLength[Prev_Index_letter_screen]);
      display.display();
      //Apagar tempo :
      display.setCursor(50, 35);
      char Temp[10];
      snprintf_P(Temp, sizeof(Temp), PSTR("%02i:%02i:%02i"),
                 profileTimes_h[Prev_Index_letter_screen],
                 profileTimes_min[Prev_Index_letter_screen],
                 profileTimes_sec[Prev_Index_letter_screen]);
      display.println(Temp);
      display.display();
      //Escrever temperatura ::
      display.setCursor(50, 45);
      char Temp_13[7];
      snprintf_P(Temp_13, sizeof(Temp_13), PSTR("%.1f C"),
                 profileSetpoint[Prev_Index_letter_screen]);
      display.println(Temp_13);
      display.display();
      //Apagar indice de passos :
      display.setCursor(100, 45);
      char Temp_2[10];
      snprintf_P(Temp_2, sizeof(Temp_2), PSTR("%i/%i"),
                 Prev_Index_letter_screen + 1,
                 Nr_Letters + 1);
      display.println(Temp_2);
      //Final de Apagar.

    }

    //Escrever novo:
    //Escrever letra grande::
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor(15, 25);
    display.println(profileLetters[Index_letter_screen]);
    display.display();
    //Escrever steps ::
    display.setTextSize(1);
    display.setCursor(50, 25);
    display.println(profileLength[Index_letter_screen]);
    display.display();
    //Escrever tempo ::
    display.setCursor(50, 35);
    char Temp_3[10];
    snprintf_P(Temp_3, sizeof(Temp_3), PSTR("%02i:%02i:%02i"),
               profileTimes_h[Index_letter_screen],
               profileTimes_min[Index_letter_screen],
               profileTimes_sec[Index_letter_screen]);
    display.println(Temp_3);
    display.display();
    //Escrever temperatura ::
    display.setCursor(50, 45);
    char Temp_12[7];
    snprintf_P(Temp_12, sizeof(Temp_12), PSTR("%.1f C"),
               profileSetpoint[Index_letter_screen]);
    display.println(Temp_12);
    display.display();
    //Escrever indice de passos :
    display.setCursor(100, 45);
    char Temp_4[10];
    snprintf_P(Temp_4, sizeof(Temp_4), PSTR("%i/%i"),
               Index_letter_screen + 1,
               Nr_Letters + 1);
    display.println(Temp_4);
    display.display();
    //Final de Escrever novo.

    Prev_Index_letter_screen = Index_letter_screen;

    if (Switch_1)Switch_1 = false;
    if (Switch_2)Switch_2 = false;
    if (Switch_3)Switch_3 = false;

  }
  //Final de Re-Desenhar o texto movel:

  //

}

void Print_Screen_1() {
  static byte time_ = 3;
  static unsigned long currentMillis, previousMillis = 0;

  if (!Screen_1_Drawn) {
    detachInterrupt(Btn1_GPIO);
    detachInterrupt(Btn2_GPIO);
    Sw1_Sw2_detached = true;

    //Apagar Texto do Screen 0:
    display.setTextColor(WHITE);
    display.clearDisplay();
    Prev_temp = -1;

    //Escrever Texto do Screen 1:
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor(15, 25);
    display.println(profileLetters[Index_letter_screen]);
    display.display();

    display.setTextSize(1);
    display.setCursor(50, 05);
    display.println("Starting in:");
    display.display();

    time_ = 3;
    currentMillis = 0;
    previousMillis = 0;
    Screen_1_Drawn = true;

  }

  //Atualizar Temperatura:
  //Update_Temp();
  //

  currentMillis = millis();
  if ((currentMillis - previousMillis >= 1000) && time_ != 255) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    //Apagar numero:
    if (time_ != 3) {
      display.setTextColor(BLACK);
      display.setTextSize(2);
      display.setCursor(80, 25);
      display.println(time_ + 1);
      display.display();
    }


    //Escrever numero:
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor(80, 25);
    display.println(time_);
    display.display();

    time_--;

  }

  if (time_ == 255)Stage = 2;
}

void Print_Screen_2() {
  static int Prev_segundos = -1, Prev_Passo = -1, Prev_s = -1, Prev_m = 0, Prev_h = 0;
  char Temp_s[3];
  char Temp_m[3];
  char Temp_h[3];

  if (!Screen_2_Drawn) {
    Switch3_foi_pressionado_3s = false;
    Prev_segundos = -1;
    Prev_Passo = -1;
    Serial.println("Screen_2_Drawn");

    display.setTextColor(WHITE);
    display.clearDisplay();
    Prev_temp = -1;


    //Escrever letra grande::
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor(20, 25);
    display.println(profileLetters[Index_letter_screen]);
    display.display();
    //Escrever indice de passos :
    display.setCursor(60, 5);
    display.setTextSize(1);
    char Temp_4[10];
    snprintf_P(Temp_4, sizeof(Temp_4), PSTR("1/%i"),
               profileLength[Index_letter_screen]);
    display.println(Temp_4);
    display.display();

    //Atualizar segundos:
    display.setCursor(60, 20);
    char Temp_10[10];
    snprintf_P(Temp_10, sizeof(Temp_10), PSTR("%02i:%02i:%02i"),
               0,
               0,
               0);
    display.write(Temp_10);
    display.display();

    //Final de Escrever novo.
    display.setTextSize(1);
    display.setCursor(60, 50);
    char Temp_5[10];
    snprintf_P(Temp_5, sizeof(Temp_5), PSTR("%02i:%02i:%02i"),
               profileTimes_h[Index_letter_screen],
               profileTimes_min[Index_letter_screen],
               profileTimes_sec[Index_letter_screen]);
    display.write(Temp_5);
    display.display();

    display.setTextSize(2);
    display.setCursor(80, 30);
    display.cp437(true);         // Use full 256 char 'Code Page 437' font
    display.write(25);
    display.display();



    Screen_2_Drawn = true;

  }

  //Atualizar Temperatura:
  Update_Temp();
  //

  if (Second_F && Prev_segundos != segundos) {
    //
    int t = segundos;
    int s = t % 60;
    t = (t - s) / 60;
    int m = t % 60;
    int h = (t - m) / 60;
    //

    //Atualizar step:
    if (Prev_Passo != Passo) {
      //Apagar step antigo:
      display.setTextColor(BLACK);
      display.setCursor(60, 5);
      display.setTextSize(1);
      char Temp_8[10];
      snprintf_P(Temp_8, sizeof(Temp_8), PSTR("%i"),
                 Prev_Passo + 1);
      display.println(Temp_8);
      display.display();
      //Escrever step novo:
      display.setTextColor(WHITE);
      display.setCursor(60, 5);
      display.setTextSize(1);
      char Temp_9[10];
      snprintf_P(Temp_9, sizeof(Temp_9), PSTR("%i"),
                 Passo + 1);
      display.println(Temp_9);
      display.display();
      Prev_Passo = Passo;
    }
    //Atualizar segundos:
    display.setTextColor(BLACK);
    display.setTextSize(1);
    //display.setCursor(60, 20);    //Apagar hora anterior: //APAGAR//

    if (Prev_segundos == -1) {
      display.setCursor(96, 20);    //Apagar hora anterior: //APAGAR//
      display.write("00");
      display.display();
    } else {
      //Ajustar segundos:
      //Apagar segundos:
      display.setCursor(96, 20);

      /*
            t = (t - s) / 60;
            int Prev_m = t % 60;
            int h = (t - m) / 60;;
      */


      snprintf_P(Temp_s, sizeof(Temp_s), PSTR("%02i"),
                 Prev_s);

      display.write(Temp_s);
      display.display();
    }


    if (Prev_m != m) {
      //Ajustar minutos:
      //Apagar minutos:
      display.setCursor(78, 20);

      snprintf_P(Temp_m, sizeof(Temp_m), PSTR("%02i"),
                 Prev_m );

      display.write(Temp_m);
      display.display();

      Prev_m = m;

      //Escrever minutos:
      display.setTextColor(WHITE);
      display.setTextSize(1);
      display.setCursor(78, 20);
      snprintf_P(Temp_m, sizeof(Temp_m), PSTR("%02i"),
                 m);
      display.write(Temp_m);
      display.display();
    }


    if (Prev_h != h) {
      //Ajustar horas:
      //Apagar horas:
      display.setTextColor(BLACK);

      display.setCursor(60, 20);

      snprintf_P(Temp_h, sizeof(Temp_h), PSTR("%02i"),
                 Prev_h );

      display.write(Temp_h);
      display.display();

      Prev_h = h;

      //Escrever horas:
      display.setTextColor(WHITE);
      display.setTextSize(1);
      display.setCursor(60, 20);
      snprintf_P(Temp_h, sizeof(Temp_h), PSTR("%02i"),
                 h);
      display.write(Temp_h);
      display.display();

    }


    //Escrever segundos:
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(96, 20);
    snprintf_P(Temp_s, sizeof(Temp_s), PSTR("%02i"),
               s);
    display.write(Temp_s);
    display.display();

    Prev_segundos = segundos;
    Prev_s = s;
  }
}

void Print_Screen_3() {
  static bool _attached = false;

  if (!Screen_3_Drawn) {
    Desligar_LEDS();
    detachInterrupt(Btn3_GPIO);//RISING, FALLING,CHANGE, LOW .
    _attached = false;
    segundos = 0;
    Passo = 0;//changed from 0 to 1
    if (Switch_3 ) {
      Serial.println("Switch_3 estava true no inicio de stage 3");
      Switch_3 = false;
    }

    if (Switch3_foi_pressionado_3s ) {
      Serial.println("Switch3_foi_pressionado_3s estava true no inicio de stage 3");
      Switch3_foi_pressionado_3s = false;
    }



    //Switch3_foi_pressionado_3s = false;
    Start_time = 0;
    Running_running = false;

    //Apagar varias coisas:
    //Apagar tempo :
    display.setTextSize(1);
    display.setTextColor(BLACK);
    display.setCursor(60, 50);
    char Temp_9[10];
    snprintf_P(Temp_9, sizeof(Temp_9), PSTR("%02i:%02i:%02i"),
               profileTimes_h[Index_letter_screen],
               profileTimes_min[Index_letter_screen],
               profileTimes_sec[Index_letter_screen]);
    display.write(Temp_9);
    display.display();

    //Escreve Finished:
    display.setTextColor(WHITE);
    display.setCursor(60, 50);
    display.println("Canceled!");
    display.display();

    Screen_3_Drawn = true;

    delay(2000);
    ESP.restart();
  }

  //Atualizar Temperatura:
  Update_Temp();
  //

  delay(2000);
  if (digitalRead(Btn3_GPIO) && !_attached) {
    _attached = true;
    Serial.println("attachInterrupt");
    attachInterrupt(Btn3_GPIO, &Ext_INT3_ISR, FALLING);
  }

}

void Print_Screen_4() {

  if (!Screen_4_Drawn) {
    segundos = 0;
     Passo = 0;//changed from 0 to 1
    Switch3_foi_pressionado_3s = false;
    Start_time = 0;
    Running_running = false;

    //Apagar varias coisas:
    //Apagar tempo :
    display.setTextSize(1);
    display.setTextColor(BLACK);
    display.setCursor(60, 50);
    char Temp_9[10];
    snprintf_P(Temp_9, sizeof(Temp_9), PSTR("%02i:%02i:%02i"),
               profileTimes_h[Index_letter_screen],
               profileTimes_min[Index_letter_screen],
               profileTimes_sec[Index_letter_screen]);
    display.write(Temp_9);
    display.display();

    //Escreve Finished:
    display.setTextColor(WHITE);
    display.setCursor(60, 50);
    display.println("Finished!");
    display.display();

    Screen_4_Drawn = true;
  }

  //Atualizar Temperatura:
  Update_Temp();
  //

}
