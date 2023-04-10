#include <iostream>
#include <string>
#include <curl/curl.h>
#include "SpotifyAPIHandler.h"
#include "config.h"
#include "json/json.h"


using namespace constants;

size_t  writeCallback(char* buf, size_t size, size_t nmemb, void* up) {
    // callback must have this signature
    // this writes the response from the server
    // to the stringstream
    ((std::string*)up)->append((char*)buf, size * nmemb);
    return size * nmemb;
}

int deserialiseResponse(std::string response) {
    Json::Value root;
    Json::Reader reader;

    try {
        reader.parse(response, root);
        Json::Value error{ root.get("error", "") };
        return error.get("status", "").asInt();
    }
    catch (exception) {
        return 0;
    }
}

std::string getNewAccessToken() {
    return "";
}

void addTrack() {
    CURL* curl;
    CURLcode  res;
    std::string json_data = "{\"uris\": [\"spotify:track:05fXnwkg4iQoLWGOuZWV5n\"]}";
    curl_slist* headers{ NULL };
    std::string readBuffer;

    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, ("Authorization: Bearer " + constants::spotifyAccessToken).c_str());

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, ("https://api.spotify.com/v1/playlists/" + constants::playlist_id + "/tracks").c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);

        int status{ deserialiseResponse(readBuffer) };
        curl_easy_cleanup(curl);

        if (status == 401) {

        }


    }
}

//=================================================================================================================================================

//void authenticate() {
//    // Prepare the request body
//    std::string body = "grant_type=client_credentials&client_id=" + constants::spotifyClientId + "&client_secret=" + constants::spotifyClientSecret;
//
//    // Initialize cURL
//    CURL* curl = curl_easy_init();
//    if (!curl) {
//        std::cerr << "Error initializing cURL" << std::endl;
//        return;
//    }
//
//    // Set cURL options
//    curl_easy_setopt(curl, CURLOPT_URL, "https://accounts.spotify.com/api/token");
//    curl_easy_setopt(curl, CURLOPT_POST, 1);
//    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);
//
//    // Perform the request
//    CURLcode res = curl_easy_perform(curl);
//    if (res != CURLE_OK) {
//        std::cerr << "Error performing cURL request: " << curl_easy_strerror(res) << std::endl;
//        return;
//    }
//
//    // Parse the JSON response
//    Json::Value json;
//    Json::Reader reader;
//    if (!reader.parse(response, json)) {
//        std::cerr << "Error parsing JSON response" << std::endl;
//        return;
//    }
//
//    // Extract the access token from the JSON response
//    access_token = json["access_token"].asString();
//}