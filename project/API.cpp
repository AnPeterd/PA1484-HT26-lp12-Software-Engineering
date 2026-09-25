
#include "API.hpp"
#include "secrets.h"

void wifi_reboot()
{
            WiFi.disconnect(true);
            WiFi.mode(WIFI_OFF);

            delay(100); // this can be changed if needed

            WiFi.mode(WIFI_STA);
            WiFi.begin(WIFI_SSID, WIFI_PASSWORD); //

            delay (1000); //giving some extra time
}
void wifi_shutdown()
{
   WiFi.disconnect(true);
   WiFi.mode(WIFI_OFF); //for now this is enough
}

void troubleshoon_network(int8_t code) //auto-troubleshhoting function for WiFi
{
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("No longer in need of torubleshooting");
        return;
    }
    else
    {
        wl_status_t current_status = WiFi.status();
        for (int i = 1; i <4 && current_status != WL_CONNECTED; i++)
        {
            switch (current_status)
            {
                case (WL_IDLE_STATUS): // first toubleshooting for this one is simply waitnig
                {
                    const uint32_t start = millis(); //potential problem here!
                    while (WiFi.status() == WL_IDLE_STATUS && (millis() - start) < i*2000) {
                        delay(i*100);//progressivly longer waitig time
                    }
                    if (WiFi.status() == WL_IDLE_STATUS && i>2)
                    {
                        wifi_reboot(); //rebooting of waiting did not work
                    }
                    break;
                }

                    //some other statuses are going to be here
                default:
                wifi_reboot();// for rare conditions - the only seafe option is rebooting
            }

            current_status = WiFi.status();
        }

    }


    
}

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
        Serial.println ("Code 1xx: Informational response (weak FALSE - continues)");
        Serial.print(http_code);
        break;

        case (2):
        Serial.println("Code 2xx: Success (TRUE)");
        Serial.print(http_code);
        break;

        case (3):
        Serial.println ("Code 3xx: Redirection (weak FALSE - continues)");
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