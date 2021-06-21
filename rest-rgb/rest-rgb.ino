#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServerSecure.h>

/******************************************************************
 * SERVICE CONSTANTS
 *****************************************************************/
#define SERVICE_HOST_NAME "couch-rgb"
#define WIFI_CONNECTION_TIMEOUT 30000
#define WIFI_SSID "Pentagrama do Prazer"
#define WIFI_PASSWORD "melambe69"

BearSSL::ESP8266WebServerSecure server(443);

const String HTTP_DEFAULT_HEADERS[] = {
  "Access-Control-Allow-Methods", "GET,POST",
  "Access-Control-Allow-Origin", "*"
};

const char serverCert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIBzzCCAXmgAwIBAgIUJTCGQ5lzmcJIkJQn7D7JCz6mhOQwDQYJKoZIhvcNAQEL
BQAwPDEUMBIGA1UEBwwLZGV2ZWxvcG1lbnQxEjAQBgNVBAoMCWFsYmVydG93ZDEQ
MA4GA1UEAwwHKi5sb2NhbDAeFw0yMTA2MjAxNjM0MzlaFw0zMTA2MTgxNjM0Mzla
MDwxFDASBgNVBAcMC2RldmVsb3BtZW50MRIwEAYDVQQKDAlhbGJlcnRvd2QxEDAO
BgNVBAMMByoubG9jYWwwXDANBgkqhkiG9w0BAQEFAANLADBIAkEAxdzzFC8/mmfU
uUgV1La9ALZZZMq26U2zyN3HKng8gIPdR4cH6OA2WEYBseyeqZ9ig0zPCkV8b5AM
hkrz8e/e6wIDAQABo1MwUTAdBgNVHQ4EFgQUJRpGNJ+TbdvZNJHRV/cWIL/ZbiEw
HwYDVR0jBBgwFoAUJRpGNJ+TbdvZNJHRV/cWIL/ZbiEwDwYDVR0TAQH/BAUwAwEB
/zANBgkqhkiG9w0BAQsFAANBAGK+x9Jo1cMVVIWr+mHXfwgWB8SjdqCbc5dSv7Iv
OKCd25Wmi1WX+42iPN05fOPhHjleh06MwhY4RQa8VN8tslE=
-----END CERTIFICATE-----
)EOF";

const char serverKey[] PROGMEM =  R"EOF(
-----BEGIN PRIVATE KEY-----
MIIBVAIBADANBgkqhkiG9w0BAQEFAASCAT4wggE6AgEAAkEAxdzzFC8/mmfUuUgV
1La9ALZZZMq26U2zyN3HKng8gIPdR4cH6OA2WEYBseyeqZ9ig0zPCkV8b5AMhkrz
8e/e6wIDAQABAkEAsUK9/tJJf/IPcKJ09Gl2L+Iteg/mCGOVcpnFqXWodlSZMG6T
ZtbTMXtARvWQ30gR/0A2DMrFBYffQKBwTNcCgQIhAOIHygFqnt4QMY1Li/M56jjf
OFdmWDLUblEOLthNg2ZBAiEA4BkRoH0wvJqZVkQFHXXebkBkVyTQozPIVy/aoZul
MisCIDqDlbIELhiCzLEta4+N4KrUbruBjTerctuc9lecyXWBAiB/1a0ZePC5VhOM
g0bLtHZLPE5A685cmuh42+KC9++3ewIgTyEvIwUC672vosULHHLMbQBiqCoqbTkW
fEd3Lme8LKQ=
-----END PRIVATE KEY-----
)EOF";

/******************************************************************
 * LED CONSTANTS
 *****************************************************************/
#define LED_PIN_ANALOG_RANGE 256  // The LED pin analog range
#define LED_PIN_BLUE D1           // GPIO5 = Blue LED strip PIN
#define LED_PIN_GREEN D2          // GPIO4 = Green LED strip PIN
#define LED_PIN_NODE D4           // GPIO2 = Board blue LED
#define LED_PIN_RED D3            // GPIO0 = Red LED strip PIN

unsigned int ledBlueValue(255);
unsigned int ledGreenValue(255);
unsigned int ledRedValue(255);

