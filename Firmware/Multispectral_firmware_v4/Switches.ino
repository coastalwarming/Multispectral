void Check_SW3_Long_press() {

  if (Switch_3 && digitalRead(Btn3_GPIO) && !Switch3_foi_pressionado_3s) {
    Switch_3 = false;
    Serial.println("Switch 3");

    if (Stage == 3 || Stage == 4) {
      Stage = 0;
      Screen_0_Drawn = false;
      Screen_1_Drawn = false;
      Screen_2_Drawn = false;
      Screen_3_Drawn = false;
      Screen_4_Drawn = false;

      timerAlarmDisable(timer);//-> Desliga timer.
      timerWrite(timer, 0);//-> Faz reset ao timer.
      segundos = 0;
        Passo = 0;//changed from 0 to 1
      Start_time = 0;
      Running_running = false;

      Serial.println("Stage = 0");
    }

  } else if ((Switch_3) && (millis() - Millis_Botao_passed_3 > 3000) && (!Switch3_foi_pressionado_3s)) {
    Serial.println("Switch 3 pressionado 3 segundos.");
    Serial.print("Switch_3:");Serial.println(Switch_3);
    Serial.print("Millis_Botao_passed_3:");Serial.println(Millis_Botao_passed_3);
    Serial.print("Switch3_foi_pressionado_3s:");Serial.println(Switch3_foi_pressionado_3s);


    if (Stage == 0) {
      Stage = 1;
      Serial.println("Stage = 1");
    } else if (Stage == 2) {
      Stage = 3;
      Serial.println("Stage = 3");
    }





    Switch3_foi_pressionado_3s = true;
    Switch_3 = false;

  }
}

void Ext_INT1_ISR()
{

  if (!Switch_1 && ((millis() - Millis_Botao_passed) > Debounce_ms)) {

    Millis_Botao_passed = millis();
    Switch_1 = true;
    Index_letter_screen++;
    // Serial.println(Index_letter_screen);
    if (Index_letter_screen > Nr_Letters)Index_letter_screen = 0;
  }
}

void Ext_INT2_ISR()
{

  if (!Switch_2 && ((millis() - Millis_Botao_passed) > Debounce_ms)) {

    Millis_Botao_passed = millis();
    Switch_2 = true;
    Index_letter_screen--;
    // Serial.println(Index_letter_screen);
    if (Index_letter_screen == 255)Index_letter_screen = Nr_Letters;

  }

}


void Ext_INT3_ISR()
{

  if (!Switch_3 && ((millis() - Millis_Botao_passed_3) > Debounce_ms) && (!Switch3_foi_pressionado_3s)) {

    Millis_Botao_passed_3 = millis();
    Switch_3 = true;
    //Serial.println("SW3");
  }

}
