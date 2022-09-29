#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>                                         // подключаем библиотеку RF24 для работы с модулем nRF24L01+
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789                                                                                          // подключаем библиотеку UTFT для работы с TFT дисплеем
#include <UTFT.h>

Adafruit_ST7735 lsd = Adafruit_ST7735(7,5,4,3,6);                                                          // объявляем  объект lsd для работы с библиотекой UTFT, указывая тип дисплея и выводы (TFT01_22SP, SDI/MOSI, SCK, CS, RESET, DC/RS);
RF24            radio(9, 10);                                                                               // создаём объект radio для работы с библиотекой RF24, указывая номера выводов nRF24L01+ (CE, CSN)
extern uint8_t  SmallFont[];                                                                                // подключаем маленький шрифт
       uint8_t  Loading        = 0;                                                                         // первый проход по не сохраняется
       uint8_t  ChannelScan    = 0;                                                                         // номер сканируемого канала
       uint8_t  ChannelPrint   = 1;                                                                         // номер выводимого канала
       uint8_t  ChannelPowerNow[128];                                                                       // текущая мощность сигналов на каждом канале
       uint8_t  ChannelPowerMax[128];                                                                       // максимальная мощность сигналов на каждом канале
       uint16_t PositionPoints[10];                                                                         // положение точек на дисплее
const  uint16_t ColorBG          = ST7735_BLACK;                                                               // цвет фона
const  uint16_t ColorScale       = ST7735_BLUE;                                                                // цвет шкалы и векторов графика
const  uint16_t ColorTextLoad    = ST7735_WHITE;                                                               // цвет текста отображающего проценты при старте
const  uint16_t ColorGraphNow    = ST7735_WHITE;                                                               // цвет графика отображающего текущие значения
const  uint16_t ColorGraphMax    = ST7735_RED;                                                                 // цвет графика отображающего максимальные значения
const  uint16_t PositionLeft     = 0;                                                                      // крайнее левое положение графика на дисплее
const  uint16_t PositionWidth    = 1;                                                                       // ширина одного канала на дисплее
const  uint16_t PositionTop      = 0;                                                                      // крайнее верхнее положение графика на дисплее
const  uint16_t PositionBottom   = 128;                                                                     // крайнее нижнее  положение графика на дисплее
const  uint8_t  PositionHeight1  = 1;                                                                       // размер каждой 1  линии каналов на нижней шкале графика
const  uint8_t  PositionHeight5  = 4;                                                                       // размер каждой 5  линии каналов на нижней шкале графика
const  uint8_t  PositionHeight10 = 17;                                                                       // размер каждой 10 линии каналов на нижней шкале графика
const  uint8_t  FactorGraph      = 1;                                                                       // множитель значений графика





void setup(){
//  подготавливаем дисплей и модуль к работе
     lsd.initR();
  // Установить контраст
     lsd.fillScreen(ColorBG);
    lsd.setRotation(2);
    radio.begin();                                                                                          // инициируем работу модуля nRF24L01+
    radio.setAutoAck(false);                                                                                // отключаем режим подтверждения приёма
    memset(ChannelPowerNow, 0, sizeof(ChannelPowerNow));                                                    // заполняем все элементы массива ChannelPowerNow значением 0
    memset(ChannelPowerMax, 0, sizeof(ChannelPowerMax));                                                    // заполняем все элементы массива ChannelPowerMax значением 0
    PositionPoints[6]=PositionBottom;                                                 // положение текста с номерами каналов
 //   PositionPoints[7]=PositionBottom-lsd.getFontYsize()*2;                                               // положение текста с числами частот
     PositionPoints[8]=PositionBottom-PositionHeight10;                              // положение нулевой координаты по оси Y
//  выводим нижнюю шкалу графика для всего диапазона каналов
//        lsd.setColor(ColorScale);                                                                        // устанавливаем цвет заливки шкалы
    for(uint8_t i=0; i<128; i++){                                                                           // создаём цикл по всему диапазону каналов
        PositionPoints[9]=PositionLeft+i*PositionWidth;                                                     // положение координаты очередного канала по оси X
 

 
}}

