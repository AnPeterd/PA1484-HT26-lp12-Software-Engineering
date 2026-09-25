#pragma once
#include <WiFi.h>
#include <HTTPClient.h>
#include <time.h>

bool request_send(String stop_id);

void troubleshoon_network(int8_t code);

void wifi_shutdown();

void wifi_reboot();