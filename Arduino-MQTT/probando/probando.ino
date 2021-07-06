#define posicion_mandarina 0
#define posicion_pera 120
#define posicion_uva 240
int GradoActual = 0;
int GradoDeseado = 0;

const int BotonIzquierda = 16;
const int BotonDerecha = 17;

const int P_Step = 19;
const int P_Dir = 18;
const int led_estado = 13;
const int led_mandarina = 12;
const int led_pera = 14;
const int led_uva = 27;

void setup() {
  pinMode(P_Step, OUTPUT);
  pinMode(P_Dir, OUTPUT);
  pinMode(led_estado, OUTPUT);
  pinMode(led_mandarina, OUTPUT);
  pinMode(led_pera, OUTPUT);
  pinMode(led_uva, OUTPUT);
  pinMode(BotonIzquierda, INPUT);
  pinMode(BotonDerecha, INPUT);
  Serial.begin(9600);
}

void loop() {

  ActualizarPosicionMotores();
  ActualizarBotones();
  DetectarMensaje();
}
void ActualizarPosicionMotores() {
  Serial.print(GradoActual);
  Serial.print("-");
  Serial.print(GradoDeseado);
  Serial.println();
  if (GradoActual < GradoDeseado) {
    SubirGrado();
  }
  else if ( GradoActual > GradoDeseado) {
    BajarGrado();
  }
}

void SubirGrado() {
  Serial.println("Moviendo Arriba");
  GradoActual++;
  MoverMotor(1);
}

void BajarGrado() {
  Serial.println("Moviendo Abajo");
  GradoActual--;
  MoverMotor(-1);
}

void MoverMotor(int Pasos) {
  if (Pasos > 0) {
    digitalWrite(P_Dir, HIGH);
  } else {
    digitalWrite(P_Dir, LOW);
    Pasos = -Pasos;
  }
  for (int i = 0; i < Pasos; i++) {
    digitalWrite(P_Step, HIGH);
    delay(10);
    digitalWrite(P_Step, LOW);
    delay(10);
  }
}

void ActualizarBotones() {
  if (digitalRead(BotonIzquierda)) {
    Serial.println("Reiniciando hacia la izquierda");
    MoverMotor(1);
    GradoActual = 0;
    GradoDeseado = 0;
    digitalWrite(led_mandarina, LOW);
    digitalWrite(led_pera, LOW);
    digitalWrite(led_uva, LOW);
  }
  if (digitalRead(BotonDerecha)) {
    Serial.println("Reiniciando hacia la derecha");
    MoverMotor(-1);
    GradoActual = 0;
    GradoDeseado = 0;
    digitalWrite(led_mandarina, LOW);
    digitalWrite(led_pera, LOW);
    digitalWrite(led_uva, LOW);
  }
}

void DetectarMensaje() {
  if (Serial.available()) {
    char Letra = Serial.read();
    digitalWrite(led_estado, 0);
    if (Letra == 'M') {
      GradoDeseado = posicion_mandarina;
      Serial.println("Cambiando a Mandarina");
      digitalWrite(led_mandarina, HIGH);
      digitalWrite(led_pera, LOW);
      digitalWrite(led_uva, LOW);
    }
    else if (Letra == 'P') {
      GradoDeseado = posicion_pera;
      Serial.println("Cambiando a Pera");
      digitalWrite(led_pera, HIGH);
      digitalWrite(led_mandarina, LOW);
      digitalWrite(led_uva, LOW);
    } else if (Letra == 'U') {
      GradoDeseado = posicion_uva;
      Serial.println("Cambiando a Uva");
      digitalWrite(led_uva, HIGH);
      digitalWrite(led_pera, LOW);
      digitalWrite(led_mandarina, LOW);
    }
  }
}
