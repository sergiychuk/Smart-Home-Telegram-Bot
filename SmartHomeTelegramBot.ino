// String keyboardJson = "[[\"0\", \"1\", \"2\", \"3\", \"4\"],[\"/status\"]]";

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

int Bot_mtbs = 100; //mean time between scan messages
long Bot_lasttime;   //last time messages' scan has been done

const int ledPin = LED_BUILTIN;


// MENU NUMBERS
int menuChatNumber = 0;
int userChatChoise = 0;

// BALCONY NUMBERS
bool fanStatus = false;
bool lightStatus = false;

char* fezChatID = "391878473";

void setup() {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  pinMode(4, OUTPUT);
  digitalWrite(ledPin, HIGH);
//  for (int i = 0; i < 1023; i++){
//    analogWrite(lightPin, i);
//    delay(10);
//    Serial.println(i);
//  }
  
  delay(25);

  // Set WiFi to station mode and disconnect from an AP if it was Previously connected
  Serial.println("");
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(25);

  // attempt to connect to Wifi network:
  Serial.println("");
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    blinkBuitlinLed(1);
    delay(250);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  delay(10);
  //bot.sendMessage(fezChatID, "Bot started and connected!\n", "Markdown");
}
void loop() {
  if (millis() > Bot_lasttime + Bot_mtbs)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages) {
      Serial.println("Got new response message");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    Bot_lasttime = millis();
  }
}
void handleNewMessages(int numNewMessages) {
  Serial.println("Working On New Messages");
  Serial.println(String(numNewMessages));

  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Guest";

    if (text == "/start" || text == "0") {
      userChatChoise = (text).toInt();
    }
    if (text == "1") {
      userChatChoise = (text).toInt();
    }
    if (text == "2") {
      userChatChoise = (text).toInt();
    }
    if (text == "3") {
      userChatChoise = (text).toInt();
    }
    if (text == "4") {
      userChatChoise = (text).toInt();
    }
    if (text == "/status") {
      bot.sendMessage(chat_id, "User chat choise number: *" + String(userChatChoise) + "*\nCurrent menu chat number: *" + String(menuChatNumber) + "*\n", "Markdown");
      return;
    }
    
    //menuChatNumber = userChatChoise;
    //setChatMenu(menuChatNumber, chat_id, from_name);

    if (text == "/servoOn"){
      pinMode(4, OUTPUT);
      analogWrite(4, HIGH);
      delay(1000);
      analogWrite(4, LOW);
    }
    if (text == "/servoOff"){
      pinMode(4, INPUT);
    }
    //    else {
    //      bot.sendMessage(chat_id, "Извините, я Вас не понял. Просто отправьте мне цифру меню\nили отправьте */start* чтобы открыть главное меню.", "Markdown");
    //    }

  }
}

void setChatMenu(int menuNum, String chat_id, String user_name) {
  if (menuNum == 0) {
    String startMessage = "Добро пожаловать Бро *" + user_name + "*.\n\n";
    startMessage += "`Это главное меню управления`\n";
    startMessage += "`не очень умным домом.`\n\n";
    startMessage += "`Что бы использовать меню,`\n";
    startMessage += "`достаточно отправить в чат`\n";
    startMessage += "`номер желаемой команды.`\n\n";
    startMessage += "*Меню управления:*\n";
    startMessage += "*1 -* _Балкон_\n";
    startMessage += "*2 -* _Комната_\n";
    startMessage += "*3 -* _Настройки_\n";
    String keyboardJson = "[[\"1\", \"2\", \"3\"],[\"/status\"]]";
    bot.sendMessageWithReplyKeyboard(chat_id, startMessage, "Markdown", keyboardJson, true);
  }
  if (menuNum == 1) {
    String balconyMessage = "*БАЛКОН*.\n\n";
    balconyMessage += "*Вытяжка:* _" + String(fanStatus) +"_.\n";
    balconyMessage += "*Освещение:* _" + String(lightStatus) +"_.\n\n";
    balconyMessage += "*Меню управления балконом:*\n";
    balconyMessage += "*0 -* _Главное меню_\n";
    balconyMessage += "*1 -* _Вытяжка_\n";
    balconyMessage += "*2 -* _Освещение_\n";
    String keyboardJson = "[[\"0\", \"1\", \"2\"]]";
    bot.sendMessageWithReplyKeyboard(chat_id, balconyMessage, "Markdown", keyboardJson, true);
  }
}

void blinkBuitlinLed(int blinkCount) {
  for (int i = 0; i < blinkCount; i++) {
    digitalWrite(ledPin, LOW);
    delay(100);
    digitalWrite(ledPin, HIGH);
  }
}


