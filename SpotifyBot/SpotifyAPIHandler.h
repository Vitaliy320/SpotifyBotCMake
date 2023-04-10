#pragma once

size_t  writeCallback(char* buf, size_t size, size_t nmemb, void* up);

void addTrack();

int deserialiseResponse(std::string response);

std::string getNewAccessToken();

void authenticate();