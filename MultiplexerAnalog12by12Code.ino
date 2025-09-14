const uint8_t N_ROWS = 12;
const uint8_t N_COLS = 12;

const uint8_t S0 = 8;
const uint8_t S1 = 9;
const uint8_t S2 = 10;
const uint8_t S3 = 11;

const uint8_t ADC_PIN = A0;

const uint8_t selBits[16][4] = {
  {0,0,0,0}, {1,0,0,0}, {0,1,0,0}, {1,1,0,0},
  {0,0,1,0}, {1,0,1,0}, {0,1,1,0}, {1,1,1,0},
  {0,0,0,1}, {1,0,0,1}, {0,1,0,1}, {1,1,0,1},
  {0,0,1,1}, {1,0,1,1}, {0,1,1,1}, {1,1,1,1}
};

inline void setMux(uint8_t channel) {
  digitalWrite(S0, selBits[channel][0]);
  digitalWrite(S1, selBits[channel][1]);
  digitalWrite(S2, selBits[channel][2]);
  digitalWrite(S3, selBits[channel][3]);
}

const uint16_t SETTLE_ROW_US = 150;
const uint16_t SETTLE_COL_US = 80;

const uint8_t ADC_SAMPLES = 1;

void setup() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);

  analogReference(DEFAULT);     
  analogRead(ADC_PIN);          

  Serial.begin(115200);
}

void loop() {
  for (uint8_t r = 0; r < N_ROWS; r++) {
    setMux(r);
    delayMicroseconds(SETTLE_ROW_US);

    for (uint8_t c = 0; c < N_COLS; c++) {
      setMux(c);
      delayMicroseconds(SETTLE_COL_US);

      uint16_t acc = 0;
      for (uint8_t i = 0; i < ADC_SAMPLES; i++) {
        acc += analogRead(ADC_PIN);
      }
      uint16_t value = acc / ADC_SAMPLES;

      Serial.print(value);
      if (!(r == N_ROWS - 1 && c == N_COLS - 1)) Serial.print(',');
    }
  }
  Serial.println();
}
