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

int menuChatNum = 0;

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

    if (text == "/start") {
      String welcome = "Добро пожаловать Бро *" + from_name + "*.\n\n";
      
      welcome += "`Это главное меню управления`\n";
      welcome += "`не очень умным домом.`\n\n";
      welcome += "`Что бы использовать меню,`\n";
      welcome += "`достаточно отправить в чат`\n";
      welcome += "`номер желаемой команды.`\n\n";
      
      welcome += "*Меню управления:*\n";
      welcome += "*1 -* _Балкон_\n";
      welcome += "*2 -* _Комната_\n";
      welcome += "*3 -* _Настройки_\n";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }

    if (text == "/options") {
      String keyboardJson = "[\"1\", \"2\", \"3\", \"4\", \"5\"]";
      bot.sendMessageWithReplyKeyboard(chat_id, "Кнопки управления меню", "", keyboardJson, true, false);
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
