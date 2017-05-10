#define NOTE_E7  2637
#define NOTE_C7  2093
#define NOTE_G6  1568
#define NOTE_B6  1976

#define NOTE_B0  31

#define melodyPin 3

#define led1 7
#define led2 6
#define led3 5
#define led4 4

//Mario main theme melody
int melody[] = { NOTE_E7, NOTE_C7, NOTE_G6, NOTE_B6 };

int sensorPin = A0;
int sensorPin1 = A1;

int sensorValue = 0;
int sensorValue1 = 0;

boolean juego = false;
int indice = 0;
int nivel = 0;
int secuencia[80];
int jugador[80];

void setup() {
  Serial.begin(9600);
  pinMode(melodyPin,OUTPUT);

  //led
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(led3,OUTPUT);
  pinMode(led4,OUTPUT);
  
  Serial.println("Iniciado");
  generarSecuencia();
}

void loop() {

  if(juego) {

    sensorValue = analogRead(sensorPin);
    sensorValue1 = analogRead(sensorPin1);
  
    int boton = 5;
  
    if(sensorValue >= 500 && sensorValue <=710) {
      boton = 0;   
    } else if(sensorValue > 710 && sensorValue <=1000) {
      boton = 1;
    }
  
    if(sensorValue1 >= 500 && sensorValue1 <=710) {
      boton = 2;
    } else if(sensorValue1 > 710 && sensorValue1 <=1000) {
      boton = 3;
    }
  
    if(boton < 4) {
      jugador[indice] = boton;
      tocarNota(boton);
    
      if(jugador[indice] == secuencia[indice]) {
          Serial.println("Nota igual a secuencia");
          indice++;
          if(indice < nivel) {
            Serial.println("Espera la siguiente nota");
          } else {
            Serial.println("completo nivel");
            nivel++;
            juego = false;
          }
        } else {
          Serial.println("game over");
          gameOver();
          generarSecuencia();
        }
    }

  
  } else {

    if(nivel > 0) {
      Serial.println("Reproduce melodia");
      for (int i = 0; i < nivel; i = i + 1) {
        Serial.print(secuencia[i]);
        Serial.print(" ");
        jugador[i] = -1;
        tocarNota(secuencia[i]);
      }
      Serial.println(" ");
    } else {
      tocarNota(0);
      Serial.println("Reproduce melodia");
      //generarSecuencia();
    }
    juego = true;
    indice = 0;
    
    
  }
  delay(200);
  
}


void generarSecuencia() {
    Serial.println("Genera secuencia");
    for (int i = 0; i < 80; i = i + 1) {
      secuencia[i] = random(0,3);
      jugador[i] = -1;
    }
    juego = false;
    indice = 0;
    nivel = 0;    
}

void gameOver() {
      int noteDuration = 1000 / 2;

      buzz(melodyPin,NOTE_B0, noteDuration,0);

      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);

      // stop the tone playing:
      buzz(melodyPin, 0, noteDuration,0);    
}

void tocarNota(int thisNote) {
      // to calculate the note duration, take one second
      // divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      //int noteDuration = 1000 / tempo[thisNote];
      int noteDuration = 1000 / 4;

      buzz(melodyPin, melody[thisNote], noteDuration,thisNote);

      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);

      // stop the tone playing:
      buzz(melodyPin, 0, noteDuration,thisNote);    
}

void iniciar() {
  int size = sizeof(melody) / sizeof(int);
  for (int thisNote = 0; thisNote < size; thisNote++) {

    tocarNota(thisNote);
  }
}

void buzz(int targetPin, long frequency, long length,int thisNote) {
  
  long delayValue = 1000000 / frequency / 2; // calculate the delay value between transitions
  //// 1 second's worth of microseconds, divided by the frequency, then split in half since
  //// there are two phases to each cycle
  long numCycles = frequency * length / 1000; // calculate the number of cycles for proper timing
  //// multiply frequency, which is really cycles per second, by the number of seconds to
  //// get the total number of cycles to produce
  for (long i = 0; i < numCycles; i++) { // for the calculated length of time...
    digitalWrite((7-thisNote), HIGH);
    digitalWrite(targetPin, HIGH); // write the buzzer pin high to push out the diaphram
    delayMicroseconds(delayValue); // wait for the calculated delay value
    digitalWrite((7-thisNote), LOW);
    digitalWrite(targetPin, LOW); // write the buzzer pin low to pull back the diaphram
    delayMicroseconds(delayValue); // wait again or the calculated delay value
  }
}
