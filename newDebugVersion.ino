#include "SIM900.h"
#include <SoftwareSerial.h>
#include "inetGSM.h"

// start: for GRPS Connection
InetGSM inet;
//CallGSM call;

char msg[2];
int numdata;
char inSerial[50];
int i=0;
boolean started=false;

boolean IsGPRSConnect = false;
// end: for GRPS Connection

int vsen1 = A0; //for pin deglaration
int vsen2 = A1; //for pin deglaration
int vsen3 = A2; //for pin deglaration
int vsen4 = A3; //for pin deglaration
int vsen5 = A4; //for pin deglaration
int wsen = A5;  //for pin deglaration

int vdata1;//save for vibration Sensor maping data
int vdata2;//save for vibration Sensor maping data
int vdata3;//save for vibration Sensor maping data
int vdata4;//save for vibration Sensor maping data
int vdata5;//save for vibration Sensor maping data
int wdata;//save for water Sensor mapping data

int vmdata1; //save for vibration Sensor real data
int vmdata2;//save for vibration Sensor real data
int vmdata3;//save for vibration Sensor real data
int vmdata4;//save for vibration Sensor real data
int vmdata5;//save for vibration Sensor real data
int wmdata;//save for water Sensor real data

boolean d = true; // debug check

void setup()
{
    if (d)
    {
        Serial.begin(9600);
        Serial.println("setup");
    }

    ConnectGPRS();

    pinMode(vsen1, INPUT); //for pin Mode Setup
    pinMode(vsen2, INPUT); //for pin Mode Setup
    pinMode(vsen3, INPUT); //for pin Mode Setup
    pinMode(vsen4, INPUT); //for pin Mode Setup
    pinMode(vsen5, INPUT); //for pin Mode Setup
    pinMode(wsen, INPUT); //for pin Mode Setup

}

void dataRead()
{
    if (d)
    {
        Serial.println("dataRead");
    }

    // read all sensor data and compare 0 to 10
    vmdata1 = analogRead(vsen1);
    vmdata2 = analogRead(vsen2);
    vmdata3 = analogRead(vsen3);
    vmdata4 = analogRead(vsen4);
    vmdata5 = analogRead(vsen5);

    wmdata = analogRead(wsen);

    vdata1 = map(vmdata1, 500, 1023, 0, 10);
    vdata2 = map(vmdata2, 500, 1023, 0, 10);
    vdata3 = map(vmdata3, 500, 1023, 0, 10);
    vdata4 = map(vmdata4, 500, 1023, 0, 10);
    vdata5 = map(vmdata5, 500, 1023, 0, 10);

    if( wmdata < 400 )
    {
        wmdata = 400;
    }

    wdata = map(wmdata, 400, 800, 0, 10);
}

void serialprint()
{
    Serial.println("serialprint");

    // Serial.begin(9600);
    // serial print sensor data
    Serial.print("Vibration Sensor1 data = ");
    Serial.println(vmdata1); // print vibration sensor 1 data
    Serial.print("Vibration Sensor2 data = ");
    Serial.println(vmdata2);// print vibration sensor 2 data
    Serial.print("Vibration Sensor3 data = ");
    Serial.println(vmdata3);// print vibration sensor 3 data
    Serial.print("Vibration Sensor4 data = ");
    Serial.println(vmdata4);// print vibration sensor 4 data
    Serial.print("Vibration Sensor5 data = ");
    Serial.println(vmdata5);// print vibration sensor 5 data
    Serial.print("Water Sensor data = ");
    Serial.println(wdata);// print water senso 1 data
    Serial.println("");

    Serial.print("Vibration Sensor1 mapping data = ");
    Serial.println(vdata1); // print vibration sensor 1 data
    Serial.print("Vibration Sensor2 mapping data = ");
    Serial.println(vdata2);// print vibration sensor 2 data
    Serial.print("Vibration Sensor3 mapping data = ");
    Serial.println(vdata3);// print vibration sensor 3 data
    Serial.print("Vibration Sensor4 mapping data = ");
    Serial.println(vdata4);// print vibration sensor 4 data
    Serial.print("Vibration Sensor5 mapping data = ");
    Serial.println(vdata5);// print vibration sensor 5 data
    Serial.print("Water Sensor data = ");
    Serial.println(wdata);// print water senso 1 data
    Serial.println("");
}