void loop(){
//  сканируем очередной канал и сохраняем его данные
    radio.setChannel(ChannelScan);                                                                          // устанавливаем очередной канал работы модуля
    radio.startListening();                                                                                 // начинаем прослушивать очередной канал
    delayMicroseconds(50);                                                                                  // ждём 50 мкс
    radio.stopListening();                                                                                  // останавливаем прослушивание
    if(radio.testRPD()){ChannelPowerNow[ChannelScan]++;}                                                    // если сигнал на указанном канале имел мощность > -64 дБм, то увеличиваем значение в массиве ChannelPowerNow для текущего канала (функция не работает на модулях nRF24L01 без знака «+»)
    if (ChannelPowerMax[ChannelScan]<ChannelPowerNow[ChannelScan]){                                         // если мощность отсканированного канала превышает ранее полученное максимальное значение для этого канала, то ...
        ChannelPowerMax[ChannelScan]=ChannelPowerNow[ChannelScan];                                          // сохраняем мощность отсканированного канала как максимальную
    }   ChannelScan++;                                                                                      // увеличиваем номер сканируемого канала
//  выводим полученные данные                                                                               // данные выводятся по одному каналу, через полный цикл сканирования всех каналов
    if(ChannelScan==127){                                                                                   // если был просканирован последний возможный канал, то ...
        ChannelScan=0;                                                                                      // переходим к 0 каналу
        if(Loading<128){                                                                                    // если это первый проход по диапазону каналов после пуска, то
            Loading++;                                                                                      // увеличиваем счетчик каналов
            if(Loading>=128){lsd.fillRect (50, 50, 60, 60,ColorBG);}                                                // стираем текст
            else{lsd.setTextSize(2);lsd.setCursor(50, 50);lsd.setTextColor(ColorTextLoad);lsd.println((String)map(Loading,0,127,0,100)+"%"); lsd.setCursor(50, 50);lsd.setTextColor(ColorBG);lsd.println((String)map(Loading,0,127,0,100));}                 // выводим количество процентов загруженной информации о мощности сигналов в каналах
        }else{                                                                                              // если это не первый проход по диапазону каналов, то ...
//          Рассчитываем положение точек графиков на дисплее
            PositionPoints[0]=PositionPoints[8]-ChannelPowerMax[ChannelPrint-1]*4; if(PositionPoints[0]<PositionTop){PositionPoints[0]=PositionTop;} // координата 1 точки по оси Y для графика максимальных значений
            PositionPoints[1]=PositionPoints[8]-ChannelPowerMax[ChannelPrint  ]*4; if(PositionPoints[1]<PositionTop){PositionPoints[1]=PositionTop;} // координата 2 точки по оси Y для графика максимальных значений
            PositionPoints[2]=PositionPoints[8]-ChannelPowerNow[ChannelPrint-1]*4; if(PositionPoints[2]<PositionTop){PositionPoints[2]=PositionTop;} // координата 1 точки по оси Y для графика текущих значений
            PositionPoints[3]=PositionPoints[8]-ChannelPowerNow[ChannelPrint  ]*4; if(PositionPoints[3]<PositionTop){PositionPoints[3]=PositionTop;} // координата 2 точки по оси Y для графика текущих значений
            PositionPoints[4]=(ChannelPrint-1)*PositionWidth+PositionLeft;                                  // координата 1 точки по оси X для обоих графиков
            PositionPoints[5]= ChannelPrint   *PositionWidth+PositionLeft;                                  // координата 2 точки по оси X для обоих графиков


if(ChannelPrint==12||ChannelPrint==17||ChannelPrint==22||ChannelPrint==27||ChannelPrint==32||ChannelPrint==37
  ||ChannelPrint==42||ChannelPrint==47||ChannelPrint==52||ChannelPrint==57||ChannelPrint==62||ChannelPrint==68
  ||ChannelPrint==72||ChannelPrint==84)
 {  
   
  lsd.drawLine(PositionPoints[4],PositionPoints[2],PositionPoints[4],118,ColorScale);  // Отрисовываем сетку WI-FI каналов 
 lsd.setTextSize(0);
lsd.setTextColor(ColorScale);
lsd.setCursor(9,120);lsd.println("1");                  //выводим название шкалы WI-FI каналов
lsd.setCursor(19,120);lsd.println("3");
lsd.setCursor(29,120);lsd.println("5");
lsd.setCursor(39,120);lsd.println("7");
lsd.setCursor(49,120);lsd.println("9");
lsd.setCursor(59,120);lsd.println("1");
lsd.setCursor(69,120);lsd.println("3");
lsd.setCursor(80,120);lsd.println("4");
 }

//          Выводим графики на дисплей
             
              lsd.drawLine(PositionPoints[4]+1,0,PositionPoints[5],PositionPoints[8],ColorBG);                              // стираем с дисплея часть предыдущих графиков
             lsd.drawLine(PositionPoints[4],PositionPoints[0],PositionPoints[5],PositionPoints[1],ColorGraphMax);                // выводим график максимальных данных
              lsd.drawLine(PositionPoints[4],PositionPoints[2],PositionPoints[5],PositionPoints[3],ColorGraphNow);                // выводим график текущих данных



            
 
        } 
       
        
        ChannelPowerNow[ChannelPrint-1]=0;                                                              // стираем показания мощности для только что выведенного канала
            ChannelPrint++; if(ChannelPrint==128){ChannelPrint=1;}                                          // увеличиваем номер выводимого канала
    }
}
