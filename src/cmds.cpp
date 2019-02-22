#include "cmd.h"
#include "common.h"
#include "events.h"
#include "fs.h"
#include "str.h"

using namespace std;

#include <stdio.h>
string getParamOfPath(string path, string p)
{
    FILE* f = fopen(path.c_str(), "r");
    char buffer[4096];
    size_t bytes_read;
    bytes_read = fread(buffer, 1, sizeof(buffer), f);
    string dat(buffer, bytes_read);
    args_t lines = str::words(dat, '\n');
    //cout << lines[0] << endl;
    for (auto line : lines) {
        args_t param = str::words(line, ':');
        if (str::at(param[0], p))
            return str::replase(param[1], "  ", " ");
    }
    return "";
}

#include "../version.h"
#include <chrono>
void test(cmdHead)
{
    std::chrono::time_point<std::chrono::system_clock> begin, end;
    begin = std::chrono::system_clock::now();
    eventOut->net->send("http://api.vk.com");
    end = std::chrono::system_clock::now();
    unsigned int t = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    eventOut->msg += "http://github.com/EVGESHAd/vk-cpp-bot (" + string(FULLVERSION_STRING) + ")\n";
    eventOut->msg += "Обработка VK API за: " + to_string(t) + "мс\n";
    eventOut->msg += "id чата (пользователь/чат): " + to_string(eventIn->from_id) + "/" + to_string(eventIn->peer_id) + "\n";

    //получаем использование памяти
    string allMem = to_string((int)((float)str::fromString(getParamOfPath("/proc/meminfo", "MemTotal")) / 1024));
    string usedMem = to_string((int)((float)(str::fromString(getParamOfPath("/proc/meminfo", "MemTotal")) - str::fromString(getParamOfPath("/proc/meminfo", "MemAvailable"))) / 1024));
    string myMem = to_string((int)((float)str::fromString(getParamOfPath("/proc/self/status", "VmRSS")) / 1024));

    eventOut->msg += "CPU:" + getParamOfPath("/proc/cpuinfo", "model name") + "\n";
    eventOut->msg += "RAM: " + usedMem + "/" + allMem + " Мб\n";
    eventOut->msg += "Я сожрал оперативы: " + myMem + " Мб\n";
    eventOut->msg += "Потоков занял: " + getParamOfPath("/proc/self/status", "Threads") + "\n\n";

    eventOut->msg += "Всего сообщений от тебя: " + std::to_string(eventOut->user.msgs) + "\n";
}

void con(cmdHead)
{
    char buffer[512];
#ifdef __linux__
    args_t commands = str::words(str::summ(eventIn->words, 1), '\n');
    string c = "";
    for (auto command : commands)
        c += command + " 2>&1\n";
#elif _WIN32
    string c = str::summ(eventIn->words, 1);
#endif

    FILE* pipe = popen(c.c_str(), "r");
    while (fgets(buffer, sizeof buffer, pipe) != NULL) {
        eventOut->msg = buffer;
        eventOut->send();
    }
    eventOut->msg = "done!";
    pclose(pipe);
}
