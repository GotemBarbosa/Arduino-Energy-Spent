#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int v = 110; //voltagem
int amp = 40; //valor da amperagem (com media iniciando em 40)
int temp = 0; //tempo
float pot = 0; //potencia
float potA = 0; //Potencia atual
float Wh; //potencia hora
int limit = 3600;  //quantidade de parametros
float kpot; //potencia em Kwh
float tarifaPadrao = 0.30;
float alteracaoTf = 0;
float tarifa = 0;
double value = 0; //preço

int hora = 0;



void setup(){
    lcd.begin(16,2);
 
	pinMode(A5, INPUT);
  	pinMode(6, OUTPUT);
  	pinMode(7, INPUT);
   	analogWrite(6,30);
   	Serial.begin(9600);
}


int GetAmperagem(int x){
    /* gerando valores aleatórios variados */

    int valormenor = (x - 10);
    int valormaior = (x + 10);

    int y = random(valormenor, valormaior);
    
    if(y > 90){
        y = random(80, 85);
    }
    if(y < 0){
        y = 0;
    }
  	return y;
}



void loop(){
  
  //botão e reset for acionado por mais de um segundo
  if(digitalRead(7)== HIGH){
    pot = 0;
    temp = 0; 
    potA = 0;
    value = 0;
  }
    temp++;
    lcd.clear();
  
  	//altera o preço da tarifa pelo potenciometro
  	alteracaoTf= map(analogRead(A5), 0, 1023, -3, 10);
  	tarifa = tarifaPadrao + (alteracaoTf/10);
  
	//amperagem recebe o valor do sensor.
    amp = GetAmperagem(amp);
	
  	//calculo da potencia total e atual
    potA = amp * v;
    pot += potA;


  	//após atingir o tempo limite (1 hora) o calculo é 
  	//atualizado e o novo preço surge na tela
  	//assim a cada uma hora o preço atualiza e o contador(temp) é reiniciado 
    if(temp == limit){
        Wh = pot/limit;
        kpot = Wh /1000;
        value += kpot * tarifa;
      
        temp = 0;
        pot = 0;
      
      	hora++;
    }

    //potencia
    lcd.setCursor(0, 0);
    lcd.print("W:");
    lcd.setCursor(2,0);
    lcd.print((int)potA);

    //tarifa
    lcd.setCursor(7,0);
    lcd.print("Tf");
    lcd.setCursor(10,0);
    lcd.print(tarifa);

    //preço
    lcd.setCursor(0,1);
    lcd.print("Preco: RS");
    lcd.setCursor(10,1);
    lcd.print(value);

  
  	Serial.print("hora: ");
  	Serial.println(hora);


    delay(1000);
}