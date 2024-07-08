#include <ServoTimer2.h>//Incluye la libreria ServoTimer2.h como alternativa a la libreria Servo.h, permite controlar los servos
ServoTimer2 servo1;//Trae la funcion servo1

#include <Adafruit_NeoPixel.h> //Incluye la libreria Adafruit_Neopixel para controlar los LED respectivos
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#include <toneAC.h>//Incluye libreria toneAC como reemplazo al uso de Tone

// Aqui se incluyen las variables generales para el buzzer
// Primero se definen las notas musicales con sus frecuencias segun la numeracion americana y la octava a la que pertenecen.

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0   //Se define un descanso con frecuencai 0 (Silencio)

// Incluye las variables generales para la Tira LED
int pixelpin = 6; //Pin en el que se conecta el LED
int pixeles = 14; //Numero de Pixeles
int intensidad=5; //Variable de intensidad
int Contador_LED1 = 0; //Contador para la variable LED
int intervalo_parpadeo=300; //Intervalo para el parpadeo de carga en ms
Adafruit_NeoPixel strip(pixeles, pixelpin, NEO_GRB + NEO_KHZ800);

int contadorsito=1; // Crea el contador "contadorsito"

// Incluye las variables generales para el Sensor Ultrasonico
int TRIG = 7 ;
int ECO = 8  ;
int RANGO_BUSQUEDA=150; //Rango o distancia de busqueda del sensor
int DISTANCIA;


void setup() {
  strip.begin();//Inicia el funcionamiento de las luces LED en color blanco
  strip.show();
  Serial.begin(9600);//Inicia el serial, el que permitiria registar y mostrar valores en el SerialMonitor
  servo1.attach(11); //Define el pin que controla al servo
  //Sensor de proximidad
  pinMode (TRIG, OUTPUT);
  pinMode (ECO, INPUT);
  //Se comienza la secuencia inicial del animatronico

  Inicio();
  //musica_inicial(); //Ejecuta la funcion musica inicial
  
}




//A continuación se definen las varias funciones utilizadas en este codigo
void Inicio(){ //Esta funcion desarrolla un movimineto inicial del animatronico que permite revisar que los actuadores esten en funcionamiento
  Serial.println("Inicio");
  pixeles=13;
  int respiraciones=3;
  int pasos=20;
  int pos=400;

  servo1.write(1500);

  for(int j=11; j<=pixeles; j++){
      strip.setPixelColor(j,strip.Color(128,0,255)); //Enciende los ojos y la boca
    }
  for(int i=0;i<=respiraciones;i++){//Realiza los ciclos de respiracion
    for(int intensidad=10;intensidad<=255;intensidad=intensidad+255/pasos){
      strip.setBrightness(intensidad);//Setea la intensidad de los LED
      strip.show();
      delay(25);}
    for(intensidad=255; intensidad>=0;intensidad=intensidad-255/pasos){
      strip.setBrightness(intensidad);//Setea la intensidad de los LED
      strip.show();
      delay(25);}
    }
  for(int j=11; j<pixeles; j++){
      strip.setPixelColor(j,strip.Color(0,0,0)); //Apaga los ojos y la boca
    }
  }


int DistanciaEnemigo(){//Crea una funcion para medir la distancia a la que se encuentra el enemigo con un sensor de ultrasonido
  Serial.println("DistanciaEnemigo");
  digitalWrite(TRIG, HIGH);
  delay(10);
  digitalWrite(TRIG, LOW);
  int DURACION = pulseIn(ECO, HIGH);
  DISTANCIA = abs(DURACION) / (58.2); //En [cm]
  Serial.print("Distancia");
  Serial.println(DISTANCIA);
  delay(100);
  return DISTANCIA;
}

