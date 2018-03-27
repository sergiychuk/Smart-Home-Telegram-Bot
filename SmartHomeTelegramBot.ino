/*******************************************************************
    An example of how to use a custom reply keyboard markup.
 *                                                                 *
 *                                                                 *
    written by Brian Lough
 *******************************************************************/
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

// Initialize Wifi connection to the router
char ssid[] = "FEZ_DEZ";     // your network SSID (name)
char password[] = "18273645"; // your network key

// Initialize Telegram BOT
#define BOTtoken "577387895:AAG7pnUscdGbhZ-kyEdAy0yRG3X11fXW8oA"  // your Bot Token (Get from Botfather)

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

int Bot_mtbs = 500; //mean time between scan messages
long Bot_lasttime;   //last time messages' scan has been done

const int ledPin = LED_BUILTIN;
int ledStatus = 0;

char* fezChatID = "391878473";

void setup() {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);

  // Set WiFi to station mode and disconnect from an AP if it was Previously connected
  Serial.println("");
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  
  // attempt to connect to Wifi network:
  Serial.println("");
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    blinkBuitlinLed(2);
    delay(50);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  delay(10);
  bot.sendMessage(fezChatID, "Bot started and connected!\n", "Markdown");
}
void loop() {
  if (millis() > Bot_lasttime + Bot_mtbs)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages) {
      Serial.println("Got new response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    Bot_lasttime = millis();
  }
}
void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Guest";

    if (text == "/showinfo") {
      bot.sendMessage(chat_id, "Name: *" + from_name + "*.\nChat ID: *" + chat_id + "*.", "Markdown");
    }
    
    if (text == "/ledon") {
      digitalWrite(ledPin, LOW);   // turn the LED on (HIGH is the voltage level)
      ledStatus = 1;
      bot.sendSimpleMessage(chat_id, "Led is *ON*", "Markdown");
    }

    if (text == "/ledoff") {
      ledStatus = 0;
      digitalWrite(ledPin, HIGH);    // turn the LED off (LOW is the voltage level)
      bot.sendSimpleMessage(chat_id, "Led is *OFF*", "Markdown");
    }

    if (text == "/status") {
      if (ledStatus) {
        bot.sendSimpleMessage(chat_id, "Led is *ON*", "Markdown");
      } else {
        bot.sendSimpleMessage(chat_id, "Led is *OFF*", "Markdown");
      }
    }
    
    if (text == "/books") {
      bot.sendMessage(chat_id, "Тут можно купить *ШМАЛЮКЮ*", "Markdown");
    }
    if (text == "/speedshoes") {
      bot.sendMessage(chat_id, "Тут можно купить *АМФЕТАМІНЧІК*", "Markdown");
    }

    if (text == "/options") {
      String keyboardJson = "[[\"/books\", \"/speedshoes\"],[\"/ledon\", \"/ledoff\"],[\"/status\"]]";
      bot.sendMessageWithReplyKeyboard(chat_id, "Choose from one of the following options", "", keyboardJson, true, false);
    }

    if (text == "/start") {
      String welcome = "Welcome to Universal Arduino Telegram Bot library, " + from_name + ".\n";
      welcome += "This is Reply Keyboard Markup example.\n\n";
      welcome += "/ledon : to switch the Led ON\n";
      welcome += "/ledoff : to switch the Led OFF\n";
      welcome += "/status : Returns current status of LED\n";
      welcome += "/options : returns the reply keyboard\n";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }
  }
}

void blinkBuitlinLed(int blinkCount) {
  for (int i = 0; i < blinkCount; i++) {
    digitalWrite(ledPin, LOW);
    delay(100);
    digitalWrite(ledPin, HIGH);
  }
}

