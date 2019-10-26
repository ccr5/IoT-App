#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <PushButton.h>

const int door = 2;
const int lock = 3;
const int button = 10

LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3, POSITIVE);
SoftwareSerial mySerial(7, 8);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
PushButton botao(pinBot);

uint8_t numID = 1;
bool gravar=false;

uint8_t modoGravacaoID(uint8_t IDgravar);

void setup() {

  pinMode(door, INPUT);
  pinMode(lock, OUTPUT);
  lcd.begin(16,2);
  lcd.clear();
  lcd.print("Exponencial");

  Serial.begin(9600);
  finger.begin(57600);

  if(finger.verifyPassword()){
    Serial.println("Sensor biometrico encontrado!");
  } else {
    Serial.println("Sensor biometrico não encontrado! Verifique a conexão e reinicie o sistema");
    while(true) { 
      delay(1); 
      }
  }
  
}

void loop() {

  botao.button_loop();
    
  if ( botao.pressed() ) {
    gravar = true;
    
  } else if(gravar) {
    
    modoGravacaoID(0);
    gravar = false;
    
  }

  getFingerprintIDez();
  
}

uint8_t modoGravacaoID(uint8_t IDgravar) {

  int p = -1;
  Serial.print("Esperando uma leitura válida para gravar #"); 
  Serial.println(IDgravar);
  delay(2000);
  
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Leitura concluída");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(".");
      delay(200);
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Erro comunicação");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Erro de leitura");
      break;
    default:
      Serial.println("Erro desconhecido");
      break;
    }
  }

  // OK successo!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Leitura convertida");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Leitura suja");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Erro de comunicação");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Não foi possível encontrar propriedade da digital");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Não foi possível encontrar propriedade da digital");
      return p;
    default:
      Serial.println("Erro desconhecido");
      return p;
  }
  
  Serial.println("Remova o dedo");
  delay(2000);
  p = 0;
  
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  
  Serial.print("ID "); Serial.println(IDgravar);
  p = -1;
  Serial.println("Coloque o Mesmo dedo novamente");
  
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Leitura concluída");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      delay(200);
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Erro de comunicação");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Erro de Leitura");
      break;
    default:
      Serial.println("Erro desconhecido");
      break;
    }
  }

  // OK successo!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Leitura convertida");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Leitura suja");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Erro de comunicação");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Não foi possível encontrar as propriedades da digital");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Não foi possível encontrar as propriedades da digital");
      return p;
    default:
      Serial.println("Erro desconhecido");
      return p;
  }
  
  // OK convertido!
  Serial.print("Criando modelo para #");  Serial.println(IDgravar);
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("As digitais batem!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Erro de comunicação");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("As digitais não batem");
    return p;
  } else {
    Serial.println("Erro desconhecido");
    return p;
  }   
  
  Serial.print("ID "); Serial.println(IDgravar);
  p = finger.storeModel(IDgravar);
  if (p == FINGERPRINT_OK) {
    Serial.println("Armazenado!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Erro de comunicação");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Não foi possível gravar neste local da memória");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Erro durante escrita na memória flash");
    return p;
  } else {
    Serial.println("Erro desconhecido");
    return p;
  }   
}

int getFingerprintIDez() {
  
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  //Encontrou uma digital!
  if (finger.fingerID == 0) {
     Serial.print("Modo Administrador!");
     
     numID++;
     modoGravacaoID(numID);
     return 0; 
  
  } else {

     int result = digitalRead(door);
     if (result == HIGH) {
        digitalWrite(lock, HIGH);
        Serial.print("ID encontrado #"); 
        Serial.print(finger.fingerID); 
        Serial.print(" com confiança de ");
        Serial.println(finger.confidence);
        lcd.clear();
        lcd.print("Lock is opened!");
        delay(1000);
        lcd.clear();
        return finger.fingerID;
        
      } else if (result == LOW) {
        digitalWrite(lock, LOW);
        Serial.print("ID encontrado #"); 
        Serial.print(finger.fingerID); 
        Serial.print(" com confiança de ");
        Serial.println(finger.confidence);
        lcd.clear();
        lcd.print("Lock is closed!");
        delay(1000);
        lcd.clear();
        return finger.fingerID;
      } 
}