// A continuacion la idea es crear una funcion que compare un numero n de lecturas de distancia enemigo para evitar la deteccion de errores
int DeteccionEnemigo(){
  Serial.println("DeteccionEnemigo");
  int Comparar=3;//Numero de comparaciones a hacer
  int Contador_Deteccion=0;
  int Distancias[Comparar]; //Array con las distancias registradas en una muestra
  int Distancia_Deteccion=-1;//Distancia final detectada
  int Promedio=0;
  int Detectado=0;

  for(Contador_Deteccion; Contador_Deteccion<Comparar; Contador_Deteccion++){//Registra un numero determinado de distancias
    Distancias[Contador_Deteccion]=DistanciaEnemigo();
    
  }
  Contador_Deteccion=0;

  for (Contador_Deteccion; Contador_Deteccion<Comparar; Contador_Deteccion++){ //Evalua si todas las distancias estan dentro del rango de busqueda
  if (Distancias[Contador_Deteccion]<RANGO_BUSQUEDA){
    Detectado=Detectado+1; 
    }
  
  }
  Contador_Deteccion=0;

  for(Contador_Deteccion; Contador_Deteccion<Comparar; Contador_Deteccion++){//Calcula el promedio del array
    Promedio=Promedio+Distancias[Contador_Deteccion]/Comparar;
    Serial.println("Promedio");
    Serial.println(Promedio);
  }
  
  if(Detectado==Comparar){Distancia_Deteccion=Promedio;} //si se cumplen las condicion entrega el valor de la distancia detectada

  return Distancia_Deteccion;
}


void Final(){//Funcion que describe el comportamiento final deseado
  Serial.println("Final");
  pixeles=14; //Considera todos los pixeles conectados para el rugido
  servo1.write(400); //Posicion "Boca Abierta" 
  int numero_parpadeos = 10; //Define el numero de parpadeos que realizara
  int frec_rugido=80; //Define la frecuencia incial del rugido 
  for(int i=1; i<=numero_parpadeos;i++){ //
    strip.setBrightness(255);//Setea la intensidad de los LED
    for(int j=0; j<pixeles; j++){
      strip.setPixelColor(j,strip.Color(0,0,0)); //Apaga los LED
      servo1.write(1000);
    }
    strip.show();
    toneAC(frec_rugido-i*(frec_rugido-31)/numero_parpadeos);	// Sonido del rugido
    delay(intervalo_parpadeo/2); //Duracion del sonido rugido
    noToneAC(); 
    for(int j=0; j<pixeles; j++){ //Enciende los LED
      strip.setPixelColor(j,strip.Color(128,0,255));
      servo1.write(800);
    }
    strip.show();
    delay(intervalo_parpadeo/2);
  }
}

void TiraLED(int Contador_LED2, int frec, int dur_in, int DISTANCIA){// Define la funcion TiraLED, una subfuncion de godzilla_roar
  Serial.print("variacion de color");
  
  int variacion_color = map(DISTANCIA,0,RANGO_BUSQUEDA,255,0);// Ya agregada
  Serial.println(variacion_color);
  int variacion_parpadeo = map(DISTANCIA,0,RANGO_BUSQUEDA,150,intervalo_parpadeo); // Aun por agregar
  
	for(Contador_LED1; Contador_LED1<=Contador_LED2;Contador_LED1++){//secuencia de encendido
    Serial.println("lento 1");
		strip.setPixelColor(Contador_LED1,strip.Color(variacion_color,0,255));// Setea el color del led "contador"
		strip.setBrightness(100);//setea la intensidad antes del parpadeo
		strip.show();
    toneAC(frec, variacion_parpadeo);
    delay(variacion_parpadeo);
    noToneAC();
    intensidad=5;
    
		for(int contador_parpadeo=Contador_LED1+1;contador_parpadeo<pixeles;contador_parpadeo++){//setea el parpadeo de las led que no estan encendidas            
			
      Serial.println("lento 2");
      strip.setPixelColor(contador_parpadeo ,strip.Color(variacion_color,0,255));
			intensidad=intensidad+(200-intensidad)/(pixeles-contador_parpadeo);
			strip.setBrightness(intensidad);//setea la intensidad de los LED
			strip.show();
			strip.setPixelColor(contador_parpadeo ,strip.Color(0,0,0));// segun yo, tiene sentido un strip.show a continuacion, pero no genera el efecto deseado al agregarlo            
			variacion_parpadeo=variacion_parpadeo*0.97;
      toneAC(frec, variacion_parpadeo);
      delay(variacion_parpadeo);
      noToneAC();
      Serial.println("lento 2.1");
			}
	}

};// Aqui termina la funcion TiraLED

