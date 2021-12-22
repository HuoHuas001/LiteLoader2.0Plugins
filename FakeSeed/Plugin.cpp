#include "pch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <LoggerAPI.h>
using namespace std;

#define CONF_PATH "./plugins/FakeSeed/seed.txt"
int seed = -2138562307;
Logger fakeseed("FakeSeed");
void entry()
{
    try
    {
        if (!filesystem::exists(CONF_PATH))
        {
            filesystem::create_directories("./plugins/FakeSeed");
            ofstream fout(CONF_PATH);
            fout << seed << endl;
        }
        else
        {
            ifstream fin(CONF_PATH);
            if (fin)
                fin >> seed;
        }
    }
    catch (...) {}
    fakeseed.info("FackSeed Loaded.");
    fakeseed.info("Seed is set to {}",seed);
}

THook(void*, "?write@StartGamePacket@@UEBAXAEAVBinaryStream@@@Z", void* a, void* b)
{
    dAccess<int, 48>(a) = seed;
    return original(a, b);
}