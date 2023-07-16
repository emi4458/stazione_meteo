
#include "CTBot.h"
#include "DHT.h"
#define DHTPIN D2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
#define TEMP  "Temperatura"
#define DOMANDA_SE_PIOVE  "Piove"
#define APRI_CANCELLETTO "Cancelletto"
#define RELEPIN D13
CTBot myBot;

String ssid = "Casa Pedica";    
String pass = "password"; 
String token = "token";   
uint8_t led = 2;            // the onboard ESP8266 LED.    
int pioggia=0;
int check=0;
unsigned long tempo=0;

CTBotInlineKeyboard pulsante;
DHT dht(DHTPIN, DHTTYPE);
TBMessage msg;

///////////////////////////////////////////////////////////////////////////////
void scriviTemperatura(){
      float h = dht.readHumidity();
      float t = dht.readTemperature()-2.1;
      float f = dht.readTemperature(true);
    
      // Check if any reads failed and exit early (to try again).
      if (isnan(h) || isnan(t) || isnan(f)) {
        Serial.println(F("Failed to read from DHT sensor!"));
        return;
      }
      // Compute heat index in Celsius (isFahreheit = false)
      float hic = dht.computeHeatIndex(t, h, false);
      
      myBot.sendMessage(msg.sender.id,("Umidità: "+String(h)+"%"));
      myBot.sendMessage(msg.sender.id,("Temperatura: "+String(t)+"°C"));
      myBot.sendMessage(msg.sender.id,("Percepita: "+String(hic)+"°C"));
      
}
/////////////////////////////////////////////////////////////////////////////////////


void setup() {
  
  pulsante.addButton("Mostra temperatura, umidità, temperatura percepita", TEMP, CTBotKeyboardButtonQuery);
  pulsante.addButton("Piove?", DOMANDA_SE_PIOVE, CTBotKeyboardButtonQuery);
  pulsante.addRow();

  pinMode(RELEPIN,OUTPUT);
  
  dht.begin();
	// initialize the Serial
	Serial.begin(115200);
	//Serial.println("Starting TelegramBot...");

	// connect the ESP8266
	myBot.wifiConnect(ssid, pass);

	myBot.setTelegramToken(token);

	//test connection
	if (myBot.testConnection())
		Serial.println("\ntestConnection OK");
	else
		Serial.println("\ntestConnection NOK");

  

}



void loop() {

  
	// a variable to store telegram message data
	TBMessage msg;

  ////////////////////////////////////////////////////
  int sensore_pioggia=analogRead(A0);
  if (sensore_pioggia>20 and pioggia==0){
     if(millis()-3600000>tempo){
      myBot.sendMessage(msg.sender.id,("Sta piovendo."));
      myBot.sendMessage(176562109,"Sta piovendo.");
      myBot.sendMessage(838642583,"Sta piovendo");
      pioggia=1;
      tempo=millis(); 
     }
  }
  if (sensore_pioggia<=20 and pioggia==1){
    pioggia=0;
  }
 ////////////////////////////////////////////////////
  
	// if there is an incoming message...
	if (myBot.getNewMessage(msg)) {
    if (msg.text.equalsIgnoreCase("id")){
      myBot.sendMessage(msg.sender.id,((String)msg.sender.id));
      myBot.sendMessage(idemiliano,"Sei Emiliano");
      myBot.sendMessage(idgiuliano,"Sei Giuliano");
    }
    
		if (msg.text.equalsIgnoreCase("Temperatura")) {              
      float h = dht.readHumidity();
      float t = dht.readTemperature()-2.1;
      float f = dht.readTemperature(true);
    
      // Check if any reads failed and exit early (to try again).
      if (isnan(h) || isnan(t) || isnan(f)) {
        Serial.println(F("Failed to read from DHT sensor!"));
        return;
      }
      // Compute heat index in Celsius (isFahreheit = false)
      float hic = dht.computeHeatIndex(t, h, false);
      
      myBot.sendMessage(msg.sender.id,("Umidità: "+String(h)+"%"));
      myBot.sendMessage(msg.sender.id,("Temperatura: "+String(t)+"°C"));
      myBot.sendMessage(msg.sender.id,("Percepita: "+String(hic)+"°C"));
		}

		
   if (msg.callbackQueryData.equals(TEMP)) {
      float h = dht.readHumidity();
      float t = dht.readTemperature()-2.1;
      float f = dht.readTemperature(true);
    
      // Check if any reads failed and exit early (to try again).
      if (isnan(h) || isnan(t) || isnan(f)) {
        Serial.println(F("Failed to read from DHT sensor!"));
        return;
      }
      // Compute heat index in Celsius (isFahreheit = false)
      float hic = dht.computeHeatIndex(t, h, false);
      
      myBot.sendMessage(msg.sender.id,("Umidità: "+String(h)+"%"));
      myBot.sendMessage(msg.sender.id,("Temperatura: "+String(t)+"°C"));
      myBot.sendMessage(msg.sender.id,("Percepita: "+String(hic)+"°C"));
   }
   
   if (msg.callbackQueryData.equals(DOMANDA_SE_PIOVE)){
    if(pioggia==1){
      myBot.sendMessage(msg.sender.id,("Si"));
      }
      else{
        myBot.sendMessage(msg.sender.id,("No"));
      }
    }
    
   else {                                                    // otherwise...
     // generate the message for the sender
      String reply;
      reply = (String)"Benvenuto/a "+msg.sender.firstName+", questo è quello che posso fare:";
      myBot.sendMessage(msg.sender.id, reply ,pulsante);            
    }
	}
	delay(100);
}
