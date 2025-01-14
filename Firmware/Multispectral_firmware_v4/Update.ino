void updateFromFS(fs::FS &fs) {
  File updateBin = fs.open("/update.bin");
  //  listDir(SD_MMC, "/", 0);
  if (updateBin) {
    if (updateBin.isDirectory()) {
      Serial.println("Error, update.bin is not a file");
      updateBin.close();
      return;
    }

    size_t updateSize = updateBin.size();

    if (updateSize > 0) {
      Serial.println("Try to start update");

      display.setTextColor(WHITE);
      display.setTextSize(1);
      display.setCursor(15, 25);
      display.println("Updating...");
      display.display();

      performUpdate(updateBin, updateSize);
    }
    else {
      Serial.println("Error, file is empty");
    }

    updateBin.close();

    // whe finished remove the binary from sd card to indicate end of the process
    fs.remove("/update.bin");



    ESP.restart();
  }
  else {
    Serial.println("Could not load update.bin from sd root");


  }
}


void performUpdate(Stream &updateSource, size_t updateSize) {
  if (Update.begin(updateSize)) {
    size_t written = Update.writeStream(updateSource);
    if (written == updateSize) {
      Serial.println("Written : " + String(written) + " successfully");
    }
    else {
      Serial.println("Written only : " + String(written) + "/" + String(updateSize) + ". Retry?");
    }
    if (Update.end()) {
      Serial.println("OTA done!");
      if (Update.isFinished()) {
        Serial.println("Update successfully completed. Rebooting.");

        display.setTextColor(WHITE);
        display.setTextSize(1);
        display.setCursor(15, 35);
        display.println("successful!");
        display.display();

        display.setCursor(15, 45);
        display.println("Rebooting...");
        delay(2000);



      }
      else {
        Serial.println("Update not finished? Something went wrong!");

      }
    }
    else {
      Serial.println("Error Occurred. Error #: " + String(Update.getError()));
    }

  }
  else
  {
    Serial.println("Not enough space to begin OTA");
  }
}
