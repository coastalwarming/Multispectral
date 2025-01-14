int getposition(const char *array, size_t size, char c) {
  for (size_t i = 0; i < size; i++)
  {
    if (array[i] == c)
      return (int)i;
  }
  return -1;
}

void Count_Profiles(fs::FS & fs, const char * dirname, uint8_t levels) {
  static byte Index_letter = 0;

  // Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      //  Serial.print("  DIR : ");
      //  Serial.println(file.name());
      if (levels) {
        //listDir(fs, file.name(), levels - 1);
      }
    } else {
      if (strlen(file.name()) == 5) {
        char Name[4];
        strncpy(Name, file.name() + 2, 3 );
        Name[3] = '\0';
        //   Serial.print("Name:"); Serial.println(Name);

        if (strstr(Name, "txt") > 0) {

          //Serial.print("Variaveis com 5 chars e txt:");
          //Serial.println(file.name());
          profileLetters[Index_letter] = file.name()[0];

          //Serial.print("file.name()[0]:");
          //Serial.println(file.name()[0]);

          //Serial.print("profileLetters[Index_letter]:");
          //Serial.println(profileLetters[Index_letter]);

          //
          //GetProfile_SD_2();
          int Passo_ = 0;
          char buf[80];

          Serial.printf("Reading file: %s\n", file.name());

          //File Profile = fs.open(path);//teste a ver se nao abre A.txt dentro do count por ja ter aberto.
          if (!file) {
            Serial.println("Failed to open file! for reading.");
            //  return false;
          }



          while (file.available()) {


            file.readBytesUntil('\n', buf, 80);
            buf[79] = '\0';
            // Serial.print("buf:"); Serial.println(buf);

            if (Passo_ == 0) {
              buf[4] = '\0';

              profileSetpoint[Index_letter] = atof(buf);
              if ( profileSetpoint[Index_letter] < 0) profileSetpoint[Index_letter] = Low_temp_limit;
              if ( profileSetpoint[Index_letter] > 50) profileSetpoint[Index_letter] = High_temp_limit;
              //Serial.print("profileSetpoint[Index_letter]:"); Serial.println(profileSetpoint[Index_letter]);
            } else {

              //Recolher o tempo de esta linha:
              int A = getposition(buf, sizeof(buf), '-');
              //Serial.print(profileLetters[Index_letter]);  Serial.print(" :" ); Serial.println(A);

              char temp[6] = {0, 0, 0, 0, 0, 0};
              strncpy(temp, buf,  A );
              //Serial.print("temp:"); Serial.println(temp);

              temp[A] = '\0';
              //Soma os tempos com o anterior.
              if (Passo_ > 1) {
                profileTimes[Index_letter][Passo_ - 1] = atoi(temp) + profileTimes[Index_letter][Passo_ - 2];

              } else {
                profileTimes[Index_letter][Passo_ - 1] = atoi(temp);


              }


              //

              //Recolha dos 24 leds para esta linha:
              int LED_num = 0;
              for (int x = A + 1; x < 76; x = x + 3) {

                char Led_temp[4] = {buf[x], buf[x + 1], buf[x + 2], '\0'};

                LEDs_Light[Index_letter][LED_num][Passo_ - 1] = atoi(Led_temp);

                //Serial.printf("LEDs_Light[%i][%i][%i]:%i\n",Index_letter, LED_num, Passo_, LEDs_Light[Index_letter][LED_num][Passo_]);


                LED_num++;
              }

              //
              //Passo_++;
            }
            Passo_++;
          }//Final else de ja n ser linha 1.
          if (file.available() == 0) {
            profileLength[Index_letter] = Passo_ - 1; //Guarda o numero de passos do perfil.
            int t = profileTimes[Index_letter][Passo_ - 2];
  

            int s = t % 60;
            //Serial.print("s:"); Serial.println(s);

            t = (t - s) / 60;
            int m = t % 60;
            //Serial.print("m:"); Serial.println(m);

            int h = (t - m) / 60;;
            //Serial.print("h:"); Serial.println(h);

            profileTimes_sec[Index_letter] = s;
            profileTimes_min[Index_letter] = m;
            profileTimes_h[Index_letter] = h;


            //   return true;
          }


          //

          Index_letter++;
          //   Serial.println("Final txt");

        } else {
          // Serial.println("File without txt");
        }




      }
    }
    file = root.openNextFile();
  }
  Serial.printf("Found %i profiles. \n", Index_letter);
  Nr_Letters = Index_letter - 1;
  file.close();


}
