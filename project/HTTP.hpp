#pragma once
#include <HTTPClient.h>
#include "stops.hpp"

bool request_send(String stop_id);

bool stop_lookup (String user_input); // ONLY ASCII ALLOWED! (or I shoud write some url-refining function)