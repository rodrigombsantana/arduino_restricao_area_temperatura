
// C++ code
//temperatura ideal é até 28C
//caso temperatura suba entre 28C e 30C, alerta é gerado e ventilador acionado para que temperatura reduza
//caso suba acima de 30C, alerta crítico é gerado, alerta sonoro é gerado, e acesso a area é bloqueado.
#include <Servo.h>
#include <DHT.h>
#define buzz01 7 
#define stemp01 4 
#define DHTTYPE DHT11 
#define push_button 11
#define motor 5
#define led_verm 8
#define led_verd 10
#define led_ama 9
int temp_atual=0;
int alerta=0;
int abrir = 0;
Servo cancela;
DHT dht(stemp01, DHTTYPE);

void setup()
{
  pinMode(motor, OUTPUT);
  pinMode(push_button, INPUT);
  pinMode(buzz01, OUTPUT);
  dht.begin();
  pinMode(led_verm, OUTPUT);
  pinMode(led_verd, OUTPUT);
  pinMode(led_ama, OUTPUT);
  cancela.attach(6,500,2500);
  cancela.write(90);
  delay(1000);
  Serial.begin(9600);
}

//configura semaforo e buzzer conforme alerta de temperatura
//e aciona ventilador para baixar a temperatura
void semaforo(int alerta){
  switch (alerta){
  	case 0:
    	digitalWrite(buzz01, LOW);
    	digitalWrite(led_verm, LOW);
    	digitalWrite(led_ama, LOW);
    	digitalWrite(led_verd, HIGH);
    	digitalWrite(motor,LOW);
    	break;
  	case 1:
    	digitalWrite(buzz01, LOW);
    	digitalWrite(led_verm, LOW);
    	digitalWrite(led_ama, HIGH);
    	digitalWrite(led_verd, LOW);
    	digitalWrite(motor,HIGH);
    	break;
  	case 2:
    	digitalWrite(buzz01, LOW);
    	digitalWrite(led_verm, HIGH);
    	digitalWrite(led_ama, LOW);
    	digitalWrite(led_verd, LOW);
    	digitalWrite(motor,HIGH);
    	break;
  }
}

//abre cancela caso não exista algum alerta grave de temperatura
void abrir_cancela(int alerta){
  if (alerta < 2){
      Serial.println("Abrindo Cancela");
      cancela.write(180);
  	  delay(5000);
  }
  else
  	Serial.println("Entrada proibida devido a alerta!");
  		
}
void fechar_cancela(){
  cancela.write(90);
  delay(1000);
}


void loop()
{

  //ler temperatura
  temp_atual = dht.readTemperature();
  Serial.print("Temperatura:");
  Serial.println(temp_atual);
  
  if (temp_atual <28)
  {
    //acesso normal. 
    digitalWrite(buzz01, LOW);
    alerta = 0;
    semaforo(alerta);
  }
  if (temp_atual >=28 && temp_atual <=30){
  	//acesso normal, porem alerta é gerado, e ventilador ligado
    Serial.println("Temperatura acima do esperado");
    Serial.println("Ligar ventilador!");
    digitalWrite(buzz01, LOW);
    alerta = 1;
    semaforo(alerta);
  }
  if (temp_atual >30){
    //acesso proibido, alerta critico
    Serial.println("Alerta Critico!!!");
  	digitalWrite(buzz01, HIGH);
    alerta = 2;
    semaforo(alerta);
  }

  //abrir cancela
  abrir = digitalRead(push_button);
  if (abrir == HIGH)
    abrir_cancela(alerta);
  else
    fechar_cancela();
  
}
