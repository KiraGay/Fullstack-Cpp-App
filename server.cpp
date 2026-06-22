#include "httplib.h"
#include "json.hpp"
#include <iostream>
#include <string>

using json = nlohmann::json;

int main() {
    httplib::Server svr;

    // SOLUSI MUTLAK CORS: Menempelkan izin pada SETIAP respons yang keluar
    svr.set_post_routing_handler([](const httplib::Request&, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, GET, PUT, OPTIONS, PATCH, DELETE");
        res.set_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
    });

    // Wajib ada untuk merespons ketukan pintu pertama dari browser
    svr.Options(".*", [](const httplib::Request& req, httplib::Response& res) {
        res.status = 200; // Cukup jawab "OK"
    });

    // Endpoint POST
    svr.Post("/api/kirim", [](const httplib::Request& req, httplib::Response& res) {
        try {
            json data_masuk = json::parse(req.body);
            std::string nama = data_masuk["nama"];

            json balasan;
            balasan["status"] = "Berhasil";
            balasan["pesan"] = "Mantap! C++ sudah terima data dari: " + nama;

            res.set_content(balasan.dump(), "application/json");
        } catch (const std::exception& e) {
            res.status = 400; 
            res.set_content("{\"status\": \"Error\", \"pesan\": \"Format data salah\"}", "application/json");
        }
    });

    std::cout << "Server C++ berjalan di http://127.0.0.1:8080..." << std::endl;
    svr.listen("0.0.0.0", 8080);
    return 0;
}