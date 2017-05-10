#define NOTE_E7  2637
#define NOTE_C7  2093
#define NOTE_G6  1568
#define NOTE_B6  1976

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

void setup() {
  Serial.begin(9600);
  pinMode(melodyPin,OUTPUT);

  //led
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(led3,OUTPUT);
  pinMode(led4,OUTPUT);
  
  Serial.println("Iniciado");
  iniciar();
}

void loop() {
  sensorValue = analogRead(sensorPin);
  sensorValue1 = analogRead(sensorPin1);

  int boton = 5;

  if(sensorValue >= 500 && sensorValue <=710) {
    Serial.print(sensorValue);
    Serial.println(" boton 1");
    boton = 0;   
  } else if(sensorValue > 710 && sensorValue <=1000) {
    Serial.print(sensorValue);
    Serial.println(" boton 2");    
    boton = 1;
  }

  if(sensorValue1 >= 500 && sensorValue1 <=710) {
    Serial.print(sensorValue1);
    Serial.println(" boton 3");
    boton = 2;
  } else if(sensorValue1 > 710 && sensorValue1 <=1000) {
    Serial.print(sensorValue1);
    Serial.println(" boton 4");    
    boton = 3;
  }

  if(boton < 4) {
    tocarNota(boton);
  }
  
  delay(200);
}

void apagar() {
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
}

void tocarNota(int thisNote) {
      // to calculate the note duration, take one second
      // divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      //int noteDuration = 1000 / tempo[thisNote];
      int noteDuration = 1000 / 12;

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
