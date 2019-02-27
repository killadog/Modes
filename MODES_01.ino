#include <SimpleTimer.h>
#include <Bounce2.h>

#define BUTTON_PIN                  5                               //пин BUTTON
#define SDA_PIN                     A4                              //пин SDA
#define SCL_PIN                     A5                              //пин SCL

byte    MODES                     = 2;                              //количество режимов
byte    CURRENT_MODE              = 0;                              //текущий режим
boolean BUTTON_FLAG               = 0;                              //флаг состояния кнопки

SimpleTimer timer;
Bounce debouncer = Bounce();

void setup() {
  Serial.begin(115200);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(SDA_PIN,    INPUT_PULLUP);
  pinMode(SCL_PIN,    INPUT_PULLUP);

  debouncer.attach(BUTTON_PIN);                                    //настройка Bouncer на пин кнопки
  debouncer.interval(5);                                           //интервал дребезга

  timer.setInterval(50L, Check_BUTTON);                            //слушаем кнопку каждые N миллисекнуд
  timer.setInterval(1000L, RUN_MODE);                              //запуск текщего режима каждые N миллисекнуд
}

void loop() {
  timer.run();                                                     //таймер
}

void RUN_MODE() {
  switch (CURRENT_MODE) {
    case 0: MODE_0(); break;
    case 1: MODE_1(); break;
  }
}

void MODE_0() {
  Serial.print("MODE: ");
  Serial.println(CURRENT_MODE);
}

void MODE_1() {
  Serial.print("MODE: ");
  Serial.println(CURRENT_MODE);
}

void Check_BUTTON() {
  boolean changed = debouncer.update();
  if (changed) {
    int value = debouncer.read();
    if (value == LOW && BUTTON_FLAG == 0) {                        //замкнуто (жмут кнопку!)
      CURRENT_MODE++;                                              //следущий режим
      BUTTON_FLAG = 1;                                             //флаг, что кнопка НАжата
      if (CURRENT_MODE > MODES - 1) {                              //когда режимы "закончились",
        CURRENT_MODE = 0;                                          //то сброс счётчика режимов на начало
      }
      Serial.println("Button pressed!");
    }
    if (value == HIGH && BUTTON_FLAG == 1) {                       //разомкнуто и флаг говорит, что до этого кнопка "жалась"
      BUTTON_FLAG = 0;                                             //ставим флаг, что кнопка ОТжата
    }
  }
}