/******************************************************************
 * LED FUNCTIONS
 *****************************************************************/

/**
 * Tries to set the blue LED pin.
 * @param {unsigned int} blue Blue color value between 0 - 255.
 * @return {bool} True if it has set the value.
 */
bool setBlueLED(unsigned int blue) {
  bool result(true);
  if (0 <= blue && 256 > blue) {
    analogWrite(LED_PIN_BLUE, blue);
    ledBlueValue = blue;
  } else {
    Serial.printf("Invalid blue value (0-255): %d\n", blue);
    result = false;
  }
  return result;
}

/**
 * Tries to set the green LED pin.
 * @param {unsigned int} green Green color value between 0 - 255.
 * @return {bool} True if it has set the value.
 */
bool setGreenLED(unsigned int green) {
  bool result(true);
  if (0 <= green && 256 > green) {
    analogWrite(LED_PIN_GREEN, green);
    ledGreenValue = green;
  } else {
    Serial.printf("Invalid green value (0-255): %d\n", green);
    result = false;
  }
  return result;
}

/**
 * Tries to set the red LED pin.
 * @param {unsigned int} red Red color value between 0 - 255.
 * @return {bool} True if it has set the value.
 */
bool setRedLED(unsigned int red) {
  bool result(true);
  if (0 <= red && 256 > red) {
    analogWrite(LED_PIN_RED, red);
    ledRedValue = red;
  } else {
    Serial.printf("Invalid red value (0-255): %d\n", red);
    result = false;
  }
  return result;
}

/**
 * Sets up the LED PINs analog range.
 */
void setupLEDRange() {
  analogWriteRange(LED_PIN_ANALOG_RANGE);
  setBlueLED(255);
  delay(250);
  setBlueLED(0);
  delay(250);
  setGreenLED(255);
  delay(250);
  setGreenLED(0);
  delay(250);
  setRedLED(255);
  delay(250);
  setRedLED(0);
  delay(250);
  setBlueLED(255);
  setGreenLED(255);
  setRedLED(255);
}

/**
 * Returns the current blue LED pin value between 0 and 255.
 * @return {unsigned int} The current LED value.
 */
unsigned int getBlue() { return ledBlueValue; }

/**
 * Returns the current red LED pin value between 0 and 255.
 * @return {unsigned int} The current LED value.
 */
unsigned int getGreen() { return ledGreenValue; }

/**
 * Returns the current red LED pin value between 0 and 255.
 * @return {unsigned int} The current LED value.
 */
unsigned int getRed() { return ledRedValue; }

/******************************************************************
 * SERVICE FUNCTIONS
 *****************************************************************/

/**
 * Returns if the WiFi connection is up.
 * @return {bool} True if its connected.
 */
bool isWiFiConnected() {
  return WL_CONNECTED == WiFi.status();
}

/**
 * Tries to connect to the WiFi network using
 * the pre configured SSID and password.
 * @return {bool} True if it has connected.
 */
