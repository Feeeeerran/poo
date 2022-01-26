char a;  
void setup() {
  Serial.begin(9600);
  pinMode(13,1);
  digitalWrite(13,0);
}

void loop() {
  if(Serial.available()){
    a=Serial.read();
    if(a){                      
      digitalWrite(13,1);
      Serial.println("Operacion realizada con exito");
    }
    else {
      digitalWrite(13,0);
      Serial.println("No se pudo ejecutar la orden");
    }
  }
}
