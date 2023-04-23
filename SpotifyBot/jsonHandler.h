#pragma once
#include <string>
#include "json/json.h"

Json::Value readJSON();

std::string parseJSON(Json::Value messages);