void godzilla_roar(int DISTANCIA){ //Define la funcion godzilla_roar, consiste en la carga del aliento atomico de godzilla
Serial.println("GodzillaRoar");
int dur_in=200; //Duración inicial de cada tono en milisegundos
int topF=80; //Frecuencia maxima incial
int botF=50; //Frecuencia minima incial
int maxF=300; //Frecuencia maxima del rugido
int i;
int frec=topF;
int Contador_LED2=1;
Contador_LED1=0;
intervalo_parpadeo=300;
pixeles=11; //Define los pixeles utilizados para la carga
  while(topF<=maxF){
    Serial.println("entre al wile");
    for(frec=topF; frec>=botF;frec=botF-1){//La idea es que con cada ascenso de frecuencia se encienda un led
    Serial.println("entre al for");
    Serial.println("frec ");
    Serial.println(frec);
    Serial.println("topF ");
    Serial.println(topF);
    Serial.println("botF ");
    Serial.println(botF);
    TiraLED(Contador_LED2, frec, dur_in, DISTANCIA);
    }//del for "frec"
      
  //for(frec; frec>botF;frec=frec-2*(topF-botF)/pixeles){//La idea es que con cada ascenso de frecuencia se encienda un led
    
    //Serial.println("entre al for");
    //Serial.println("frec ");
    //Serial.println(frec);
    //Serial.println("topF ");
    //Serial.println(topF);
    //Serial.println("botF ");
    //Serial.println(botF);
    
    //}//del for "frec"
  	      
  if(topF<=maxF){
    Serial.println("entre al if 1");
    topF=topF+(topF-botF)/pixeles;
    if(dur_in>5){
    dur_in=dur_in*0.95;}
  }
  Contador_LED2=Contador_LED2+1;
  }//del while
  contadorsito+=1;
  Final();//Ejecuta la secuencia final del rugido
  }
// Aqui termina godzilla Roar


