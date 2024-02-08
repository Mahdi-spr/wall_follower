#define IN1 2
#define IN2 3
#define IN3 4
#define IN4 5
#define pin_e1 9 
#define pin_e2 6
#define vol 13
#define pwm_e1 60
#define pwm_e2 60
#define turn_delay 310
#define rthold 280 //at most range for wall
#define lthold 280
#define fthold 120
#define lit_turn_ltr 70
#define lit_turn_rtr 55
#define lit_turn_rtl 70
#define lit_turn_ltl 55
#define turn_toright 200 //forward for right turn
#define ad_del 250
#define stop_after_turn 1000
#define f_time 100
///Sensors
const int trigPinf=10;
const int echoPinf=11;
const int trigPinr=7;
const int echoPinr=8;
const int trigPinl=12;
const int echoPinl=13;

float fdist,fdur,rdist,rdur,ldist,ldur;
void forward(int d)
{
    analogWrite(pin_e1,pwm_e1); // analogRead values go from 0 to 255, 
    analogWrite(pin_e2,pwm_e2);
    digitalWrite(IN1,HIGH);
    digitalWrite(IN2,LOW);
    digitalWrite(IN3,HIGH);
    digitalWrite(IN4,LOW); 
    if(d!=0){
      delay(d);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    }
    
}
void right(int d)
{
  analogWrite(pin_e1,pwm_e1); // analogRead values go from 0 to 255, 
  analogWrite(pin_e2,pwm_e2);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  if(d==0){ // turn right to adjust to right wall
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    delay(lit_turn_rtr);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    return ;
  }
  if(d==1){ // turn right to adjust to left wall
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    delay(lit_turn_rtl);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    return ;
  }
  delay(turn_delay);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void left(int d)
{
  analogWrite(pin_e1,pwm_e1); // analogRead values go from 0 to 255, 
  analogWrite(pin_e2,pwm_e2);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  if(d==0){ //turn left to adjust right wall
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    delay(lit_turn_ltr);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    return ;
  }
  if(d==1){ //turn left to adjust right wall
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    delay(lit_turn_ltl);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    return ;
  }
  delay(turn_delay);
  digitalWrite(IN1, LOW);
  digitalWrite(IN4, LOW);
}

void stopb()
{
  analogWrite(pin_e1,0); // analogRead values go from 0 to 255, 
  analogWrite(pin_e2,0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}



float sen_dist(int trigPin, int echoPin){
  for(int i=0;i<4;i++){
    digitalWrite(trigPin,HIGH);
    delayMicroseconds(50);
    digitalWrite(trigPin,LOW);
    float dur = pulseIn(echoPin,HIGH);
    float dist=dur*0.344/2;
    return dist;

  }

}

void setup() 
{  
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(pin_e1, OUTPUT);
  pinMode(pin_e2, OUTPUT);
  pinMode(trigPinf,OUTPUT);
  pinMode(echoPinf,INPUT);
  pinMode(trigPinl,OUTPUT);
  pinMode(echoPinl,INPUT);
  pinMode(trigPinr,OUTPUT);
  pinMode(echoPinr,INPUT);
  Serial.begin(9600);
  delay(2000);
  

}

void loop()
{
    fdist=sen_dist(trigPinf,echoPinf);
    rdist=sen_dist(trigPinr,echoPinr);
    ldist=sen_dist(trigPinl,echoPinl);
    if(fdist>=fthold && rdist<=rthold){ //if right wall is available and front is open gooooo
        Serial.println(fdist);
        Serial.println(rdist);
        if(rdist<59){
          left(0);
          stopb();
          right(0);
          stopb();
          Serial.println("Little left to adjust"); 
          delay(ad_del);
        }else if(rdist<=30){
          stopb();
        }
        if(ldist<69){
          right(1);
          stopb();
          left(1);
          stopb();
          Serial.println("Little right to adjust"); 
          delay(ad_del);
        }else if(ldist<=30){
          stopb();
        }
        forward(f_time); // moves forward if right wall within range and front wall out of range
    } else if(rdist>=rthold && fdist>=fthold){ // if rightTurn is available and front is open goo staright and then turn right
        Serial.println(fdist);
        Serial.println(rdist);
        Serial.println("Little front and then right");
        forward(turn_toright);
        right(3); 
        stopb();   
        delay(stop_after_turn);                     
    }else if(rdist<=rthold && fdist<=fthold && ldist<=lthold){ // if it's a deadend so turn right
        Serial.println(fdist);
        Serial.println(rdist);
        Serial.println(" right");
        right(3);    
        stopb();
        delay(stop_after_turn);
    }else if(ldist>=lthold && fdist<=fthold && rdist<=rthold){ // if rightTurn isn't available and front is closed turn left 
        Serial.println(fdist);
        Serial.println(rdist);
        Serial.println("Left");
        left(3);  
        stopb();  
        delay(stop_after_turn);
    }else if(fdist<=fthold){
      stopb();
    }  
} 