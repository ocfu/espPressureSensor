//
// espPressureSensor
//
// Analogue preasure sensor measurement with a NodeMCU
//
// OC
//

const int sensorPin = A0;    // pin that the sensor is attached to

// Analog pressure sensor properties. Vcc = 5V
//  
//  Vsensor
//    ^
//   5|
//    |                        X (5bar/4.5V)
//   4|                     X 
//    |                  X   
//   3|               X
//    |            X 
//   2|         X
//    |      X 
//   1|   X 
//    X (0bar/0.5V)
//    +---------------------------> P(bar)
//    0    1    2    3    4    5
// 
float Vzero = 0.5;     // sensor output voltage at zero pressure 
float Vmax = 4.5;      // sensor output voltage at max. pressure
float Pmax = 5.0;      // sensor max. pressure [bar]


// Voltage divider for the analog input A0 (range = 0..1V)
// The internal NodeNCU divider is: R1 = 220k, R2 = 100k for the range 0..3.3V. 
// For the range 0..5V we need R1 = 400k, this can be done with one 180k in series (Rs) to the 
// A0 input. If you have no 180k, take Rs = 100k. The consequence is, that the 
// max. measured input it capped at 4.6 bar, so you can't use the full pressure range. 
//                     
//               ___     A0 (NodeMCU)
// Vsensor o----|___|---o-----.
//               Rs           +
//                           | |R1 (internal)
//                           | |
//                            +
//                            o------o Vin DAC 
//                            +
//                           | |R2 (internal)
//                           | |
//                            +
//                            |
//                           ---
float Rs = 100.0;
float R1 = 220.0;  
float R2 = 100.0;
float fRDivider = R2/(Rs+R1+R2);

// Calculate the digital value for zero pressure (offset) and max. pressure (fullscale).
int offset = int(Vzero * fRDivider * 1023);
int fullscale = int(Vmax * fRDivider * 1023);

float pressure; // measured pressure

void setup() {
  Serial.begin(115200);
}

void loop() {
  int sensorVal = analogRead(A0);

  float bar = Pmax * (sensorVal - offset) / (fullscale - offset);

  // alternatively you can get rid of all variables, if your setup-up is fix
  //
  // fRDivider = 100 / 420 = 0.238
  // offset = 0.5 * 0.238 * 1023 = 122
  // fullscale = 4.5 * 0.238 * 1023 = 1096
  // Pmax / (fullscale - offset) = 5 / (1096 - 122) = 0.0051335
  //
  // float bar = (sensorVal - 122) * 0.0051335;
  
  if (bar < 0.0) 
    bar = 0.0;
    
  Serial.print("Bar: ");
  Serial.println(bar);

  delay(100);
}
