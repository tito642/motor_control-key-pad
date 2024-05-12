#include <LiquidCrystal.h>
#define r1 0
#define r2 1
#define r3 2
#define r4 3
#define c1 4
#define c2 5
#define c3 6
#define c4 7

//Define a 4x4 keypad 
const byte ROWS = 4; 
const byte COLS = 4; 
// Define the keys on the keypad
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// Initialize LCD objects
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);


float result = 0;        // Final result of the calculations
float tempNum = 0;       // Temporary number storage for input
char operation = '\0';   // Operation (+, -, *, /) being performed
bool inputInProgress = false; // Flag to track input state


const int motorPin1 = A0; // Motor pin 1
const int motorPin2 = A1; // Motor pin 2
const int enablePin = A2; // Motor enable pin

int motorSpeed = 0;
int timeInSeconds = 0;
void setup() {
  // Sets the row pins (r1-r4) as OUTPUT and initializes them to a HIGH state.
  pinMode(r1, OUTPUT);
  pinMode(r2, OUTPUT);
  pinMode(r3, OUTPUT);
  pinMode(r4, OUTPUT);

  digitalWrite(r1, HIGH);
  digitalWrite(r2, HIGH);
  digitalWrite(r3, HIGH);
  digitalWrite(r4, HIGH);

  // Sets the column pins (c1-c4) as INPUT_PULLUP, enabling internal pull-up resistors.
  pinMode(c1, INPUT_PULLUP);
  pinMode(c2, INPUT_PULLUP);
  pinMode(c3, INPUT_PULLUP);
  pinMode(c4, INPUT_PULLUP);
  
  // Initializes the LCD with 16 columns and 2 rows, then displays the initial message on the LCD.
  lcd.begin(16, 2);     
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(enablePin, OUTPUT);
  lcd.print("Motor Control:");
  lcd.setCursor(0, 1);
  lcd.print("Speed: ");
  lcd.setCursor(0, 2);
  lcd.print("Time: ");

}

// Function to check if a key is pressed
char ckeckInput(void) {
  int Read;
  int flag = 0;

  for (int i = 0; i < ROWS; i++) {
    digitalWrite(i, LOW);

    for (int j = 0; j < COLS; j++) {
      Read = digitalRead(j + 4);

      // Wait until the key is released
      while (Read == LOW) {
        Read = digitalRead(j + 4);
        flag = 1;
      }

      if (flag == 1) {
        // Release the row pin
        digitalWrite(i, HIGH);
        return keys[i][j]; // Return the pressed key
      }
    }
    // Release the row pin
    digitalWrite(i, HIGH);
  }
  return 0; // Return 0 if no key is pressed
}

// Continuously checks for a pressed key using ckeckInput() and processes it using handleInput() if a valid key is detected.
void loop() {
  char key = ckeckInput();
 if (key != '\0') {
    switch(key) {
      case '0'...'9':
        // Update the speed value
        motorSpeed = motorSpeed * 10 + (key - '0');
        if (motorSpeed > 3000) {
          motorSpeed = 3000;
        }
        break;
      case 'A': // Clear the speed value
        motorSpeed = 0;
        break;
      case 'B': // Increase time by 10 seconds
        timeInSeconds += 10;
        if (timeInSeconds > 300) {
          timeInSeconds = 300;
        }
        break;
      case 'C': // Decrease time by 10 seconds
        timeInSeconds -= 10;
        if (timeInSeconds < 0) {
          timeInSeconds = 0;
        }
        break;
      case 'D': // Start the motor
        startMotor();
        break;
    }
    updateLCD();
  }
  
}

void startMotor() {
  analogWrite(enablePin, motorSpeed);
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  delay(timeInSeconds * 1000); // Convert seconds to milliseconds
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
}

void updateLCD() {
  lcd.setCursor(7, 1);
  lcd.print("       "); // Clear previous speed value
  lcd.setCursor(7, 1);
  lcd.print(motorSpeed);
  lcd.setCursor(6, 2);
  lcd.print("      "); // Clear previous time value
  lcd.setCursor(6, 2);
  lcd.print(timeInSeconds);
}


// Function to perform the calculation based on the stored operator

void resetCalculator() {
  // Reset final result , temporary number , operator and input state flag
  result = 0;            
  tempNum = 0;           
  operation = '\0';     
  inputInProgress = false; 
}
