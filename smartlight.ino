#define CEL_FOTV A6 //Celula fotovoltaica
#define SENS_PRES 2 //Sensor de presencia
#define LAMP_1 7 //Actuador luminoso
#define LAMP_2 8 //Actuador luminoso
#define LAMP_3 9 //Actuador luminoso
#define LAMP_4 10 //Actuador luminoso
#define LED 6 //Piloto
#define TIME_LAMP 15000 //Duracion de lampara (10seg)
#define DARK 5 //Umbral para oscuridad

bool led_blink;//Active Parpadeo Piloto
bool state_led_blink;//Estado parpadeo Piloto

String STATE;//Etapa del sistema
//READY: Preparado para comenzar una deteccion
//DETECCION: Nueva presencia detectada
//LAST: Ultimos 3seg de lampara

int temp_lamp;//Temporizador de lampara
int temp_sleep;//Time sleep system

void setup() {

  pinMode(SENS_PRES, INPUT);
  pinMode(LAMP_1,OUTPUT);
  pinMode(LAMP_2,OUTPUT);
  pinMode(LAMP_3,OUTPUT);
  pinMode(LAMP_4,OUTPUT);
  pinMode(LED,OUTPUT);
  
  //Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(SENS_PRES), presencia, RISING);
  temp_lamp=0;
  temp_sleep=1000;//1seg
  led_blink=true;//Active Parpadeo Piloto
  state_led_blink=false;//Estado parpadeo Piloto
  STATE="READY";
}

void loop() {

  //Maquina de Estados

    if(STATE=="READY"){
          ctrlLuz(false);
          led_blink=true;
          temp_sleep=1000;//1seg
          temp_lamp=0;//Temporizado lampara
    }
    
    else if(STATE=="DETECCION"){
          ctrlLuz(true);
          led_blink=false;
          if(temp_lamp>=(TIME_LAMP-5000))
          {
            STATE="LAST";
          }
          temp_sleep=1000;//1seg          
          temp_lamp+=temp_sleep;//Temporizado lampara
    }         

    else if(STATE=="LAST"){
          ctrlLuz(true);
          led_blink=true;
          if(temp_lamp>TIME_LAMP)
          {
              ctrlLuz(false);
              STATE="READY";  
          }          
          temp_sleep=250;//0.25seg
          temp_lamp+=temp_sleep;//Temporizado lampara
     }
          
    else{
          STATE="READY";
          ctrlLuz(false);
    }
  
  //Parpadeo Piloto
  if(led_blink)
  {
    digitalWrite(LED,state_led_blink);
    state_led_blink=!state_led_blink;
  }
  //Sleep system
  delay(temp_sleep);
}
void presencia()
{
  //Condicion de encendido
  if(analogRead(CEL_FOTV)<DARK)
  {
    STATE="DETECCION";
    digitalWrite(LED,LOW);
    ctrlLuz(true);
    temp_lamp=0;
  } 
}

void ctrlLuz(bool state)
{
  digitalWrite(LAMP_1,state);
  digitalWrite(LAMP_2,state);
  digitalWrite(LAMP_3,state);
  digitalWrite(LAMP_4,state);
}
