#include<Arduino.h>
// Declaración de pines para los LEDs
const int LED1 = 14;  // Cambia estos pines según tu configuración
const int LED2 = 13;
const int LED3 = 12;

// Pin para el canal ADC (Potenciómetro)
const int ADC_PIN = 34;  // Pin de lectura del ADC

// Variables para UART
String inputString = "";
bool inputComplete = false;

//declaración de funciones 
void setup();
void loop();
void displayMenu();
void handleInput(String command);
void readADC();
void controlLEDs();
void Encender_LED(int led, int timeMs);
void serialEvent();

void setup() {
  // Configuración de los pines de los LEDs como salida
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  // Inicializar UART (Serial)
  Serial.begin(9600);
  inputString.reserve(200);
  
  // Mostrar el menú inicial
  displayMenu();
}

void loop() {
  // Verificar si se ha recibido un comando completo
  if (inputComplete) {
    handleInput(inputString);
    inputString = "";
    inputComplete = false;
  }
}

// Función para desplegar el menú principal
void displayMenu() {
  Serial.println("----- MENU -----");
  Serial.println("1. Lectura ADC");
  Serial.println("2. Controlar LEDs");
  Serial.print("Ingrese una opción: ");
}

// Función que maneja la entrada del usuario
void handleInput(String command) {
  int option = command.toInt();

  switch (option) {
    case 1:
      // Opción para leer el ADC
      readADC();
      break;
    case 2:
      // Opción para controlar LEDs
      controlLEDs();
      break;
    default:
      Serial.println("Opción inválida. Intente nuevamente.");
      displayMenu();
      break;
  }
}

// Función para leer el valor ADC y mostrarlo en la terminal
void readADC() {
  int adcValue = analogRead(ADC_PIN);
  float voltage = adcValue * (3.3 / 4095.0);  // Conversión a voltaje
  Serial.print("Voltaje: ");
  Serial.print(voltage, 2);  // Mostrar voltaje con dos decimales
  Serial.println("V");
  displayMenu();
}

// Función para controlar los LEDs
void controlLEDs() {
  Serial.println("Ingrese el LED (1, 2 o 3) y el tiempo en ms (Ej: 1,500):");
  
  // Esperar a que el usuario ingrese el comando
  while (!Serial.available());
  
  String ledCommand = Serial.readStringUntil('\n');
  ledCommand.trim();  // Eliminar posibles espacios y saltos de línea

  // Separar los valores ingresados (LED y tiempo)
  int commaIndex = ledCommand.indexOf(',');
  
  if (commaIndex != -1) {
    int ledNum = ledCommand.substring(0, commaIndex).toInt();
    int timeMs = ledCommand.substring(commaIndex + 1).toInt();

    // Verificar si los valores son válidos
    if ((ledNum >= 1 && ledNum <= 3) && timeMs > 0) {
      Encender_LED(ledNum, timeMs);
    } else {
      Serial.println("Comando inválido. Intente nuevamente.");
    }
  } else {
    Serial.println("Comando inválido. Intente nuevamente.");
  }

  displayMenu();
}

// Función para encender el LED indicado por cierto tiempo
void Encender_LED(int led, int timeMs) {
  int ledPin;
  
  // Seleccionar el LED correcto
  switch (led) {
    case 1:
      ledPin = LED1;
      break;
    case 2:
      ledPin = LED2;
      break;
    case 3:
      ledPin = LED3;
      break;
    default:
      Serial.println("LED inválido.");
      return;
  }

  // Encender el LED
  digitalWrite(ledPin, HIGH);
  delay(timeMs);  // Mantenerlo encendido por el tiempo indicado
  digitalWrite(ledPin, LOW);  // Apagar el LED
  Serial.print("LED");
  Serial.print(led);
  Serial.println(" apagado.");
}

// Función que captura la entrada de texto del usuario
void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    
    if (inChar == '\n') {
      inputComplete = true;
    }
  }
}
