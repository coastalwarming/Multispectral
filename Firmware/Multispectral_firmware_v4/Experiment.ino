void Run_experiment() {
  Print_Screen_2();

  static bool Passo_implementado = false;

  if (Passo == 0 && Start_time == 0 && !Running_running) {
    Running_running = true;
    timerWrite(timer, 0);//-> Faz reset ao timer.
    timerAlarmEnable(timer);
    //Start_time = millis();//Guarda instante de inicio da experiment.
    Start_time = 0;//Guarda instante de inicio da experiment.


  }
  //Implementar passo:
  if (!Passo_implementado) {
    Passo_implementado = true;
    Apply_intensity();
  }

  //portENTER_CRITICAL(&timerMux);
  if (Second_F) {
    Print_Screen_2();//Atualizar os segundos no oled.

    //Serial.print("segundos2:");   Serial.println(segundos);
    //Serial.print("profileTimes[Index_letter_screen][Passo]:");   Serial.println(profileTimes[Index_letter_screen][Passo]);

    portENTER_CRITICAL(&timerMux);
    Second_F = false;
    portEXIT_CRITICAL(&timerMux);

    //Esta na hora de mudar de passo?
    if ( segundos >= profileTimes[Index_letter_screen][Passo]) {
      if ((Passo + 1) < profileLength[Index_letter_screen]) { //Ainda nao foi o ultimo passo?

        Passo++;
        Passo_implementado = false;
        Serial.printf("Passei ao passo %i .\n", Passo);


      } else {//Já foi o ultimo passo - Terminar!
        Running_experiment = false;
        Desligar_LEDS();
        Passo_implementado = false;
        Serial.println("Terminei");

        Stage = 4;
        timerAlarmDisable(timer);//-> Desliga timer.
        timerWrite(timer, 0);//-> Faz reset ao timer.
        segundos = 0;

        /*Desligar tudo!!!*/

      }
    }
  }
  //portEXIT_CRITICAL(&timerMux);

}


void Apply_intensity() {

  for (int i = 0; i < 24; i++) {
    tlc.setPWM(i, int(LEDs_Light[Index_letter_screen][i][Passo] * 40.95));
    //Serial.print(i); Serial.print("-");
    //Serial.println(int(LEDs_Light[Index_letter_screen][i][Passo] * 40.95));
  }
  tlc.write();

  Serial.printf("Reached step:%i and ", Passo + 1);
  Serial.print("Set LED nº1 to:");
  Serial.println(int(LEDs_Light[Index_letter_screen][0][Passo]));

  /*
    tlc.setPWM(0, int(LEDs_Light[1][Passo] * 40.95));
    Serial.print("1 Set LED1 to:");
    Serial.println(int(LEDs_Light[1][Passo] * 40.95));

    tlc.write();
  */

}

void Desligar_LEDS() {
  for (int i = 0; i < 24; i++) {
    tlc.setPWM(i, 0);
  }
  tlc.write();
}