void loop()
{
    if (d) { Serial.println("loop"); }

    dataRead();
    DataChangeCheck();

    if (d) { serialprint(); }
}

void DataChangeCheck()
{
    if (d) { Serial.println("DataChangeCheck"); }

    String SensingLocaiotn1 = "Dhaka" ;
    String SensingLocaiotn2 = "Chittagong" ;
    String SensingLocaiotn3 = "Chylet" ;
    String SensingLocaiotn4 = "Khulna" ;
    String SensingLocaiotn5 = "Rajshahi" ;

    // start: Rikter scale
    if( vdata1 > 0)
    {
        if(IsGPRSConnect)
        {
            DataSentToServer(String(vdata1), SensingLocaiotn1, "00:00:00", String(wdata));
        }
    }

    if( vdata2 > 0)
    {
        if(IsGPRSConnect)
        {
            DataSentToServer(String(vdata2), SensingLocaiotn2, "00:00:00", String(wdata));
        }
    }

    if( vdata3 > 0)
    {
        if(IsGPRSConnect)
        {
            DataSentToServer(String(vdata3), SensingLocaiotn3, "00:00:00", String(wdata));
        }
    }

    if( vdata4 > 0)
    {
        if(IsGPRSConnect)
        {
            DataSentToServer(String(vdata4), SensingLocaiotn4, "00:00:00", String(wdata));
        }
    }

    if( vdata5 > 0)
    {
        if(IsGPRSConnect)
        {
            DataSentToServer(String(vdata5), SensingLocaiotn5, "00:00:00", String(wdata));
        }
    }
    // end: Rikter scale
    // start: water level
    if(wdata > 1)
    {
        if(IsGPRSConnect)
        {
            DataSentToServer(String(vdata1), SensingLocaiotn1, "00:00:00", String(wdata));
        }
    }
    // end: water level

}

void DataSentToServer(String RikterScale, String Location, String Time, String WaterLevel)
{
    
    if (d) { Serial.println("DataSentToServer"); }

    String UrlParameter = "/?r="+RikterScale+"&l="+Location+"&t="+Time+"&w="+WaterLevel;
    int UrlLength = UrlParameter.length();

    // start: url generatory for type character
    char path[UrlLength+1];

    for (int i = 0; i < UrlLength; i++)
    {
      char c = UrlParameter[i];

      path[i] = c;
    }
    path[UrlLength++] = 0;
    path[UrlLength++] = 0;
    path[UrlLength++] = 0;
    path[UrlLength] = 0;

    // numdata = inet.httpGET("www.earthquake.tk", 80, path, msg, 2);
    numdata = inet.httpPOST("www.earthquake.tk", 80, path, msg, 2);
    //Print the results.
    if (d)
    {
        Serial.println("\nNumber of data received:");
        Serial.println(numdata);
        Serial.println("\nData received:");
        Serial.println(msg);
    }
}

boolean ConnectGPRS()
{
    if (d) { Serial.println("ConnectGPRS"); }
    if (d) { Serial.println("GSM Shield testing."); }

    if (gsm.begin(19200))
    {
        if (d) { Serial.println("\nstatus=READY"); }
        started = true;
    }
    else
    {
        if (d) { Serial.println("\nstatus=IDLE"); }
        started = false;
    }

    //GPRS attach, put in order APN, username and password.
    //If no needed auth let them blank.
    boolean status = false;
    if (started)
    {
        if (inet.attachGPRS("internet", "", ""))
        {
            if (d) { Serial.println("status=ATTACHED"); }
            status = true;
            IsGPRSConnect = true;
        }
        else
        {
            if (d) { Serial.println("status=ERROR"); }
            status = false;
            IsGPRSConnect = false;
        }
        delay(1000);

        //Read IP address.
        gsm.SimpleWriteln("AT+CIFSR");
        delay(5000);
        //Read until serial buffer is empty.
        gsm.WhileSimpleRead();
    }
    return status;
}

void TryConnectGPRS()
{

}