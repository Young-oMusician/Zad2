#include <iostream>
#include <string>
#include <memory>
#include <fstream>
#include "Sender.h"
#include "Receiver.h"
#include "COM.h"
#include "types.h"

using namespace std;

int main() {
    bytes data;
    cout << "WYBIERZ PORT:\n";
    string port;
    getline(cin,port);
    COM com(port);
    cout << "Wybierz funkcję:\n"
            "0) ODBIORNIK\n"
            "1) NADAJNIK\n";
    int mode;
    cin >> mode;
    if(mode == 0){
        Receiver odbiornik(make_shared<COM>(com));
        cout << "Wybierz alg:\n"
                "0) ALG\n"
                "1) CRC\n";
        int alg;
        cin >> alg;
        data = odbiornik.receive(alg);
        ofstream file(R"(D:\Bitner\Programowanko\C++\TELEKOMUNA\Zad2\test.txt)", ios::binary);
        if(file.good()){
            file.write((char*)data.data(),data.size());
        }
        else{
            cout << "Błąd przy ładowaniu pliku\n";
        }
        file.close();
    }
    else if(mode == 1){
        Sender nadajnik(make_shared<COM>(com));
        ifstream file(R"(D:\Bitner\Programowanko\C++\TELEKOMUNA\Zad2\test.txt)", ios::binary);
        if(file.good()){
            while(file){
                data.push_back(file.get());
            }
        }
        else{
            cout << "Błąd przy ładowaniu pliku\n";
        }
        file.close();
        nadajnik.send(data);
    }

    return 0;
}
