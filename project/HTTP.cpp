#include "HTTP.hpp"
#include "secrets.h"


bool request_send(String stop_id)
{
    if (WiFi.status()!= WL_CONNECTED)
    {
        Serial.println("No WiFi connection - unable to send request");
        return false;
    }

    HTTPClient http; 

    String url = String("https://realtime-api.trafiklab.se/v1/departures/") + stop_id + "?key=" + API_KEY; // creation of URL for the upcoming request
    
    bool begin_result = http.begin(url);

    if (begin_result == false) 
    {
        Serial.println ("Request not sent - bad url");
        return false;
    }

    int http_code = http.GET();

    int short_code = http_code/100;

    switch (short_code)
    {
        case (1):
        Serial.println ("Code 1xx: Informational response (weak TRUE - continues)");
        Serial.print(http_code);
        break;

        case (2):
        Serial.println("Code 2xx: Success (TRUE)");
        Serial.print(http_code);
        break;

        case (3):
        Serial.println ("Code 3xx: Redirection (weak TRUE - continues)");
        Serial.print(http_code);
        break;

        case (4):
        Serial.println("Code 4xx: Client error (strong FALSE - break)");
        Serial.print(http_code);
        http.end();
        return false;

        case (5):
        Serial.println("Code 5xx: Server error (strong FALSE - break)");
        Serial.print (http_code);
        http.end();
        return false;

        default:
        Serial.println("Unexpected HTTP code (strong FALSE - break)");
        Serial.print(http_code);
        Serial.print(HTTPClient::errorToString(http_code));
        http.end();
        return false;
    }

    String response = http.getString(); //THIS IS A BAD WAY TO HANDLE JSON FILES! NEED TO REPLACE! (good enough for now)

    Serial.println(response);

    http.end(); //closing the HTTP connection
    return true;
}


bool stop_lookup (String user_input) //The API allows to lookup (!!) stops. With proper architecture user will be able lookup his own stops 
{
    HTTPClient http;
    
    String url = String("https://realtime-api.trafiklab.se/v1/stops/name/") + user_input + "/?key=" + API_KEY;

    http.begin(url);
    
    int http_code = http.GET();

    int short_code = http_code/100;

    if (short_code >= 4)
    {
        Serial.println("HTTP request error");
        Serial.print(http_code);
        Serial.print(HTTPClient::errorToString(http_code));
        return false;
    }

    // TEMPORARY BLOCK HERE!!!     
    String response = http.getString(); //THIS IS A BAD WAY TO HANDLE JSON FILES! NEED TO REPLACE! (good enough for now)

    Serial.println(response);

    http.end(); //closing the HTTP connection
    return true;
    //END OF TEMPORARY BLOCK!!!
    
    
}