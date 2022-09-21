#define PIN_LED 7
unsigned int toggle;
void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_LED, OUTPUT);
  Serial.begin(115200);
  while(!Serial){
    ;
  }
  Serial.println("Hello World!");
  toggle = 0;
  digitalWrite(PIN_LED, toggle);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  for(int i=1; i<=10; i++){
    toggle = i % 2;
    digitalWrite(PIN_LED, toggle);
    delay(100);
  }
  toggle = 1;
  digitalWrite(PIN_LED, toggle);
  while(1){
    ;
  }
}