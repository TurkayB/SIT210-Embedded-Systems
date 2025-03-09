String morseCode[] = {
  ".-",    // A
  "-...",  // B
  "-.-.",  // C
  "-..",   // D
  ".",     // E
  "..-.",  // F
  "--.",   // G
  "....",  // H
  "..",    // I
  ".---",  // J
  "-.-",   // K
  ".-..",  // L
  "--",    // M
  "-.",    // N
  "---",   // O
  ".--.",  // P
  "--.-",  // Q
  ".-.",   // R
  "...",   // S
  "-",     // T
  "..-",   // U
  "...-",  // V
  ".--",   // W
  "-..-",  // X
  "-.--",  // Y
  "--.."   // Z
};

void dot() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(300);
  digitalWrite(LED_BUILTIN, LOW);
  delay(300);
}

void dash() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(800);
  digitalWrite(LED_BUILTIN, LOW);
  delay(300);
}

void blinkLetter(char letter) {
  letter = toupper(letter);
  if (letter == ' ') {
    delay(1500);
    return;
  }

  int index = letter - 'A';
  if (index >= 0 && index <= 25) {
    String symbols = morseCode[index];
    for (int i = 0; i < symbols.length(); i++) {
      if (symbols[i] == '.') {
        dot();
      }
      else if (symbols[i] == '-') {
        dash();
      }
    }
  }
  delay(600);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    String firstName = Serial.readStringUntil('\n');
    firstName.trim();

    for (int i = 0; i < firstName.length(); i++) {
      blinkLetter(firstName[i]);
    }
    delay(1500);
  }
}