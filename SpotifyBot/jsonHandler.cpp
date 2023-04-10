#include "jsonHandler.h"
#include <fstream>
#include "config.h"
#include <regex>

using namespace std;


Json::Value readJSON(){
    std::ifstream jsonFile("D:\\Programming\\C++\\Projects\\SpotifyBot_\\chatHistory\\result.json");

    Json::Value messages;

    if (jsonFile) {
        jsonFile >> messages;
        std::cout << messages;
    }
    return messages;
}

std::string parseJSON(Json::Value messagesJson) {
    Json::FastWriter fastWriter;

    std::vector<std::string> messages;
    std::vector<std::string> songLinks;
    std::string messageText;
    std::string messageTextLowercase;
    std::regex urlRegex("\\b((?:https?|ftp|file)://[-a-zA-Z0-9+&@#/%?=~_|!:, .;]*[-a-zA-Z0-9+&@#/%=~_|])");
    std::string spotifySubstring("spotify");
    std::smatch match;

    for (Json::Value message : messagesJson["messages"]) {
        try {
            messageText = fastWriter.write(message["text"]);
            messageTextLowercase = messageText;

            std::transform(messageTextLowercase.begin(), messageTextLowercase.end(), messageTextLowercase.begin(),
                [](unsigned char c) { return std::tolower(c); });

            if (std::regex_search(messageText, match, urlRegex)) {
                if (match.size() > 0 && messageTextLowercase.find(spotifySubstring) != std::string::npos) {
                    songLinks.push_back(match[0]);
                }
            }
        }
        catch (...) {

        }

    }
    return "";
}
