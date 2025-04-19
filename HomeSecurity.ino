#define TRIGGER_PIN 7
#define ECHO_PIN 6
#define LED_VERDE_PIN 9
#define LED_VERMELHO_PIN 10
#define BUZZER_PIN 8
#define BOTAO_PIN 3

class SensorMovimento {
  private:
    int triggerPin;
    int echoPin;
    long duracao;
    int distancia;
    int limiteInvasao;
  public:
    SensorMovimento(int trigger, int echo, int limite = 100) : triggerPin(trigger), echoPin(echo), limiteInvasao(limite) {
      pinMode(triggerPin, OUTPUT);
      pinMode(echoPin, INPUT);
    }
    long medirDistancia() {
      digitalWrite(triggerPin, LOW);
      delayMicroseconds(2);
      digitalWrite(triggerPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(triggerPin, LOW);
      duracao = pulseIn(echoPin, HIGH);
      distancia = duracao * 0.034 / 2;
      return distancia;
    }
    bool detectarInvasao() {
      return medirDistancia() < limiteInvasao;
    }
};

class Led {
  private:
    int pin;
  public:
    Led(int pin) {
      this->pin = pin;
      pinMode(pin, OUTPUT);
    }
    void ligar() {
      digitalWrite(pin, HIGH);
    }
    void desligar() {
      digitalWrite(pin, LOW);
    }
};

class Buzzer {
  private:
    int pin;
  public:
    Buzzer(int pin) {
      this->pin = pin;
      pinMode(pin, OUTPUT);
    }
    void ligar() {
      digitalWrite(pin, HIGH);
    }
    void desligar() {
      digitalWrite(pin, LOW);
    }
    
};

class Usuario {
  private:
    int botaoPin;
    int estadoBotao;
    int estadoBotaoAnt;
    bool sistemaLigado;
  public:
    Usuario(int botao) : botaoPin(botao), estadoBotaoAnt(HIGH), sistemaLigado(false) {
      pinMode(botaoPin, INPUT_PULLUP);
    }
    bool alternarSistema() {
      estadoBotao = digitalRead(botaoPin);
      if (estadoBotao != estadoBotaoAnt && estadoBotao == LOW) {
        sistemaLigado = !sistemaLigado;
        delay(200);
      }
      estadoBotaoAnt = estadoBotao;
      return sistemaLigado;
    }
};

class SistemaSeguranca {
  private:
    SensorMovimento sensor;
    Led ledVerde;
    Led ledVermelho;
    Buzzer buzzer;
    Usuario usuario;

  public:
    SistemaSeguranca(int triggerPin, int echoPin, int ledVerdePin, int ledVermelhoPin, int buzzerPin, int botaoPin) 
      : sensor(triggerPin, echoPin), ledVerde(ledVerdePin), ledVermelho(ledVermelhoPin), buzzer(buzzerPin), usuario(botaoPin) {}

    void monitorar() {
      if (usuario.alternarSistema()) {
        ledVerde.ligar();
        if (sensor.detectarInvasao()) {
          ledVermelho.ligar();
          buzzer.ligar();
        } else {
          ledVermelho.desligar();
          buzzer.desligar();
        }
      } else {
        ledVerde.desligar();
        ledVermelho.desligar();
        buzzer.desligar();
      }
    }
};

SistemaSeguranca sistema(TRIGGER_PIN, ECHO_PIN, LED_VERDE_PIN, LED_VERMELHO_PIN, BUZZER_PIN, BOTAO_PIN);

void setup() {
  Serial.begin(9600);
}

void loop() {
  sistema.monitorar();
  delay(100);
}