#include <iostream>
#include <boost/asio.hpp>
#include "server.h"
#include <string>
#include "config.h"
#include "crow.h"
#include <tgbot/tgbot.h>
#include <stdio.h>

using namespace boost::asio;
using namespace TgBot;

const int MAX_LENGTH = 1024;


void startTelegramBot() {
    /*string token(getenv("TOKEN"));
    printf("Token: %s\n", token.c_str());*/

    CurlHttpClient curlHttpClient;

    Bot bot(constants::telegramApiKey, curlHttpClient);
    bot.getEvents().onCommand("start", [&bot](Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Hi!");
        });
    bot.getEvents().onAnyMessage([&bot](Message::Ptr message) {
        printf("User wrote %s\n", message->text.c_str());
    if (StringTools::startsWith(message->text, "/start")) {
        return;
    }
    bot.getApi().sendMessage(message->chat->id, "Your message is: " + message->text);
        });

    signal(SIGINT, [](int s) {
        printf("SIGINT got\n");
    exit(0);
        });

    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        bot.getApi().deleteWebhook();

        TgLongPoll longPoll(bot);
        while (true) {
            printf("Long poll started\n");
            longPoll.start();
        }
    }
    catch (exception& e) {
        printf("error: %s\n", e.what());
    }
}

void startTelegramBot3() {
    TgBot::Bot bot(constants::telegramApiKey);
    std::cout << "Telegram bot started" << std::endl;


    bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Hi!");
        });

    bot.getEvents().onCommand("getHistory", [&bot](TgBot::Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "History");
        });

    bot.getEvents().onAnyMessage([&bot](TgBot::Message::Ptr message) {
        printf("User wrote %s\n", message->text.c_str());
    if (StringTools::startsWith(message->text, "/start") || StringTools::startsWith(message->text, "/getHistory")) {
        return;
    }

    std::string chatDescription = bot.getApi().getChat(message->chat->id)->description;

    bot.getApi().sendMessage(message->chat->id, "Your message is: " + message->text);
        });

    try {
        TgBot::TgLongPoll longPoll(bot);
        while (true) {
            longPoll.start();
        }
    }

    catch (TgBot::TgException& e) {
        printf("error: %s\n", e.what());
    }
}

//
//void startServer2() {
//    std::thread thread_obj(startTelegramBot);
//
//    int port{ 8081 };
//
//    struct mg_mgr mgr;
//    struct mg_connection* nc;
//    void* fn_data{ NULL };
//    const char* fmt{ NULL };
//
//    std::string portToChar{ std::to_string(port) };
//    static char const* sPort{ portToChar.c_str() };
//
//    mg_mgr_init(&mgr);
//    mg_event_handler_t handler{ ev_handler };
//    nc = mg_ws_connect(&mgr, sPort, handler, fn_data, fmt);
//    if (nc == NULL) {
//        std::cout << "Failed to create listener" << std::endl;
//        return;
//    }
//
//    //mg_set_protocol_http_websocket(nc);
//    mg_http_serve_dir(nc, hm, &s_http_server_opts);
//
//    for (;;) {
//        mg_mgr_poll(&mgr, 1000);
//    }
//
//    mg_mgr_free(&mgr);
//}


void startServer() {
    std::thread thread_obj(startTelegramBot);

    crow::SimpleApp app;
    std::cout << "Server started" << std::endl;

    CROW_ROUTE(app, "/")([]() {
        return "Hello world";
    });

    CROW_ROUTE(app, "/redirect")([]() {
        return "You'll be redirected";
    });

    app.port(8080).run();
}