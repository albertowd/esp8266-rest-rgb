# esp8266-rest-rgb

A NodeMCU ESP8266 V3 REST service to control a LED strip color.

It starts connecting to a WiFi SSID and then serves a rest service to GET/POST the color of the LED strip connected to it. The connection uses the below PINs that receives a value between 0 and 255:

```cpp
#define LED_PIN_ANALOG_RANGE 256  // The LED pin analog range
#define LED_PIN_BLUE D1           // GPIO5 = Blue LED strip PIN
#define LED_PIN_GREEN D2          // GPIO4 = Green LED strip PIN
#define LED_PIN_RED D3            // GPIO0 = Red LED strip PIN
```
## Fetching Current Color

To fetch the current color, just do a GET request to the local DNS:

```sh
curl -X GET -k https://couch-rgb.local
```

It will return a URL encoded RGB definition:

> r=255&g=255&b=255


## Setting Color

A the GET returns the current color, it can be set using the same URL encoded patter as a POST request:

```sh
curl -X POST -k https://couch-rgb.local -d "r=100&g=100&b=100"
```

It will return a URL encoded RGB definition:

> r=100&g=100&b=100
