//Código para controle de motores com driver Monster Motor Shield VNH2SP30

#define BRAKE 0
#define CW    1       //Sentido horário
#define CCW   2       //Sentido Anti-horário
#define CS_THRESHOLD 8 //Definição para corrente máxima segura

//MOTOR 1              //  CW  | CCW         
#define MOTOR_A1_PIN 7 // HIGH | LOW   
#define MOTOR_B1_PIN 8 // LOW  | HIGH 

//MOTOR 2              //  CW  | CCW  
#define MOTOR_A2_PIN 4 // HIGH | LOW 
#define MOTOR_B2_PIN 9 // LOW  | HIGH

#define PWM_MOTOR_1 5
#define PWM_MOTOR_2 6

#define CURRENT_SEN_1 A2
#define CURRENT_SEN_2 A3

#define EN_PIN_1 A0
#define EN_PIN_2 A1

#define MOTOR_1 0
#define MOTOR_2 1

#define axisXpin A4 //Pino do eixo X do JoyStick

int vel = 0;        //Velocidade padrão zero
int direct = BRAKE; //Sentido padrão parado
int axisX;          //valor retornado pelo joy
void setup() {
  pinMode(MOTOR_A1_PIN, OUTPUT);
  pinMode(MOTOR_B1_PIN, OUTPUT);

  pinMode(MOTOR_A2_PIN, OUTPUT);
  pinMode(MOTOR_B2_PIN, OUTPUT);

  pinMode(PWM_MOTOR_1, OUTPUT);
  pinMode(PWM_MOTOR_2, OUTPUT);

  pinMode(CURRENT_SEN_1, OUTPUT);
  pinMode(CURRENT_SEN_2, OUTPUT);  

  pinMode(EN_PIN_1, OUTPUT);
  pinMode(EN_PIN_2, OUTPUT);

 // Serial.begin(9600);   
}

void loop() {

  delay(200);
  
  digitalWrite(EN_PIN_1, HIGH);
  digitalWrite(EN_PIN_2, HIGH); 
  
  axisX = analogRead(axisXpin);

  vel = joyToPWM(axisX);
  direct = joyToDirection(axisX);

  motorGo(MOTOR_1, direct, vel);
  motorGo(MOTOR_2, direct, vel);
  
  if(analogRead(CURRENT_SEN_1) > CS_THRESHOLD || analogRead(CURRENT_SEN_2) > CS_THRESHOLD) { //verifica se a corrente fornecida para os motores
    Stop();                                                                                  //está acima da estabelecida (8A). Caso positivo, para.
    delay(5000);
  }
}

int joyToPWM(int axisX) {
  int pwm = map(axisX, 0, 1023, -255, 255);   //mapeia as coordenadas do eixo x para um valor de velocidade entre 0 e 255 para o motor

  if(pwm >= 0)
    return pwm;                                 
  else
    return -pwm;
} 

int joyToDirection(int axisX) {     //determina o sentido de rotação a partir do valor retornado pelo joysyick
  int direct = map(axisX, 0, 1023, -255, 255); 

  if(direct > 0)
    return CW;                                 
  else
    return CCW;
}

void Stop() { //para os motores
  Serial.println("Stop");
  motorGo(MOTOR_1, BRAKE, 0);
  motorGo(MOTOR_2, BRAKE, 0);
}

void motorGo(uint8_t motor, uint8_t direct, uint8_t pwm) //Função para mover o motor: motor(0 ou 1), direction (cw ou ccw) e pwm (entre 0 e 255);
{
  if(motor == MOTOR_1)
  {
    if(direct == CW)
    {
      digitalWrite(MOTOR_A1_PIN, LOW); 
      digitalWrite(MOTOR_B1_PIN, HIGH);
    }
    else if(direct == CCW)
    {
      digitalWrite(MOTOR_A1_PIN, HIGH);
      digitalWrite(MOTOR_B1_PIN, LOW);      
    }
    else
    {
      digitalWrite(MOTOR_A1_PIN, LOW);
      digitalWrite(MOTOR_B1_PIN, LOW);            
    }
    
    analogWrite(PWM_MOTOR_1, pwm); 
  }
  else if(motor == MOTOR_2)
  {
    if(direct == CCW)
    {
      digitalWrite(MOTOR_A2_PIN, LOW);
      digitalWrite(MOTOR_B2_PIN, HIGH);
    }
    else if(direct == CW)
    {
      digitalWrite(MOTOR_A2_PIN, HIGH);
      digitalWrite(MOTOR_B2_PIN, LOW);      
    }
    else
    {
      digitalWrite(MOTOR_A2_PIN, LOW);
      digitalWrite(MOTOR_B2_PIN, LOW);            
    }
    
    analogWrite(PWM_MOTOR_2, pwm);
  }
}