void musica_inicial(){ // Aqui se define la musica inicial de godzilla
int tempo = 105; // Tempo de la canción

// La duracion de cada nota correspondera a 1, en completas, 2 en blancas, 4, en negras, 6 en trecillos de negras, 8 en corcheas, 16, en semicorcheas.
// Los negativos corresponden a notas con punto, es decir, media duracion más.
int melody[] = {
  //Desde ahora escribo la de godzilla https://musescore.com/user/38391616/scores/14016238, ajustado para unitono
  NOTE_E3,1, NOTE_GS3,4, NOTE_D3,4, NOTE_DS3,4, NOTE_A2,-4,NOTE_AS2,2, REST, 2,//Compas 8, 9 y 10
  NOTE_CS3,-4, NOTE_C3,-4, NOTE_B2,2,REST,2, NOTE_B2,4, REST,4, NOTE_AS2,4,REST,4,NOTE_A2,4, REST,4,//11, 12 y 13
  NOTE_F2,6, NOTE_FS2,6, NOTE_G2,6, NOTE_GS2,4, REST,-4, //14

  //NOTE_E4,1, NOTE_GS4,8,NOTE_GS4,8, NOTE_D4,4, NOTE_DS4,4, NOTE_A3,-4,NOTE_AS3,2, REST, 2,//Compas 1, 2 y 3
  //NOTE_CS4,-4, NOTE_C4,-4, NOTE_B3,2,REST,2, NOTE_B3,-4, REST,4, NOTE_AS3,-4,REST,4,NOTE_A3,-4, REST,4,//4, 5 y 6
  //NOTE_F3,6, NOTE_FS3,6, NOTE_G3,6, NOTE_GS3,4, REST,-4, //7  

  //NOTE_A3, 8, NOTE_B3, 8, NOTE_C4, 8, REST, 8, NOTE_A3, 8, NOTE_B3, 8, NOTE_C4, 8, REST, 8, //15
  //NOTE_C4, 8, REST, 8, NOTE_C4, 8, REST, 8, NOTE_C4, 8, REST, 8, NOTE_C4, 8, REST, 8, NOTE_C4, 8, REST, 8, //16 

  //NOTE_A3, 8.5, NOTE_B3, 8.5, NOTE_C4, 8.5, REST, 8.5, NOTE_A3, 8.5, NOTE_B3, 8.5, NOTE_C4, 8.5, REST, 8.5, //
  //NOTE_C4, 8.5, REST, 8.5, NOTE_C4, 8.5, REST, 8.5, NOTE_C4, 8.5, REST, 8.5, NOTE_C4, 8.7, REST, 8.7, NOTE_C4, 8.9, REST, 8.9,
  
  //NOTE_A3, 9, NOTE_B3, 9, NOTE_C4, 9, REST, 9, NOTE_A3, 9, NOTE_B3, 9, NOTE_C4, 9, REST, 9, //
  //NOTE_C4, 9, REST, 9, NOTE_C4, 9, REST, 9, NOTE_C4, 9, REST, 9, NOTE_C4, 9, REST, 9, NOTE_C4, 9, REST, 9,
  
  //NOTE_A3, 8.9, NOTE_B3, 8.9, NOTE_C4, 8.9, REST, 8.9, NOTE_A3, 8.8, NOTE_B3, 8.8, NOTE_C4, 8.8, REST, 8.8, //
  //NOTE_C4, 8.7, REST, 8.7, NOTE_C4, 8.5, REST, 8.5, NOTE_C4, 8, REST, 8, NOTE_C4, 7.5, REST, 7.5, NOTE_C4, 5, REST, 7.5,
};
int notes = sizeof(melody) / sizeof(melody[0]) / 2; 
int wholenote = (60000 * 4) / tempo; //Duración de una nota completa
int divider = 0, noteDuration = 0;
int a=1;
for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {
    divider = melody[thisNote + 1]; //calcula la duracion de cada nota
    if (divider > 0) {
      // si es una nota normal solo calcula
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {// Para notas con punto, cambia la duracion
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // Las notas con punto duran un 50% más que sus notas normales
    }
    toneAC(melody[thisNote], noteDuration * 0.90); //Solo se toca un 90% de la duracion de la nota
    
    //Aqui se define el juego de luces para la reproduccion de la musica
    if (a==1){
    for(int ledsita=0;ledsita<pixeles;ledsita++){
    strip.setPixelColor(ledsita ,strip.Color(128,0,255));
    strip.setBrightness(50);}
    a=0;}
    else {
    for(int ledsita=0;ledsita<pixeles;ledsita++){
    strip.setPixelColor(ledsita ,strip.Color(0,0,0));}
    strip.setBrightness(90);
    a=1;}//setea la intensidad de los LED
		strip.show();//"muestra" los LED
    delay(noteDuration);
    noToneAC();//Detiene el sonido
  }
}

void musica_busqueda(){//Aqui inica musica_busqueda.
int tempo_busqueda=160;
int melody_busqueda[] = {
  NOTE_C4, 8, NOTE_B3, 8, NOTE_A3, 4, NOTE_C4, 8, NOTE_B3, 8, NOTE_A3, 4, //24
  NOTE_C4, 8, NOTE_B3, 8, NOTE_A3, 8, NOTE_G3, 8, NOTE_A3, 8, NOTE_B3, 8, NOTE_C4, 8, NOTE_B3, 8, NOTE_A3, 4,
  NOTE_C4, 8, NOTE_B3, 8, NOTE_A3, 4, NOTE_C4, 8, NOTE_B3, 8, NOTE_A3, 4, NOTE_C4, 8, NOTE_B3, 8, NOTE_A3, 4,
  //NOTE_C4, 8, NOTE_B3, 8, NOTE_A3, 8, NOTE_G3, 8, NOTE_A3, 8, NOTE_B3, 8, NOTE_C4, 8, NOTE_B3, 8, NOTE_A3, 4, //27 y 28
  //NOTE_D4, 8, NOTE_C4, 8, NOTE_B3, 4, NOTE_D4, 8, NOTE_C4, 8, NOTE_B3, 4, //29
  //NOTE_D4, 8, NOTE_C4, 8, NOTE_B3, 8, NOTE_A3, 8, NOTE_B3, 8, NOTE_C4, 8, NOTE_D4, 8, NOTE_C4, 8, NOTE_B3, 4, //30
  //NOTE_D4, 8, NOTE_C4, 8, NOTE_B3, 4, NOTE_D4, 8, NOTE_C4, 8, NOTE_B3, 4,//31
  //NOTE_D4, 8, NOTE_C4, 8, NOTE_B3, 8, NOTE_A3, 8, NOTE_B3, 8, NOTE_C4, 8, NOTE_D4, 8, NOTE_C4, 8, NOTE_B3, 8, NOTE_D4, 8,//32 y 33
  //NOTE_D4, 8, NOTE_C4, 8, NOTE_C4, 8, NOTE_B3, 8, NOTE_B3, 8, NOTE_A3, 8, 
  //NOTE_C5, 8, NOTE_B4, 8, NOTE_A4, 4, NOTE_C5, 8, NOTE_B4, 8, NOTE_A4, 4,
  //NOTE_C5, 8, NOTE_B4, 8, NOTE_A4, 8, NOTE_G4, 8, NOTE_A4, 8, NOTE_B4, 8,
  //NOTE_C5, 8, NOTE_B4, 8, NOTE_A4, 4, NOTE_C5, 8, NOTE_B4, 8, NOTE_A4, 4,//36
  //NOTE_C5, 8, NOTE_B4, 8, NOTE_A4, 4, NOTE_C5, 8, NOTE_B4, 8, NOTE_A4, 4,//37
  //NOTE_C5, 8, NOTE_B4, 8, NOTE_A4, 8, NOTE_G4, 8, NOTE_A4, 8, NOTE_B4, 8, NOTE_C5, 8, NOTE_B4, 8, NOTE_A4, 4, //38 y más
  //NOTE_D5, 8, NOTE_C5, 8, NOTE_B4, 4, NOTE_D5, 8, NOTE_C5, 8, NOTE_B4, 4,
  //NOTE_D5, 8, NOTE_C5, 8, NOTE_B4, 8, NOTE_A4, 8, NOTE_B4, 8, NOTE_C5, 8, 
  //NOTE_D5, 8, NOTE_C5, 8, NOTE_B4, 4, NOTE_D5, 8, NOTE_C5, 8, NOTE_B4, 8, REST, 8, NOTE_D5, 8, NOTE_C5, 8, NOTE_B4, 4, // 41 y más
  //NOTE_D5, 8, NOTE_C5, 8, NOTE_B4, 4, NOTE_A4, 8, NOTE_B4, 8, NOTE_C5, 8, NOTE_D5, 8, NOTE_C5, 8, //42 y 43
  //NOTE_B4, 8, NOTE_D5, 8, NOTE_D5, 8, NOTE_C5, 8, NOTE_C5, 8, NOTE_B4, 8, NOTE_B4, 8, NOTE_A4, 8, NOTE_GS4, 4, REST, 4, // 43 y 44
  //NOTE_E3, 4, NOTE_E3, 4, NOTE_E3, 4, NOTE_E3, 4, 

};
// Declaracion elementos para la cancion busqueda
int notes_busqueda = sizeof(melody_busqueda) / sizeof(melody_busqueda[0]) / 2;
int divider_busqueda = 0, noteDuration_busqueda = 0;
int wholenote_busqueda = (60000 * 4) / tempo_busqueda;
int a=1;
for (int thisNote_busqueda = 0; thisNote_busqueda < notes_busqueda * 2; thisNote_busqueda = thisNote_busqueda + 2) {
    divider_busqueda = melody_busqueda[thisNote_busqueda + 1];
    if (divider_busqueda > 0) {
      noteDuration_busqueda = (wholenote_busqueda) / divider_busqueda;
    } else if (divider_busqueda < 0) {
      noteDuration_busqueda = (wholenote_busqueda) / abs(divider_busqueda);
      noteDuration_busqueda *= 1.5; //
    }
    toneAC(melody_busqueda[thisNote_busqueda], noteDuration_busqueda * 0.90);
    if (a==1){
    for(int ledsita=0;ledsita<pixeles;ledsita++){
    strip.setPixelColor(ledsita ,strip.Color(128,0,255));
    strip.setBrightness(50);}
    a=0;}
    else {
    for(int ledsita=0;ledsita<pixeles;ledsita++){
    strip.setPixelColor(ledsita ,strip.Color(0,0,0));}
    strip.setBrightness(90);
    a=1;}//setea la intensidad de los LED
		strip.show();
    delay(noteDuration_busqueda);
    noToneAC();

}
};//Aqui termina musica_busqueda




/////////////////////////////////////
void loop() {
    Serial.println("LOOP");
    contadorsito==1;
    //musica_inicial();
    if (DISTANCIA=DeteccionEnemigo()>0){
      
      //musica_busqueda(); //Ejecuta la funcion musica_busqueda
      
      for(int j=11; j<=pixeles; j++){
        strip.setPixelColor(j,strip.Color(0,0,0)); //Apaga los ojos y la boca
      }
    Serial.println("ruge");
    Serial.print("DISTANCIA = ");
    Serial.println(DISTANCIA);
    godzilla_roar(DISTANCIA);    

    servo1.write(700); //Define un bucle final para godzilla, simplemente un moviemiento de mandibula
    delay(500);
    servo1.write(1500);
    delay(500);

    }

} //Aqui termina el void loop