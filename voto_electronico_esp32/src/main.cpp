#include <Arduino.h>
/*
   ESP8266 o ESP32 (ver las modificaciones)
   Este script sube al server un valor "aleatorio" en el topic "datos_voto_electronico" 

*/

// Para la conexion a wifi y el mqtt
#include <WiFi.h> 
WiFiClient WIFI_CLIENT;
#include <PubSubClient.h>
PubSubClient MQTT_CLIENT;

#define SSID "UA-Alumnos"
#define PASSWORD "41umn05WLC"
#define TOPIC "datos_voto_electronico"
#define TOPIC_vAleatorio "datos_voto_electronico/aleatorio"

// Para el display 
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // Ancho del display OLED en píxeles
#define SCREEN_HEIGHT 64 // Alto del display OLED en píxeles
// Declaración del display conectado por I2C (pines SDA, SCL)
// El -1 indica que el display no tiene un pin de reset físico
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


// Definicion de las funciones -------------------------------------------------------------------------------------------------------
void reconnect();
void setup();
void loop();
void callback(char*, byte*, unsigned int);
void publishRandomInt();
void displaySetup();
// -----------------------------------------------------------------------------------------------------------------------------------


void setup() {
  Serial.begin(115200);
  
  delay(10);

  Serial.println();
  Serial.print("Conectando con ");
  Serial.println(SSID);

  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi conectado.");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  Serial.println("");

  displaySetup();
}

void loop() {
  // Continuamente comprueba si tiene conexión con MQTT,
  // en caso de que no tenga, reconecta.
  if (!MQTT_CLIENT.connected()) {
    reconnect();
  }

  publishRandomInt(); // publica un valor al azar
  
  MQTT_CLIENT.loop(); // El motor del cliente, llamamos para que se actualice y ejecute lo necesario (callbacks)
  
  // Espera antes de Publicar otro aleatorio.
  delay(5000);
}

void publishRandomInt() {
  
  // Publicar un mensaje. Publish.
  // Convierte el entero a char. DEBE ser char.
  int aleatorio = random(1,90);
  Serial.println(aleatorio);
  String aleatorioString = String(aleatorio);
  char alea[6];
  aleatorioString.toCharArray(alea, 6);
  MQTT_CLIENT.publish(TOPIC_vAleatorio, alea);
  
}
// Reconecta con MQTT broker
void reconnect() {
  // MQTT_CLIENT.setServer("192.168.1.206", 1883); // si uso un servidor local <ver IP correcta>
  IPAddress ip(44,196,23,48);
  MQTT_CLIENT.setServer(ip, 1883);  // servidor gratuito
  MQTT_CLIENT.setClient(WIFI_CLIENT);
  MQTT_CLIENT.setCallback(callback);  // Agregamos el callback para responder a la infomracion subida

  // Intentando conectar con el broker.
  while (!MQTT_CLIENT.connected()) {
    
    Serial.println("Intentando conectar con MQTT.");
    MQTT_CLIENT.connect(TOPIC); // Escribe cualquier nombre.
    MQTT_CLIENT.subscribe(TOPIC); // nos suscribimos al mismo topico para escuchar lo publicado
    
    // Espera antes de volver a intentarlo.
    delay(3000);
  }

  Serial.println("Conectado a MQTT.");
}

// Una callback tiene que tener el topic, el mensaje en bytes (payload) y el tamaño deseado del mensaje
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println("");
  Serial.print("Mensaje recibido en el topic: ");
  Serial.println(topic);
  
  std::string message;
  for (int i = 0; i < length ; i++) {
    message += (char)payload[i];
  }
  
  Serial.print("Contenido:\t");
  Serial.println(message.c_str());

}

// 
void displaySetup() {
  Serial.begin(115200);

  // Inicializar el display con la dirección I2C 0x3C (la más común)
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("Fallo al inicializar la pantalla SSD1306"));
    for(;;); // Bucle infinito si falla
  }

  // Limpiar el buffer de la pantalla (borrar cualquier basura previa)
  display.clearDisplay();

  // Configurar el tamaño y color del texto
  display.setTextSize(1);             // Escala 1:1
  display.setTextColor(SSD1306_WHITE); // Texto blanco (se ilumina el píxel)
  display.setCursor(0, 10);           // Empezar a escribir en la coordenada X:0, Y:10

  // Escribir nuestro mensaje de prueba
  display.println(F("   Pantalla OK!"));
  display.println(F(""));
  display.println(F(" Lista para arrancar"));
  display.println(F("  con el proyecto de"));
  display.println(F("    Laboratorio I"));

  // Enviar el buffer a la pantalla para que se dibuje
  display.display();
}