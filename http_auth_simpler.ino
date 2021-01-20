/*
 Web Server
 with
 http auth mod by Minh Truong
 2-20-215
 */

#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 177);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);
//char userPass[200];
//char header[500];
String header;
//int bufferSize = 0;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        header += c;
  
        if (c == '\n' && currentLineIsBlank) {

          //parse headers
          //bWluaDp0ZXN0 = 'minh:test' (user:password) base64 encode
    
          Serial.print(header);
          
          // Simpler just to find the credential string
          // send a standard http response header
          if(header.indexOf("bWluaDp0ZXN0") >= 0) {
            //successful login
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");  // the connection will be closed after completion of the response
            //client.println("Refresh: 5");  // refresh the page automatically every 5 sec
            client.println();
              if(header.indexOf("GET / HTTP/1.1") >= 0) {
                client.println("<!DOCTYPE HTML>");
                client.println("<html>");
                client.println("index");
                client.println("</html>");
              } else {
                client.println("<!DOCTYPE HTML>");
                client.println("<html>");
                client.println("hello world!");
                client.println("</html>");
              }
            
          } else {
            
            // wrong user/pass
            //client.println("HTTP/1.0 401 Authorization Required");
            client.println("HTTP/1.1 401 Unauthorized");
            client.println("WWW-Authenticate: Basic realm=\"Secure\"");
            client.println("Content-Type: text/html");
            client.println();
            client.println("<html>Text to send if user hits Cancel button</html>"); // really need this for the popup!

          }
          
          header = "";
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}


