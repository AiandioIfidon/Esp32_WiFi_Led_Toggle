#include <WiFi.h>

const char* ssid = "An_D";
const char* password = "2764fundy";

String header;
String output2State = "off";

const uint8_t output2 = 3;

unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 2000;


WiFiServer server(80);
     

void setup () 
{
  Serial.begin(115200);
  pinMode(output2, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(3000);
    Serial.println("Connecting to ");
    Serial.println(ssid);
  }

  Serial.println("Successfully connected to ");
  Serial.println(ssid);
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}


void loop() 
{
   WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;

    if ( c == '\n')
    {
      
      if (currentLine.length() == 0)
      {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-type:text/html");
      client.println("Connection: close");
      client.println();

      if(header.indexOf("GET /3/on") >= 0)
        {
          Serial.println("GPIO 3 on");
          output2State = "on";
          digitalWrite(output2, HIGH);
        } else if(header.indexOf("GET /3/off") >= 0)
        {
          Serial.println("GPIO 3 off");
          output2State = "off";
          digitalWrite(output2, LOW);
        }

    client.print("<!DOCTYPE html>");
    client.print("<html lang=\"en\">");
    client.print("<head>");
    client.print("<meta charset='UTF-8'>");
    client.print("<meta name='viewport' content='width=device-width, initial-scale=1.0'>");
    client.print("<title>ESP32</title>");
    client.print("<style>");
    client.print("body {");
    client.print("font-family: Arial, sans-serif;");
    client.print("background-color: #ecf0f1;");
    client.print("margin: 0;");
    client.print("padding: 0;");
    client.print("}");

    client.print(".navbar {");
    client.print("background-color: #2c3e50;");
    client.print("padding: 10px;");
    client.print("display: flex;");
    client.print("justify-content: space-around;"); 
    client.print("align-items: center;");
    client.print("}");

    client.print(".navbar a {");
    client.print("color: #fff;");
    client.print("text-decoration: none;");
    client.print("margin: 0 15px;");
    client.print("font-size: 1.2em;");
    client.print("}");

    client.print(".container {");
    client.print("text-align: center;");
    client.print("padding: 50px;");
    client.print("color: #34495e;");
    client.print("}");

    client.print("h1 {");
    client.print("font-size: 2em;");
    client.print("}");

    client.print("p {");
    client.print("font-size: 1.2em;");
    client.print("line-height: 1.6;");
    client.print("}");

    client.print("</style>");
    client.print("</head>");
    client.print("<body>");
    client.print("<div class='navbar'>");
    client.print("<a href='#'>Home</a>");
    client.print("<a href='#'>About</a>");
    client.print("<a href='#'>Contact Us</a>");
    client.print("<a href='#'>Menu</a>");
    client.print("</div>");
    client.print("<div class='container'>");
    client.print("<h1>ESP32</h1>");
    client.print("<p>The ESP32 is a powerful microcontroller developed by Espressif Systems. It integrates Wi-Fi and Bluetooth connectivity, making it ideal for IoT (Internet of Things) applications.");
    client.print(" With its dual-core processor and various built-in peripherals, the ESP32 offers versatility and performance for a wide range of projects, including home automation, wearables, and industrial automation. Its low power consumption and support for multiple communication protocols make it a popular choice among developers. Furthermore, its affordability and ease of use have contributed to its widespread adoption in the maker community.</p>");
    client.print("</div>");

        client.println("<p> GPIO 3 state - " + output2State + "</p");

        if( output2State == "off")
        {
          client.println("<p> <a href=\"/3/on\"> <button>ON</button> </p>");
        } else {
          client.println("<p> <a href=\"/3/off\"> <button>OFF</button> </p>");
        }

        client.println("</body></html>");
        client.println();
        break;
      } else {
        currentLine = "";
      }
    } else if (c != '\r')
    {
      currentLine += c;
    }
   }
  }
  header = "";
  client.stop();
  Serial.println("Client disconnected");
  Serial.println("");
  }
}
