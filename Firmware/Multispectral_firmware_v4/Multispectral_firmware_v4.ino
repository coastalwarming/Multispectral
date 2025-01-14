const int  Version = 4;
const int Low_temp_limit = 0, High_temp_limit = 50;


#include <Update.h>



#define FAN_GPIO 6
const int freq = 3000;
//A nova ventoinha n funciona a <3000Khz.
//COm 1kHz faz ruido em 50% mas nao faz a >60% intensidade. Solução foi 3500Hz e 4.7KOhm de pullup no gate.
const int ledChannel = 0;
const int resolution_PWM = 8;

//DS18B20
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 5
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress tempDeviceAddress;
unsigned long lastTempRequest = 0;
float temperature = 0.0, Prev_temp = -1;
int resolution_DS18B20 = 11;
//

//TLC5947
#include "Adafruit_TLC5947.h"
#define NUM_TLC5947 1
#define Data 11 //-> MOSI (GPIO11 no ESP32S3) || MOSI (GPIO23 no ESP32E) -> TLC5947 Pino 4 (SIN)
#define Clock 12 //-> SCK (GPIO12 no ESP32S3) || SCK (GPIO18 no ESP32E)  -> TLC5947 Pino 3 (SCLK)
#define latch 4 //-> Qualquer pino (Ex: GPIO4 no ESP32S3)                -> TLC5947 Pino 30 (XLAT)
#define oe -1  // set to -1 to not use the enable pin (its optional)
bool SD_Failed = false, SD_detected = false, SD_was_undecteted = false, SD_returned = false;
Adafruit_TLC5947 tlc = Adafruit_TLC5947(NUM_TLC5947, Clock, Data, latch);
//

//SW
#define Btn1_GPIO   17
#define Btn2_GPIO   15
#define Btn3_GPIO   16
const int Debounce_ms = 500;

volatile bool Switch_1 = false;
volatile bool Switch_2 = false;
volatile bool Switch_3 = false;

volatile unsigned long Millis_Botao_passed = 0, Millis_Botao_passed_3 = 0;

bool Switch1_foi_pressionado_3s = false;
//

//OLED
#include <SPI.h>
#include <Wire.h>

#include <Adafruit_GFX.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define SSD1306_NO_SPLASH
#include <Adafruit_SSD1306.h>

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3D ///< See datasheet for Address via i"C scanner code.
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

byte Stage = 0;
int   Passo = 0;//changed from 0 to 1
volatile bool Switch3_foi_pressionado_3s = false;

//

bool Running_running = false;

unsigned int profileTimes[26][100];
unsigned int LEDs_Light[26][24][100];

float profileSetpoint[26];

char profileLetters[26];
byte profileLength[100];

int profileTimes_sec[100];
int profileTimes_min[100];
int profileTimes_h[100];

bool Running_experiment = false;
unsigned long Start_time = 0;


#include "FS.h"
#include "SD_MMC.h"

/**/
#define Detect_SD_PIN 1 //Tem de ter 10k pullup.
#define DEBOUNCETIME 20
File file;
volatile int numberOfButtonInterrupts_SD = 0;
volatile bool lastState_SD;
volatile uint32_t debounceTimeout_SD = 0;
int SD_detect_state = -1;

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
void IRAM_ATTR handleButtonInterrupt_SD() {
  portENTER_CRITICAL_ISR(&mux);
  numberOfButtonInterrupts_SD++;
  lastState_SD = !digitalRead(Detect_SD_PIN);
  debounceTimeout_SD = xTaskGetTickCount(); //version of millis() that works from interrupt
  portEXIT_CRITICAL_ISR(&mux);
}
TaskHandle_t Task_gerir_SD;
/**/


//Timer de contar segundos:
volatile int segundos;
volatile bool Second_F = false;

hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

bool Screen_0_Drawn = false,
     Screen_1_Drawn = false,
     Screen_2_Drawn = false,
     Screen_3_Drawn = false,
     Screen_4_Drawn = false;

void IRAM_ATTR onTimer() {

  segundos++;

  portENTER_CRITICAL_ISR(&timerMux);
  Second_F = true;//Esta variavel é colocada a zero no decorrer da experiencia para saber que ja ajustou o segundo que passou.
  portEXIT_CRITICAL_ISR(&timerMux);
}
//

