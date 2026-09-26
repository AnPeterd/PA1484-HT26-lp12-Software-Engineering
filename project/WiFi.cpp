
#include "WiFi.hpp"
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

