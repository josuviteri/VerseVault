#include <iostream>
#include <string>
#include <fstream>
#include "curl/curl.h"
#include "nlohmann/json.hpp"
#include "descargaArchivos.h"
#include <filesystem> // Para obtener la ruta del directorio actual
#include "../database/db2.h"
using json = nlohmann::json;
namespace fs = std::filesystem;

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *userp) {
    size_t realsize = size * nmemb;
    userp->append((char*)contents, realsize);
    return realsize;
}

std::string extractTxtUtf8Link(const std::string& jsonStr) {
    std::string txtUtf8Link = "";

    try {
        json j = json::parse(jsonStr);

        for (const auto& result : j["results"]) {
            auto formats = result["formats"];
            for (auto it = formats.begin(); it != formats.end(); ++it) {
                if (it.key() == "text/plain; charset=us-ascii") {
                    txtUtf8Link = it.value();
                    break;
                }
            }
            if (!txtUtf8Link.empty()) {
                break;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error al analizar JSON: " << e.what() << std::endl;
    }

    return txtUtf8Link;
}

std::string extractBookTitle(const std::string& jsonStr) {
    std::string title = "";

    try {
        json j = json::parse(jsonStr);
        title = j["results"][0]["title"];
    } catch (const std::exception& e) {
        std::cerr << "Error al analizar JSON: " << e.what() << std::endl;
    }

    return title;
}

void descargaArchivos(char* titulo, char* nombreAutor) {
    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if(curl) {
        std::stringstream urlBuilder;
        urlBuilder << "http://gutendex.com/books/?search=";

        // Formatear el autor
        for (int i = 0; nombreAutor[i]; i++) {
            if (nombreAutor[i] == ' ') {
                urlBuilder << "%20";
            } else {
                urlBuilder << nombreAutor[i];
            }
        }

        // Formatear el título
        urlBuilder << "%20";
        for (int i = 0; titulo[i]; i++) {
            if (titulo[i] == ' ') {
                urlBuilder << "%20";
            } else {
                urlBuilder << titulo[i];
            }
        }

        std::string url = urlBuilder.str();

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Desactivar la verificación del certificado SSL
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        // Seguir automáticamente las redirecciones
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            std::string txtUtf8Link = extractTxtUtf8Link(readBuffer);
            if (!txtUtf8Link.empty()) {
                std::cout << "Enlace de texto UTF-8: " << txtUtf8Link << std::endl;

                std::string bookTitle = extractBookTitle(readBuffer);
                if (!bookTitle.empty()) {
                    std::string directory = "../libros/";
                    std::string fileName = directory + bookTitle + ".txt"; // Ruta del archivo

                    // Crear la carpeta si no existe
                    std::filesystem::create_directories(directory);

                    // Leer el contenido del enlace y guardarlo en un archivo
                    curl_easy_setopt(curl, CURLOPT_URL, txtUtf8Link.c_str());
                    readBuffer.clear();  // Limpiar el buffer antes de la nueva solicitud
                    res = curl_easy_perform(curl);
                    if(res != CURLE_OK) {
                        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
                    } else {
                        // Abrir el archivo en modo binario para asegurar que los saltos de línea se mantengan
                        std::ofstream outputFile(fileName, std::ios::binary);
                        if (outputFile.is_open()) {
                            outputFile << readBuffer;
                            outputFile.close();
                            std::cout << "\nContenido guardado en " << fileName << std::endl;
                        } else {
                            std::cerr << "\nNo se pudo abrir el archivo para escritura." << std::endl;
                        }
                    }
                } else {
                    std::cerr << "\nNo se encontro el título del libro en el JSON." << std::endl;
                }
            } else {
                std::cerr << "\nNo se encontro el enlace de texto UTF-8." << std::endl;
            }
        }
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}