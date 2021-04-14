#include <Agentuino.h>
#include <Ethernet.h>
#include <SPI.h> 


#include "DHT.h"
 
#define DHTPIN 7 // pino que estamos conectado
#define DHTTYPE DHT11 // DHT 11
 

DHT dht(DHTPIN, DHTTYPE);


//Rede Ethernet 
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192,168,0,168 };
byte subnet[] = { 255, 255, 255, 0 };
//static byte gateway[] = { 10, 10, 1, 6 };

EthernetClient client;

// Agentuino começa aqui--------------------------------------------------------------------

// RFC1213-MIB OIDs
// .iso (.1)
// .iso.org (.1.3)
// .iso.org.dod (.1.3.6)
// .iso.org.dod.internet (.1.3.6.1)
// .iso.org.dod.internet.mgmt (.1.3.6.1.2)
// .iso.org.dod.internet.mgmt.mib-2 (.1.3.6.1.2.1)
// .iso.org.dod.internet.mgmt.mib-2.system (.1.3.6.1.2.1.1)
// .iso.org.dod.internet.mgmt.mib-2.system.sysDescr (.1.3.6.1.2.1.1.1)
const static char sysDescr[20] PROGMEM     = "1.3.6.1.2.1.1.1.0";  // read-only  (DisplayString)

// .iso.org.dod.internet.mgmt.mib-2.system.sysObjectID (.1.3.6.1.2.1.1.2)
const static char sysObjectID[20] PROGMEM   = "1.3.6.1.2.1.1.2.0";  // read-only  (ObjectIdentifier)

// .iso.org.dod.internet.mgmt.mib-2.system.sysUpTime (.1.3.6.1.2.1.1.3)
const static char sysUpTime[20] PROGMEM     = "1.3.6.1.2.1.1.3.0";  // read-only  (TimeTicks)

// .iso.org.dod.internet.mgmt.mib-2.system.sysContact (.1.3.6.1.2.1.1.4)
const static char sysContact[20] PROGMEM    = "1.3.6.1.2.1.1.4.0";  // read-write (DisplayString)

// .iso.org.dod.internet.mgmt.mib-2.system.sysName (.1.3.6.1.2.1.1.5)
const static char sysName[20] PROGMEM       = "1.3.6.1.2.1.1.5.0";  // read-write (DisplayString)

// .iso.org.dod.internet.mgmt.mib-2.system.sysLocation (.1.3.6.1.2.1.1.6)
const static char sysLocation[20] PROGMEM   = "1.3.6.1.2.1.1.6.0";  // read-write (DisplayString)

// .iso.org.dod.internet.mgmt.mib-2.system.sysServices (.1.3.6.1.2.1.1.7)
const static char sysServices[20] PROGMEM   = "1.3.6.1.2.1.1.7.0";  // read-only  (Integer)

// Arduino defined OIDs
// .iso.org.dod.internet.private (.1.3.6.1.4)
// .iso.org.dod.internet.private.enterprises (.1.3.6.1.4.1)
// .iso.org.dod.internet.private.enterprises.arduino (.1.3.6.1.4.1.36582)
// .iso.org.dod.internet.private.enterprises.arduino.value.valA0-A5 (.1.3.6.1.4.1.36582.3.1-6)
const static char temp[24] PROGMEM = "1.3.6.1.4.1.36582.3.1.0";  // read-only  (Integer)
const static char umidade[24] PROGMEM = "1.3.6.1.4.1.36582.3.2.0"; // read-only (Integer)



const static char locDescr[20] = "Agentuino";

const static char locObjectID[20] = "1.3.6.1.4.1.36582";
const static uint32_t locUpTime = 0;
const static char locContact[29] = "https://github.com/lailtonmontenegro/";
const static char locName[20] = "Agentuino";
const static char locLocation[20] = "Natal, RN";
const static int32_t locServices = 7;
int loctemp = 0;
int locumidade = 0; 

uint32_t dispMillis = 0;


char oid[SNMP_MAX_OID_LEN];
SNMP_API_STAT_CODES api_status;
SNMP_ERR_CODES status;


void pduReceived()
{
  SNMP_PDU pdu;
  Serial.println("UDP Packet Received...");
  api_status = Agentuino.requestPdu(&pdu);
  if ( (pdu.type == SNMP_PDU_GET || pdu.type == SNMP_PDU_GET_NEXT || pdu.type == SNMP_PDU_SET)
    && pdu.error == SNMP_ERR_NO_ERROR && api_status == SNMP_API_STAT_SUCCESS ) {
    pdu.OID.toString(oid);
    Serial.print("OID = ");
    Serial.println(oid);
    
    if ( pdu.type == SNMP_PDU_SET ) {
      status = SNMP_ERR_READ_ONLY;
    } else if ( strcmp_P(oid, sysDescr ) == 0 ) {
      status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, locDescr);
    } else if ( strcmp_P(oid, sysUpTime ) == 0 ) {
      status = pdu.VALUE.encode(SNMP_SYNTAX_TIME_TICKS, locUpTime);
    } else if ( strcmp_P(oid, sysName ) == 0 ) {
      status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, locName);
    } else if ( strcmp_P(oid, sysContact ) == 0 ) {
      status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, locContact);
    } else if ( strcmp_P(oid, sysLocation ) == 0 ) {
      status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, locLocation);
    } else if ( strcmp_P(oid, sysServices) == 0 ) {
      status = pdu.VALUE.encode(SNMP_SYNTAX_INT, locServices);
     } else if ( strcmp_P(oid, sysObjectID) == 0 ) {
      status = pdu.VALUE.encode(SNMP_SYNTAX_OCTETS, locObjectID);
    } else if ( strcmp_P(oid, temp) == 0 ) {
      status = pdu.VALUE.encode(SNMP_SYNTAX_INT, loctemp);
    } else if ( strcmp_P(oid, umidade) == 0 ) {
      status = pdu.VALUE.encode(SNMP_SYNTAX_INT, locumidade);
    
    } else {
      status = SNMP_ERR_NO_SUCH_NAME;
    }
    pdu.type = SNMP_PDU_RESPONSE;
    pdu.error = status;
    Agentuino.responsePdu(&pdu);
  }
  Agentuino.freePdu(&pdu);
  
}

 
void setup() 
{
  Serial.begin(9600);
  Ethernet.begin(mac, ip, subnet);
  dht.begin();
 // Wire.begin();

 // disparou = false;
 // disptempo = true;

  api_status = Agentuino.begin();
 
  if ( api_status == SNMP_API_STAT_SUCCESS ) {
    Agentuino.onPduReceive(pduReceived);
    delay(10);
    Serial.println("SNMP Agent Initiated...");
    return;
  } else
  {
  delay(10);
  Serial.print("SNMP Agent failed!");
  }


  
}
 
void loop() 
{
 
  int u = (dht.readHumidity()); // variavel para umidade
  int t= (dht.readTemperature()); // variavel para temperatura

  Agentuino.listen();
   
    loctemp = t;
    locumidade = u;  
  
/*
    Serial.print("Umidade: ");
    Serial.print(u);
    Serial.print(" %t");
    Serial.print("Temperatura: ");
    Serial.print(t);
    Serial.println(" *C");
    delay(1000);*/
  
}
