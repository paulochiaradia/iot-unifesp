// --- Pinos de comunicação com o Arduino ---
// Conecte estes pinos ao conversor de nível lógico
const int pinoSaida1 = 15; // Vai para a entrada1 do Arduino
const int pinoSaida2 = 2;  // Vai para a entrada2 do Arduino

// Tempos para transição de estado (em milissegundos)
const long cleaningTime = 2000;
const long dockingTime = 2000;
const long chargingTime = 3000;

// Definição dos estados do sistema
enum State {
  IDLE,
  CLEANING,
  DOCKING,
  CHARGING
};
State currentState = IDLE;

// Variável para controlar o tempo em cada estado
unsigned long stateTransitionMillis = 0;

// Função que envia o estado atual para o Arduino através dos pinos de saída
void enviarEstadoParaArduino() {
  switch (currentState) {
    case IDLE:
      digitalWrite(pinoSaida1, LOW);
      digitalWrite(pinoSaida2, LOW);
      break;
    case CLEANING:
      digitalWrite(pinoSaida1, HIGH);
      digitalWrite(pinoSaida2, LOW);
      break;
    case DOCKING:
      digitalWrite(pinoSaida1, LOW);
      digitalWrite(pinoSaida2, HIGH);
      break;
    case CHARGING:
      digitalWrite(pinoSaida1, HIGH);
      digitalWrite(pinoSaida2, HIGH);
      break;
  }
}


void setup() {
  Serial.begin(9600);
  
  // Configura os pinos de comunicação como SAÍDA
  pinMode(pinoSaida1, OUTPUT);
  pinMode(pinoSaida2, OUTPUT);
  
  // Define o estado inicial e já o envia para o Arduino
  currentState = IDLE;
  enviarEstadoParaArduino(); 
  Serial.println("Sistema iniciado. Estado: IDLE");
}

void loop() {
  // --- Bloco 1: Receber comandos via Serial para forçar a mudança de estado (para testes) ---
  if (Serial.available() > 0) {
    char receivedChar = Serial.read();
    if (receivedChar == 'a' && (currentState == IDLE || currentState == CHARGING)) {
      currentState = CLEANING;
      stateTransitionMillis = millis(); // Reinicia o cronômetro do estado
      Serial.println("Comando 'a' recebido. Mudando para CLEANING...");
    } else if (receivedChar == 'b' && currentState == CLEANING) {
      currentState = DOCKING;
      stateTransitionMillis = millis(); // Reinicia o cronômetro do estado
      Serial.println("Comando 'b' recebido. Mudando para DOCKING...");
    }
  }

  // --- Bloco 2: Lógica de transição automática de estados baseada no tempo ---
  unsigned long currentMillis = millis();
  switch (currentState) {
    case CLEANING:
      if (currentMillis - stateTransitionMillis >= cleaningTime) {
        currentState = DOCKING;
        stateTransitionMillis = millis();
        Serial.println("Tempo de CLEANING esgotado, mudando para DOCKING...");
      }
      break;
    case DOCKING:
      if (currentMillis - stateTransitionMillis >= dockingTime) {
        currentState = CHARGING;
        stateTransitionMillis = millis();
        Serial.println("Tempo de DOCKING esgotado, mudando para CHARGING...");
      }
      break;
    case CHARGING:
      if (currentMillis - stateTransitionMillis >= chargingTime) {
        currentState = IDLE;
        Serial.println("Carga completa, mudando para IDLE...");
      }
      break;
    default:
      // No estado IDLE, não faz nada até receber um comando
      break;
  }
  
  // --- Bloco 3: Envia o estado atualizado para o Arduino a cada ciclo ---
  enviarEstadoParaArduino();
}