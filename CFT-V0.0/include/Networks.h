#include "CTBot.h"
#include <ArduinoJson.h>

String ssid = "Singkong Krispy";
String pass = "kibar899";
String token = "6683866627:AAFJtxUiYUy6BBN4_HKj2HiqW7CAGXoT1Bg";
int id = 784481491, hari = 0;
CTBot myBot;
TBMessage msg;

void netwoksBot()
{
    if (myBot.getNewMessage(msg))
    {
        if (msg.text.equalsIgnoreCase("/cekhari"))
        {
            myBot.sendMessage(msg.sender.id, "Total hari budidaya: " + String(hari));
        }

        if (msg.text.equalsIgnoreCase("/cekjamkuras"))
        {
            myBot.sendMessage(msg.sender.id, "Pengurasan Harian Pukul 08:00 WIB sebesar 10%");
            myBot.sendMessage(msg.sender.id, "Pengurasan Mingguan Pukul 07:00 WIB sebesar 80%");
        }

        if (msg.text.equalsIgnoreCase("/cekjampakan"))
        {
            myBot.sendMessage(msg.sender.id, "Pemberian pakan sehari 3x pada jam 09:00 WIB, 15:00 WIB, 21:00 WIB, dan opsional untuk jam 03:00 WIB");
        }
    }
}