volatile byte Index_letter_screen = 0;
volatile byte Nr_Letters = 0;
bool Sw1_Sw2_detached = false;
void setup() {

  Serial.begin(115200);

  Serial.printf("Firmware version:v%i .\n", Version);
  //PWM
  pinMode(FAN_GPIO, OUTPUT);
  //ledcSetup(ledChannel, freq, resolution_PWM);
  //ledcAttachPin(FAN_GPIO, ledChannel);

  //

  //DS18B20
  sensors.begin();
  sensors.getAddress(tempDeviceAddress, 0);
  sensors.setResolution(tempDeviceAddress, resolution_DS18B20);
  sensors.setWaitForConversion(false);
  sensors.requestTemperatures();
  //delayInMillis = 750 / (1 << (12 - resolution));
  lastTempRequest = millis();
  //

  //TLC5947
  tlc.begin();
  if (oe >= 0) {
    pinMode(oe, OUTPUT);
    digitalWrite(oe, LOW);
  }

  //tlc.begin();
  Desligar_LEDS();
  //

  //SW
  pinMode(Btn1_GPIO, INPUT);
  pinMode(Btn2_GPIO, INPUT);
  pinMode(Btn3_GPIO, INPUT);

  attachInterrupt(Btn1_GPIO, &Ext_INT1_ISR, FALLING);//RISING, FALLING,CHANGE, LOW .
  attachInterrupt(Btn2_GPIO, &Ext_INT2_ISR, FALLING);//RISING, FALLING,CHANGE, LOW .
  attachInterrupt(Btn3_GPIO, &Ext_INT3_ISR, FALLING);//RISING, FALLING,CHANGE, LOW .
  //

  //OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) { //Obtive pelo scan de addrress de i2c.
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  //

  //Timer segundos:
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000000, true);//1000000
  //timerAlarmEnable(timer);
  //timerAlarmDisable(timer);
  //timerWrite(timer, 0);//-> Faz reset ao timer.

  //

  /**/
  pinMode(Detect_SD_PIN, INPUT_PULLUP);  // Pull up to 3.3V on input - some buttons already have this done
  attachInterrupt(digitalPinToInterrupt(Detect_SD_PIN), handleButtonInterrupt_SD, CHANGE);//FALLING
  handleButtonInterrupt_SD();//Corre logo a interrupcao para detetar se botao ja esta pressionado.
  //Task
  xTaskCreatePinnedToCore(
    taskButtonRead_SD,   // função que implementa a tarefa
    "taskButtonRead_SD", // nome da tarefa
    2000,      // número de palavras a serem alocadas para uso com a pilha da tarefa
    NULL,       // parâmetro de entrada para a tarefa (pode ser NULL)
    1,          // prioridade da tarefa (0 a N)
    &Task_gerir_SD,       // referência para a tarefa (pode ser NULL)
    0);         // Núcleo que executará a tarefa

  delay(2000);
  /**/

  if (!SD_MMC.setPins(20, 21, 35, 36, 37, 38)) {//bool setPins(int clk, int cmd, int d0, int d1, int d2, int d3);// NO DEVKitM S3 o 33 e 34 nao tem breakout
    Serial.println("Failed pin setup");
    SD_Failed = true;
  }


  if (SD_detected) {
    if (!SD_MMC.begin()) {
      Serial.println("Card Mount Failed");
      SD_Failed = true;
      return;
    }
  } else {
    //Card absent.
    Serial.println("Card is not being detected.");

    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(5, 25);
    display.println("No SD card.");
    display.display();
    while (!SD_detected) {
      delay(1000);
    }

    //SD detected:
    //Apagar
    display.setTextColor(BLACK);
    display.setTextSize(1);
    display.setCursor(5, 25);
    display.println("No SD card.");
    display.display();
    //Escrever
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(5, 25);
    display.println("SD card detected.");
    display.display();

    display.setCursor(5, 35);
    display.println("Rebooting...");
    display.display();
    delay(2000);
    display.fillScreen(BLACK);

    ESP.restart();

  }

  if (SD_Failed && SD_detected) {
    //Card corrupted.
    Serial.println("Card is being detected but does not mount - possibly corrupted.");

    display.setTextSize(1);
    display.setCursor(5, 25);
    display.println("SD card is corrupted.");
    display.display();
    while (SD_detected) {
      delay(1000);
    }
    while (!SD_detected) {
      delay(1000);
    }

    //SD detected:
    //Apagar
    display.setTextColor(BLACK);
    display.setTextSize(1);
    display.setCursor(5, 25);
    display.println("SD card is corrupted.");
    display.display();
    //Escrever
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(5, 25);
    display.println("SD card detected.");
    display.display();

    display.setCursor(5, 35);
    display.println("Rebooting...");
    display.display();
    delay(2000);
    display.fillScreen(BLACK);
    ESP.restart();



  }

  updateFromFS(SD_MMC);


  Count_Profiles(SD_MMC, "/", 0);
  //GetProfile_SD(SD_MMC, "/B.txt");



}

void loop() {
  if (Stage == 0)Print_Screen_0();//Choosing
  if (Stage == 1)Print_Screen_1();//Countdown to start
  if (Stage == 2)Run_experiment();//Experiment
  if (Stage == 3)Print_Screen_3();//Cancel
  if (Stage == 4)Print_Screen_4();//Finish

  if (Running_experiment) Run_experiment();
  Check_SW3_Long_press();

  if (SD_returned) {

    display.fillScreen(BLACK);

    //Escrever
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(5, 25);
    display.println("SD card detected.");
    display.display();

    display.setCursor(5, 35);
    display.println("Rebooting...");
    display.display();
    delay(2000);
    display.fillScreen(BLACK);
    ESP.restart();
  }


  /*
    portENTER_CRITICAL(&timerMux);
    if (Second_F) {
      Serial.println(segundos);
      Second_F = false;
    }
    portEXIT_CRITICAL(&timerMux);
  */
}
