const int FSR_PIN1 = A0; // Pin connected to FSR/resistor divider
const int FSR_PIN2 = A1;
const int FSR_PIN3 = A2;
const int LED_PIN1 = 13;
int fsrADC1, fsrADC2, fsrADC3;
float fsrV1, fsrV2, fsrV3;
float fsrR1, fsrR2, fsrR3;
float fsrG1, fsrG2, fsrG3;
float force1, force2, force3;
const float VCC = 4.99; // Measured voltage of Ardunio 5V line
const float R_DIV = 5020.0; // Measured resistance of 5k resistor

void setup() 
{
  Serial.begin(9600);
  pinMode(FSR_PIN1, INPUT); 
  pinMode(FSR_PIN2, INPUT); 
  pinMode(FSR_PIN3, INPUT); 
  pinMode(LED_PIN1, OUTPUT); 
}

void loop() 
{
  fsrADC1 = analogRead(FSR_PIN1);
  fsrADC2 = analogRead(FSR_PIN2);
  fsrADC3 = analogRead(FSR_PIN3);
  // If the FSR has no pressure, the resistance will be
  // near infinite. So the voltage should be near 0.
  
  if (fsrADC1 > 15 and fsrADC2 > 30  and fsrADC3 > 30) // If the analog reading is non-zero
  {
    // Use ADC reading to calculate voltage:
    fsrV1 = fsrADC1 * VCC / 1023.0;
    // Use voltage and static resistor value to 
    // calculate FSR resistance:
    fsrR1 = R_DIV * (VCC / fsrV1 - 1.0);
    Serial.println("Resistance: " + String(fsrR1) + " ohms");
    // Guesstimate force based on slopes in
    // FSR datasheet:
    fsrG1 = 1.0 / fsrR1; // Calculate conductance
    
    fsrV2 = fsrADC2 * VCC / 1023.0;
    // Use voltage and static resistor value to 
    // calculate FSR resistance:
    fsrR2 = R_DIV * (VCC / fsrV2 - 1.0);
    Serial.println("Resistance: " + String(fsrR2) + " ohms");   
    // Guesstimate force based on slopes in
    // FSR datasheet:
    fsrG2 = 1.0 / fsrR2; // Calculate conductance
    
    fsrV3 = fsrADC3 * VCC / 1023.0;
    // Use voltage and static resistor value to 
    // calculate FSR resistance:
    fsrR3 = R_DIV * (VCC / fsrV3 - 1.0);
    Serial.println("Resistance: " + String(fsrR3) + " ohms");
    // Guesstimate force based on slopes in figure 3 of
    // FSR datasheet:
    fsrG3 = 1.0 / fsrR3; // Calculate conductance
    
    // Break parabolic curve down into two linear slopes:
    if (fsrR1 <= 1000 and fsrR2 <= 1000 and fsrR3 <= 1000){ 
      force1 = (fsrG1 - 0.00075) / 0.00000032639;
      force2 = (fsrG2 - 0.00075) / 0.00000032639;
      force3 = (fsrG3 - 0.00075) / 0.00000032639;
    }
    else{
      force1 =  fsrG1 / 0.000000642857;
      force2 =  fsrG2 / 0.000000642857;
      force3 =  fsrG3 / 0.000000642857;
    }
    Serial.println("Force1: " + String(force1) + "g");
    Serial.println("Force2: " + String(force2) + "g");
    Serial.println("Force3: " + String(force3) + "g\n");
    if(force1 > 100 and force2 > 100 and force3 > 100)
    {
      if((force1 < 100 and (force2 > 100 and force3 > 100)) or (force2 < 100 and (force1 > 100 and force3 > 100)) or (force3 < 100 and (force2 > 100 and force1 > 100)))
      {
      digitalWrite(LED_PIN1,LOW);
      }
    digitalWrite(LED_PIN1,HIGH);
    }
    delay(250);
  }
    else{
      // No pressure detected
      digitalWrite(LED_PIN1,LOW);
    }
  }

 
