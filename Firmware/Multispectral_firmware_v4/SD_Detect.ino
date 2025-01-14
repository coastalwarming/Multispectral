
void taskButtonRead_SD( void * parameter) {
  /*
    String taskMessage = "Debounced ButtonRead Task running on core ";
    taskMessage = taskMessage + xPortGetCoreID();
    Serial.println(taskMessage);
  */
  /*
    // set up button Pin
    pinMode(BUTTONPIN, INPUT);
    pinMode(BUTTONPIN, INPUT_PULLUP);  // Pull up to 3.3V on input - some buttons already have this done

    attachInterrupt(digitalPinToInterrupt(BUTTONPIN), handleButtonInterrupt, CHANGE);//FALLING
  */
  uint32_t saveDebounceTimeout;
  bool saveLastState;
  int save;

  // Enter RTOS Task Loop
  while (1) {

    portENTER_CRITICAL_ISR(&mux); // so that value of numberOfButtonInterrupts,l astState are atomic - Critical Section
    save  = numberOfButtonInterrupts_SD;
    saveDebounceTimeout = debounceTimeout_SD;
    saveLastState  = lastState_SD;
    portEXIT_CRITICAL_ISR(&mux); // end of Critical Section

    SD_detect_state = !digitalRead(Detect_SD_PIN);

    // This is the critical IF statement
    // if Interrupt Has triggered AND Button Pin is in same state AND the debounce time has expired THEN you have the button push!
    //
    if ((save != 0) //interrupt has triggered
        && (SD_detect_state == saveLastState) // pin is still in the same state as when intr triggered
        && (millis() - saveDebounceTimeout > DEBOUNCETIME ))
    { // and it has been low for at least DEBOUNCETIME, then valid keypress

      if (SD_detect_state == LOW)
      {
        Serial.println("Sd missing");
        SD_detected = false;
        SD_was_undecteted = true;
        file.close();
        SD_MMC.end();
      }
      else
      {
        Serial.println("Sd detected");
        SD_detected = true;
        if (SD_was_undecteted) {
          SD_was_undecteted = false;
          SD_returned = true;
        }
        //  if (RunStatus == No_SD)SD_redetected();
      }



      portENTER_CRITICAL_ISR(&mux); // can't change it unless, atomic - Critical section
      numberOfButtonInterrupts_SD = 0; // acknowledge keypress and reset interrupt counter
      portEXIT_CRITICAL_ISR(&mux);


      vTaskDelay(10 / portTICK_PERIOD_MS);
    }

    vTaskDelay(10 / portTICK_PERIOD_MS);

  }
}
