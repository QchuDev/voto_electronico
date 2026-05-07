#include "component/Scanner.h"

// SCANNER PINS - GPIO16, GPIO17
#define RX 16
#define TX 17

HardwareSerial HScanner(2); // Usar UART2

std::vector<std::string> Scanner::getData() {
    // Movemos el contenido a temp. buffer queda vacío automáticamente.
    std::vector<std::string> temp = std::move(buffer);
    
    return temp;
}

void Scanner::setup() {
    HScanner.begin(9600, SERIAL_8N1, RX, TX);
    Serial.println("Scanner listo...");
    
}



void Scanner::update() {
    if (HScanner.available()) {
        String data = HScanner.readStringUntil('\r'); // El GM865 envía un retorno de carro al final
        
        buffer.push_back(data.c_str());
        
        Serial.print("Lectura: ");
        Serial.println(data);
    }
}


