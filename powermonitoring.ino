#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <PZEM004Tv30.h>
#include <LiquidCrystal_I2C.h>
/* Hardware Serial2 is only available on certain boards.
 * For example the Arduino MEGA 2560
*/
LiquidCrystal_I2C lcd(0x27, 16, 2);
#if defined(ESP32)
PZEM004Tv30 pzem(Serial2, 16, 17);
#else
PZEM004Tv30 pzem(Serial2);
#endif

// Replace the next variables with your SSID/Password combination
const char* ssid = "alex";
const char* password = "123456aaa";

// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";
const char* mqtt_server = "192.168.218.164";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
    Serial.begin(115200);
    setup_wifi();
    client.setServer(mqtt_server,1883);
    client.setCallback(callback);
    lcd.begin();
    lcd.backlight();
    lcd.print("Hello,Greetings");

    // Uncomment in order to reset the internal energy counter
    // pzem.resetEnergy()
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    lcd.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");
  lcd.setCursor(0,0);
  lcd.print("wiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
   
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
 
  // Feel free to add more if statements to control more GPIOs with MQTT
 
  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  if (String(topic) == "energy") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      //digitalWrite(ledPin, HIGH);
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      //digitalWrite(ledPin, LOW);
    }
  }
}
 
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32Client","alex","1234")) {
      Serial.println("connected");
      lcd.setCursor(0,1);
      lcd.print("MQTT connected");
      // Subscribe
      client.subscribe("output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
   if (!client.connected()) {
    reconnect();
  }
  client.loop();
  long now = millis();
  if (now - lastMsg > 10000) {
    lastMsg = now;
        
    Serial.print("Custom Address:");
    Serial.println(pzem.readAddress(), HEX);

    // Read the data from the sensor
    float voltage = pzem.voltage();
    float current = pzem.current();
    float power = pzem.power();
    float energy = pzem.energy();
    float frequency = pzem.frequency();
    float pf = pzem.pf();

    // Check if the data is valid
    if(isnan(voltage)){
        Serial.println("Error reading voltage");
    } else if (isnan(current)) {
        Serial.println("Error reading current");
    } else if (isnan(power)) {
        Serial.println("Error reading power");
    } else if (isnan(energy)) {
        Serial.println("Error reading energy");
    } else if (isnan(frequency)) {
        Serial.println("Error reading frequency");
    } else if (isnan(pf)) {
        Serial.println("Error reading power factor");
    } else {

        // Print the values to the Serial console
        Serial.print("Voltage: ");      Serial.print(voltage);      Serial.println("V");
        Serial.print("Current: ");      Serial.print(current);      Serial.println("A");
        Serial.print("Power: ");        Serial.print(power);        Serial.println("W");
        Serial.print("Energy: ");       Serial.print(energy,3);     Serial.println("kWh");
        Serial.print("Frequency: ");    Serial.print(frequency, 1); Serial.println("Hz");
        Serial.print("PF: ");           Serial.println(pf);
        lcd.setCursor(0,0);
        lcd.print("voltage:");
        lcd.print(voltage);
        lcd.print("V");
        lcd.setCursor(0,1);
        lcd.print("Current:");
        lcd.print(current);
        lcd.print("A");
        lcd.setCursor(0,0);
        lcd.print("Power:");
        lcd.print(power);
        lcd.print("W");
        lcd.setCursor(0,0);
        lcd.print("Energy:");
        lcd.print(energy);
        lcd.print("Kwh");
        
        

    }

    // Convert the value to a char array
    char energyString[8];
    dtostrf(energy, 4, 3, energyString);
    Serial.print("energy: ");
    Serial.println(energyString);
    client.publish("energy",energyString);

        // Convert the value to a char array
    char voltageString[8];
    dtostrf(voltage, 3, 3, voltageString);
    Serial.print("voltage: ");
    Serial.println(voltageString);
    client.publish("voltage",voltageString);

            // Convert the value to a char array
    char powerString[8];
    dtostrf(power, 3, 3, powerString);
    Serial.print("power: ");
    Serial.println(powerString);
    client.publish("power",powerString);

            // Convert the value to a char array
    char currentString[8];
    dtostrf(current, 3, 3, currentString);
    Serial.print("current: ");
    Serial.println(currentString);
    client.publish("current",currentString);

    Serial.println();
    delay(2000);
}
}