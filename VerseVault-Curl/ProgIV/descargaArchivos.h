
#ifndef PROGIV_DESCARGAARCHIVOS_H
#define PROGIV_DESCARGAARCHIVOS_H


#include <iostream>
#include <string>
#include <fstream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>



static size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *userp);

std::string extractTxtUtf8Link(const std::string& jsonStr);

std::string extractBookTitle(const std::string& jsonStr);

void descargaArchivos();




#endif //PROGIV_DESCARGAARCHIVOS_H
