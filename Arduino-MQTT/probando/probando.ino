
const int led_estado = 13;
const int caja_buena = 12;
const int caja_mala = 14;

void setup() {
  pinMode(led_estado, OUTPUT);
  pinMode(caja_buena, OUTPUT);
  pinMode(caja_mala, OUTPUT);
  Serial.begin(9600);
}

void loop() {

  DetectarMensaje();
}

void DetectarMensaje() {
  if (Serial.available()) {
    char Letra = Serial.read();
    digitalWrite(led_estado, 0);
    if (Letra == 'B') {
      Serial.println("Cambiando a caja buena");
      digitalWrite(caja_buena, HIGH);
      digitalWrite(caja_mala, LOW);
    }
    else if (Letra == 'M') {
      Serial.println("Cambiando a mala");
      digitalWrite(caja_mala, HIGH);
      delay(500);
      digitalWrite(caja_buena, LOW);
    }
  }
}
