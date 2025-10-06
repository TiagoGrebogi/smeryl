#define IN1 12  // fio azul
#define IN2 14  // fio vermelha
#define IN3 27  // fio amarelo
#define IN4 26  // fio verde

#define SENSOR_DIR 33   // sensor E18 direita
#define SENSOR_ESQ 32   // sensor E18 esquerda
#define SENSOR_LINHA_ESQ 13 
#define SENSOR_LINHA_DIR 35  
#define SENSOR_LINHA_TRAS 34 
#define PIN_BOTAO 25    // botão

bool iniciado = false;
bool linhaDetectada = false;

bool obstaculoDetectado(int pino) {
  if (digitalRead(pino) == LOW) {                      
    if (digitalRead(pino) == LOW) { 
      return true;
    }
  }
  return false;
}

void frente() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void tras() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void esquerda() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void direita() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void parar() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

bool primeiraLinhaBranca() {
  if (iniciado) {
    frente();
    if (digitalRead(SENSOR_LINHA_ESQ) == LOW || digitalRead(SENSOR_LINHA_DIR) == LOW) {
      Serial.println("Linha branca detectada pela primeira vez!");
      linhaDetectada = true; // ativa modo normal
    }
  }
  return linhaDetectada;
}








void setup() {
  Serial.begin(115200);

  // pinos motores
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // sensores
  pinMode(SENSOR_DIR, INPUT);
  pinMode(SENSOR_ESQ, INPUT);
  pinMode(SENSOR_LINHA_ESQ, INPUT);
  pinMode(SENSOR_LINHA_DIR, INPUT);
  pinMode(SENSOR_LINHA_TRAS, INPUT);

  pinMode(PIN_BOTAO, INPUT_PULLUP);

  delay(3000);
  parar();
  Serial.println("Aperte o botão para iniciar...");
}







void loop() {


  if (!iniciado) {
    if (digitalRead(PIN_BOTAO) == LOW) {
      Serial.println("Botão pressionado! Iniciando em 2s...");
      delay(2000);
      iniciado = true;
    } else {
      parar();
      return;
    }
  }

  while (!linhaDetectada) {
    frente();
    Serial.println("Buscando a primeira Linha Branca");
    primeiraLinhaBranca();
  }

  // dar ré ao ver linha
  if (digitalRead(SENSOR_LINHA_ESQ) == LOW || digitalRead(SENSOR_LINHA_DIR) == LOW) {
    Serial.println("Linha frente detectada! Recuando...");
    tras();
    delay(1000);
    parar();
    return;
  }

  if (digitalRead(SENSOR_LINHA_TRAS) == LOW) {
    Serial.println("Linha tras detectada! Recuando...");
    frente();
    delay(1000);
    parar();
    return;
  }
  // procura adversário usando sensores E18
  bool dir = obstaculoDetectado(SENSOR_DIR);
  bool esq = obstaculoDetectado(SENSOR_ESQ);

  if (dir && esq) {
    frente();
    Serial.println("Frente");
  } 
  else if (dir && !esq) {
    direita();
    Serial.println("Direita");

  } 
  else if (!dir && esq) {
    esquerda();
    Serial.println("Esquerda");

  } 
  else {
    direita();
    Serial.println("Buscando Inimigo");

  }
}
