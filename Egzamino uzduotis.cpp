#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <cctype>
#include <locale>
#include <iomanip>
#include <algorithm>

// Sukuriame set'ą TDL'ams saugoti
std::set<std::string> tlds;

// Funkcija nuskaitanti TDL'us iš dokumento
void loadTLDs(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        tlds.insert(line);
    }
}


// Funkcija tikrinanti ar eilutė yra hyperlinkas ir surandanti URL bazę
bool arHyperlink(std::string& zodis) {
    // Ieškome URL būdingų dalių
    int protocol_end = zodis.find("://");
    // Tikriname, ar yra nurodytas protokolas (pvz., "http://")
    if (protocol_end != std::string::npos) {
        // Ieškome pirmo pasvyrojo brūkšnio
        int pirmas_slash = zodis.find('/', protocol_end + 3);
        // Jei randame pirmą pasvyrajį brūkšnį grąžiname tai kaip mūsų URL
        if (pirmas_slash != std::string::npos) {
            zodis = zodis.substr(0, pirmas_slash);
            return true;
        }
    }

    // Tikriname, ar zodis baigiasi su žinomu TDL
    size_t pos = zodis.find_last_of('.');
    if (pos != std::string::npos && pos < zodis.length() - 1) {
        std::string domain = zodis.substr(pos + 1);
        if (std::any_of(domain.begin(), domain.end(), ::isalpha) && tlds.find(domain) != tlds.end() && zodis.find("www.") == 0) {
            return true;
        }
    }

    return false;
}


// Funkcija, kuri panaikina netinkamus simbolius iš žodžio (skaičius)
std::string NetinkamasSimbolis(const std::string& zodis) {
    std::string result;
    for (char ch : zodis) {
        if (std::isalpha(ch)) {
            result += ch;
        }
    }
    return result;
}

int main() {
    // Nustatome locale kad būtų palaikomi simboliai
    std::locale::global(std::locale(""));

    loadTLDs("tld-list-basic.txt");

    // Atidarome dokumentą kurį norime nuskaityti
    std::ifstream inputFile("Kaunas.txt");

    // Tikriname ar dokumentas atidarytas
    if (!inputFile.is_open()) {
        std::cerr << "Klaida atveriant nuskaitomąjį failą!" << std::endl;
        return 1;
    }

    // Sukuriame set'ą hyperlink'ams saugoti
    std::set<std::string> hyperlinkai;

    // Sukuriame map'ą informacijai apie žodį saugoti
    std::map<std::string, std::pair<int, std::multiset<int>>> zodisInfo;

    // Nuskaitome kiekvieną eilutę iš dokumento
    std::string eilute;
    int eilutes_nr = 1;
    while (std::getline(inputFile, eilute)) {
        // Eilutė konvertuojama į žodžius
        std::istringstream iss(eilute);
        std::string zodis;

        while (iss >> zodis) {
            // Tikriname, ar žodis yra hyperlinkas
            if (arHyperlink(zodis)) {
                hyperlinkai.insert(zodis);
            }
            else {
                // Pašaliname netinkamus simbolius iš žodžio
                std::string geras_zodis = NetinkamasSimbolis(zodis);

                // Padarome, kad žodis būtų iš mažųjų raidžių
                std::transform(geras_zodis.begin(), geras_zodis.end(), geras_zodis.begin(), ::tolower);

                // Patikriname, ar žodis nėra skaičius
                if (!geras_zodis.empty()) {
                    auto& info = zodisInfo[geras_zodis];
                    ++info.first; // pakeičiame žodžio pasikartojimo skaičių
                    info.second.insert(eilutes_nr);
                }
            }
        }

        ++eilutes_nr;
    }

    // Uždarome nuskaitomąjį dokumentą
    inputFile.close();

    // Atidarome dokumentą, į kurį bus įrašomi duomenys
    std::ofstream outputFile("rezultatas.txt");

    // Patikriname, ar dokumentas atidarytas
    if (!outputFile.is_open()) {
        std::cerr << "Klaida atveriant failą: rezultatas.txt!" << std::endl;
        return 1;
    }

    // Išvedame duomenis
    outputFile << std::left << std::setw(20) << "Žodis" << std::setw(15) << "Pasikartojimų sk." << std::endl;
    outputFile << std::string(30, '-') << std::endl;

    for (const auto& pair : zodisInfo) {
        // Išvedame tik tuos žodžius, kurie pasikartojo daugiau nei 1 kartą
        if (pair.second.first > 1) {
            outputFile << std::left << std::setw(20) << pair.first << std::setw(15) << pair.second.first << std::endl;
        }
    }

    // Uždarome dokumentą
    outputFile.close();

    // Atidarome dokumentą, į kurį vesime hyperlinks
    std::ofstream hyperlinksFile("hyperlinkai.txt");

    // Patikriname, ar dokumentas atidarytas
    if (!hyperlinksFile.is_open()) {
        std::cerr << "Klaida atveriant failą: hyperlinkai.txt!" << std::endl;
        return 1;
    }

    // Išvedame hyperlinks į dokumentą
    hyperlinksFile << "Hyperlink'ai:" << std::endl;
    hyperlinksFile << std::string(30, '-') << std::endl;
    for (const auto& link : hyperlinkai) {
        hyperlinksFile << link << std::endl;
    }

    // Uždarome dokumentą
    hyperlinksFile.close();

    // Atidarome dokumentą, kuriame kursime cross-reference lentelę
    std::ofstream crossReferenceFile("cross_reference.txt");

    // Patikriname, ar dokumentas atidarytas
    if (!crossReferenceFile.is_open()) {
        std::cerr << "Klaida atveriant failą: cross_reference.txt!" << std::endl;
        return 1;
    }

    // Išvedame duomenis
    crossReferenceFile << std::left << std::setw(15) << "Žodis" << std::setw(20) << "Pasikartojimų sk." << std::setw(20) << "Eilučių numeriai" << std::endl;
    crossReferenceFile << std::string(100, '-') << std::endl;

    for (const auto& pair : zodisInfo) {
        // Išvedame tik žodžius, kurie pasikartojo daugiau nei 1 kartą
        if (pair.second.first > 1) {
            crossReferenceFile << std::left << std::setw(15) << pair.first << std::setw(20) << pair.second.first;
            for (int eilute : pair.second.second) {
                crossReferenceFile << eilute << " ";
            }
            crossReferenceFile << std::endl;
        }
    }

    // Uždarome dokumentą
    crossReferenceFile.close();

    std::cout << "Duomenys įrašyti į dokumentus: rezultatas.txt, cross_reference.txt, hyperlinkai.txt";

    return 0;
}
