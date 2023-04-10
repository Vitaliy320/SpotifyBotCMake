#pragma once
#include <string>
#include "json/json.h"
#include "json/value.h"

Json::Value readJSON();

std::string parseJSON(Json::Value messages);