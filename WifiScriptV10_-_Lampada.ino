#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

//Nome da sua rede Wifi
const char* ssid = "EniacEventos";

//Senha da rede
const char* password = "";

//Static infos
IPAddress ip(192, 168, 43, 11); 
IPAddress gateway(192, 168, 43, 1);
IPAddress subnet(255, 255, 255, 0);

//Criando o servidor web na porta 80
ESP8266WebServer server(80);

/*
Equivalencia das saidas Digitais entre nodeMCU e ESP8266 (na IDE do Arduino)
NodeMCU � ESP8266
D0 = 16;
D1 = 5;
D2 = 4;
D3 = 0;
D4 = 2;
D5 = 14;
D6 = 12;
D7 = 13;
D8 = 15;
D9 = 3;
D10 = 1;
*/

//Pino do led do ESP8266
const int led = 2;
const int lamp = D6;

//Pagina inicial 
void handleRoot() {
  String buf;
  buf = "";
  buf += "<!DOCTYPE HTML>\r\n<html>\r\n";
  buf += "<head> ";
  buf += "<meta charset='UTF-8'> ";
  buf += "<title>Automa&ccedil;&atilde;o Residencial</title> ";
  buf += "</head> ";
  buf += "<body> ";
  buf += "server ok, you're on the home page";
  buf += "</body> ";
  buf += "</html> ";

  server.send(200, "text/html", buf);
}

void handleNow() {
  String response;
  int status_lamp = digitalRead(lamp);
  
  if (status_lamp == HIGH) {
    response = "on";
  } else {
    response = "off";
  }

  server.send(200, "text/html", response);
  
}

void handleLamp() {
  if( ! server.hasArg("status") || server.arg("status") == NULL) {
    
    server.send(303);
    
  } else if (server.arg("status") == "on") {
    digitalWrite(lamp, HIGH);
    digitalWrite(led, HIGH);
    delay(500);
    digitalWrite(led, LOW);
    server.send(303);
    
    } else if (server.arg("status") == "off"){
    digitalWrite(lamp, LOW);
    digitalWrite(led, HIGH);
    delay(1000);
    digitalWrite(led, LOW);
    server.send(303);
    
    } else {
    server.send(401, "text/plain", "401: Unauthorized");
    }
}

//Pagina caso não for encontrado
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

//Funcao que sera executada apenas ao ligar o ESP8266
void setup() {
  pinMode(led, OUTPUT);
  pinMode(lamp, OUTPUT);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", HTTP_GET, handleRoot);
  server.on("/lamp", HTTP_GET, handleLamp);
  server.on("/now", HTTP_GET, handleNow);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

//Funcao que sera executada indefinidamente enquanto o NodeMCU estiver ligado.
void loop() {
  server.handleClient();
}