bool connectToWifi() {
  Serial.printf("\nConnecting to %s...\n", WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.hostname(SERVICE_HOST_NAME);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  // Wait for connection.
  if (WL_CONNECTED == WiFi.waitForConnectResult(WIFI_CONNECTION_TIMEOUT)) {
    Serial.printf("WiFi connected with IP: %s.\n", WiFi.localIP().toString().c_str());
    Serial.printf("Setting local DNS: %s...\n", WiFi.hostname());
    MDNS.begin(SERVICE_HOST_NAME);
    return true;
  } else {
    Serial.printf("Couldn't connect to WiFi after %d seconds, status: %d.\n", WIFI_CONNECTION_TIMEOUT / 1000, WiFi.status());
    Serial.printf("Push RST to try again.\n");
    return false;
  }
}

/**
 * NodeMCU LED animation to sinalize a service warning.
 */
void alertLEDAnimation () {
  for (byte iteration(0); iteration < 4;++iteration) {
    digitalWrite(LED_PIN_NODE, HIGH);
    delay(125);
    digitalWrite(LED_PIN_NODE, LOW);
    delay(125);
  }
}

/**
 * Sets the default HTTP response headers of the service.
 */
void setDefaultHeaders () {
  const unsigned int headerMaxCount(sizeof(HTTP_DEFAULT_HEADERS) / sizeof(HTTP_DEFAULT_HEADERS[0]));
  for (unsigned int headerIndex(0); headerIndex < headerMaxCount; headerIndex += 2) {
    server.sendHeader(HTTP_DEFAULT_HEADERS[headerIndex], HTTP_DEFAULT_HEADERS[headerIndex + 1]);
  }
}

/**
 * Handles a invalid arguments request.
 */
void handle403() {
  // Headers already defined by the handler.
  server.send(403, "text/plain; charset=UTF-8", "Invalid arguments, do not repeat.");
  alertLEDAnimation();
  Serial.printf("403 - Handled.\n");
}

/**
 * Handles any other HTTP request that not exists.
 */
void handle404() {
  setDefaultHeaders();

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
  server.send(404, "text/plain; charset=UTF-8", message);
  alertLEDAnimation();
  Serial.printf("404 - Handled.\n");
}

/**
 * Handles a not allowed HTTP method request.
 */
void handle405() {
  // Headers already defined by the handler.
  server.send(405, "text/plain; charset=UTF-8", "Method not allowed.");
  alertLEDAnimation();
  Serial.printf("405 - Handled.\n");
}

/**
 * Handles the service main route, returning the LED colors or setting them.
 * @see https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WebServer/README.rst
 */
void handleRoot() {
  String rgbOutput("r=%red%&g=%green%&b=%blue%");

  setDefaultHeaders();
  digitalWrite(LED_PIN_NODE, HIGH);

  int requestMethod = server.method();
  if(HTTP_POST == requestMethod) {
    Serial.printf("POST arguments: %s.\n", server.arg("plain").c_str());
    if (server.hasArg("r") || server.hasArg("g") || server.hasArg("b")) {
      bool success(true);
      success &= setBlueLED((unsigned int)server.arg("b").toInt());
      success &= setGreenLED((unsigned int)server.arg("g").toInt());
      success &= setRedLED((unsigned int)server.arg("r").toInt());
      if (success) {
        rgbOutput.replace("%blue%", String(getBlue()));
        rgbOutput.replace("%green%", String(getGreen()));
        rgbOutput.replace("%red%", String(getRed()));
        server.send(201, "text/plain; charset=UTF-8", rgbOutput);
        Serial.printf("201 - Handled.\n");
      } else {
        handle403();
      }
    } else {
      handle403();
    }
  } else if (HTTP_GET == requestMethod) {
    rgbOutput.replace("%blue%", String(getBlue()));
    rgbOutput.replace("%green%", String(getGreen()));
    rgbOutput.replace("%red%", String(getRed()));
    server.send(200, "text/plain; charset=UTF-8", rgbOutput);
    Serial.printf("200 - Handled.\n");
  } else {
    handle405();
  }

  digitalWrite(LED_PIN_NODE, LOW);
}

/**
 * Sets up the HTTPS service.
 */
void setupService() {
  configTime(-3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  server.getServer().setRSACert(new BearSSL::X509List(serverCert), new BearSSL::PrivateKey(serverKey));
  server.on("", handleRoot);
  server.on("/", handleRoot);
  server.onNotFound(handle404);
  server.begin();
  
  Serial.printf("Server is ready on: https://%s.local\n", SERVICE_HOST_NAME);
  Serial.printf("Access or post it with: r=255&g=255&b=255\n");
}

/******************************************************************
 * NodeMCU FUNCTIONS
 *****************************************************************/

/**
 * NodeMCU setup function, called before the main loop start.
 * It restarts the board if no connection is estabilished.
 */
void setup() {
  pinMode(LED_PIN_NODE, OUTPUT);
  Serial.begin(115200);

  if (!connectToWifi()) {
    delay(WIFI_CONNECTION_TIMEOUT);
    ESP.restart();
  }

  setupLEDRange();
  setupService();
}

/**
 * NodeMCU main loop function, called every iteration.
 */
void loop() {
  // Handles one requests.
  server.handleClient();
  // Updates the DNS broadcast.
  MDNS.update();
}
