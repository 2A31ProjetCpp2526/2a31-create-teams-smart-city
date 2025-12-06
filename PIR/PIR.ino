int pir_pin = 2;  // Pin du PIR
int bin_id = 1; // ID de poubelle associée (configurable)
int pirState = LOW;        // variable to hold the PIR state
int lastPirState = LOW;    // keep track of previous PIR state

void setup() {
  pinMode(pir_pin, INPUT);
  Serial.begin(9600);
  Serial.println("PIR ready...");
}

void loop() {
  pirState = digitalRead(pir_pin);
  if (pirState == HIGH && lastPirState == LOW) {
    // motion detected (rising edge)
    Serial.print("MOTION;BIN:");
    Serial.println(bin_id);
  }
  lastPirState = pirState;
  delay(200); // simple debounce / sampling
}
