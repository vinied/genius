#define LED_VERDE 2
#define LED_AMARELO 3
#define LED_VERMELHO 4
#define LED_AZUL 5

#define BOTAO_VERDE 9
#define BOTAO_AMARELO 10
#define BOTAO_VERMELHO 11
#define BOTAO_AZUL 12

#define BUZZER 6

#define UM_SEGUNDO 1000
#define MEIO_SEGUNDO 500
#define TAM_SEQ 4
#define N_LEDS 4 
#define NADA -1

enum Estados {
  PRONTO_PROX_RODADA,
  USUARIO_RESPONDENDO,
  JOGO_FINALIZADO_SUCESSO,
  JOGO_FINALIZADO_FALHA
};

int sequencia[TAM_SEQ];
int toque[N_LEDS] = {500, 600, 700, 800};
int botoes[TAM_SEQ];
int flag_botao[TAM_SEQ] = {0, 0, 0, 0};
int rodada;
int ans;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  iniciaPortas();
  iniciaJogo();
  rodada = 0;
  ans = 0;
}

void iniciaJogo(){
  int jogo = analogRead(0);
  randomSeed(jogo);
  for (int i = 0 ; i < TAM_SEQ ; i++) {
    sequencia[i]= sorteiaCor();
  }

  botoes[0] = BOTAO_VERDE;
  botoes[1] = BOTAO_AMARELO;
  botoes[2] = BOTAO_VERMELHO;
  botoes[3] = BOTAO_AZUL;
}

int sorteiaCor() {
  return random(LED_VERDE,LED_AZUL+1);
}


void loop() {
  switch(getEstadoAtual()) { 
    case PRONTO_PROX_RODADA:
      //Serial.println("PRONTO_PROX_RODADA");
      cortinaLed();
      delay(MEIO_SEGUNDO);
      preparaNovaRodada();
      break;
    case USUARIO_RESPONDENDO:
      //Serial.println("USUARIO_RESPONDENDO");
      processaRespostas();
      break;
    case JOGO_FINALIZADO_SUCESSO:
      Serial.println("JOGO_FINALIZADO_SUCESSO");
      piscaTudo();
      break;
    case JOGO_FINALIZADO_FALHA:
      Serial.println("Game Over!!!");
      tocaSom(300);
      piscaSequenciaGameOver();
      break;
    
  }
  //delay(MEIO_SEGUNDO);
  //mapearBotaoNoLed();
}

int getEstadoAtual() {
  if(getRodada() <=  TAM_SEQ) {
    if(ans == rodada) {
      delay(1000);
      return PRONTO_PROX_RODADA;
    } else {
      return USUARIO_RESPONDENDO;
    }
  } else if(rodada == TAM_SEQ + 1) {
    return JOGO_FINALIZADO_SUCESSO;
  } else {
    return JOGO_FINALIZADO_FALHA;
  }
}

int getRodada() {
  return rodada;
}

void preparaNovaRodada() {
  rodada++;
  ans = 0;
  if(rodada <= TAM_SEQ) {
   executaStage(); 
  }
}

void processaRespostas() {
  int res = checaRespostaJogador();
  int seq_normalizada = sequencia[ans] - 2;
  
  if(res == seq_normalizada) {
    digitalWrite(sequencia[ans], HIGH);
    delay(500);
    digitalWrite(sequencia[ans], LOW);
    ans++;
    zera_flag();
  } else if(res != NADA && !flag_botao[res]) {
    flag_botao[res] = 1;
    Serial.println(res);
    Serial.println(seq_normalizada);
    Serial.println("Bahhh errou");
    rodada = TAM_SEQ + 2;
  }
}

void zera_flag() {
  for(int i = 0 ; i < TAM_SEQ ; i++) {
   flag_botao[i] = 0; 
  }
}

void executaStage() {
  for( int i = 0 ; i < getRodada() ; i++ ) {
    piscaLed(sequencia[i]);
  }
}


int checaRespostaJogador() {
  for( int i = 0 ; i < TAM_SEQ ; i++ ) {
    if(!digitalRead(botoes[i])) {
      return i;
    }
  }
  return NADA;   
}

void mapearBotaoNoLed() {
  for( int i = 0 ; i < TAM_SEQ ; i++ ) {
     if(!digitalRead(botoes[i])) {
      digitalWrite(sequencia[i], HIGH);
      if(!flag_botao[i]) {
        Serial.println(sequencia[i]);
        flag_botao[i] = 1;
      }
    } else {
      flag_botao[i] = 0;
      digitalWrite(sequencia[i], LOW);
    }
  }
}

void iniciaPortas() {
  pinMode(LED_VERDE, OUTPUT);
  pinMode(BOTAO_VERDE, INPUT_PULLUP);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(BOTAO_AMARELO, INPUT_PULLUP);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(BOTAO_VERMELHO, INPUT_PULLUP);
  pinMode(LED_AZUL, OUTPUT);
  pinMode(BOTAO_AZUL, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
}

void tocaSom(int frequencia) {
  tone(BUZZER, frequencia, 100);
}

void piscaSequencia1() {
  piscaLed(LED_VERDE);
  piscaLed(LED_AMARELO);
  piscaLed(LED_VERMELHO);
  piscaLed(LED_AZUL);
}

void piscaSequenciaGameOver() {
  delay(150);
  digitalWrite(LED_VERDE, HIGH);
  digitalWrite(LED_AMARELO, HIGH);
  digitalWrite(LED_VERMELHO, HIGH);
  digitalWrite(LED_AZUL, HIGH);
  delay(150);
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_AMARELO, LOW);
  digitalWrite(LED_VERMELHO, LOW);
  digitalWrite(LED_AZUL, LOW);
}

void piscaLed(int porta_led) {
  digitalWrite(porta_led, HIGH);
  tocaSom(toque[porta_led-2]);
  delay(UM_SEGUNDO); //delays são em ms
  digitalWrite(porta_led, LOW);
  delay(MEIO_SEGUNDO); //delays são em ms
}

void cortinaLed() {
  mudaSeco(LED_VERDE);
  mudaSeco(LED_AMARELO);
  mudaSeco(LED_VERMELHO);
  mudaSeco(LED_AZUL);
}


void mudaSeco(int porta_led) {
  digitalWrite(porta_led, HIGH);
  delay(80); //delays são em ms
  digitalWrite(porta_led, LOW);
}

void piscaTudo() {
  delay(UM_SEGUNDO);
  digitalWrite(LED_VERDE, HIGH);
  digitalWrite(LED_AMARELO, HIGH);
  digitalWrite(LED_VERMELHO, HIGH);
  digitalWrite(LED_AZUL, HIGH);
  delay(UM_SEGUNDO);
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_AMARELO, LOW);
  digitalWrite(LED_VERMELHO, LOW);
  digitalWrite(LED_AZUL, LOW);
}
