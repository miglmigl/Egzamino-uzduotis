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

// Tikriname ar dalis atitinka lietuvišką simbolį UTF-8
bool isLithuanianSymbol(const std::string& bytes) {
    static const std::set<std::string> lithuanianSymbols = {
        "\xC4\x85", // ą
        "\xC4\x8D", // č
        "\xC4\x99", // ę
        "\xC4\x97", // ė
        "\xC4\xAF", // į
        "\xC5\xA1", // š
        "\xC5\xB3", // ų
        "\xC5\xAB", // ū
        "\xC5\xBE"  // ž
    };

    return lithuanianSymbols.find(bytes) != lithuanianSymbols.end();
}


   // Funkcija randa žodžius, kuriuose yra "aik"
// Funkciją "raskAik" ieško ir grąžina rinkinį (set) žodžių, turinčių "aik" dalį.
std::set<std::string> raskAik(const std::map<std::string, std::pair<int, std::multiset<int>>>& zodisInfo) {

    // Sukuriame tuščią rinkinį "zodziai_su_Aik", kuriame saugosime žodžius, atitinkančius kriterijus.
    std::set<std::string> zodziai_su_Aik;
    for (const auto& zodis : zodisInfo) 
    {
        if (zodis.first.find("aik") != std::string::npos) 
        {
            zodziai_su_Aik.insert(zodis.first);
        }
    }
    return zodziai_su_Aik;
}



//Funkcija naikinanti netinkamus simbolius iš žodžio
std::string NetinkamasSimbolis(const std::string& zodis) {
    std::string result;
    for (size_t i = 0; i < zodis.length();) {
        if (std::isalpha(zodis[i]) || (i + 1 < zodis.length() && isLithuanianSymbol(zodis.substr(i, 2)))) {
            result += zodis[i++];
            if (!std::isalpha(zodis[i - 1])) {
                result += zodis[i++]; // Pridedame kitą UTF-8 dalį
            }
        }
        else {
            ++i; // Praleidžiame ne abėcėles elementus
        }
    }
    return result;
}

// Funkcija apskaičiuojanti žodžiui reikalingą plotį
int displayWidth(const std::string& str) {
    int width = 0;
    for (size_t i = 0; i < str.length(); ++i) {
        if ((str[i] & 0xC0) != 0x80) { // Jei ne neuoseklumo dalis UTF-8
            ++width;
        }
    }
    return width;
}

std::set<std::string> tlds;

// Funkcija nuskaitanti TDL'us iš dokumento
void loadTLDs(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        tlds.insert(line);
    }
}






// Funkcija tikrinanti ar eilutė yra hyperlinkas
bool arHyperlink(std::string& zodis, std::set<std::string>& hyperlinkai) {
    // ieskom zinomu url daliu
    if (zodis.find("http://") == 0 || zodis.find("https://") == 0 || zodis.find("www.") == 0) {
        hyperlinkai.insert(zodis); // pridedam prie hyperlinku
        return true;
    }

    // tikriname ar baigiasi su zinomu TLD
    size_t pos = zodis.find_last_of('.');
    if (pos != std::string::npos) {
        std::string domain = zodis.substr(pos + 1);
        if (tlds.find(domain) != tlds.end()) {
            hyperlinkai.insert(zodis); // pridedam prie hyperlinku
            return true;
        }
    }

    return false;
}




int main() {

    std::locale::global(std::locale("en_US.UTF-8"));
    loadTLDs("tld-list-basic.txt");

    std::ifstream inputFile("Kaunas.txt", std::ios::in | std::ios::binary);
    if (!inputFile.is_open()) {
        std::cerr << "Klaida atveriant nuskaitomąjį failą!" << std::endl;
        return 1;
    }

    std::set<std::string> hyperlinkai;
    std::map<std::string, std::pair<int, std::multiset<int>>> zodisInfo;

    // Nuskaitome kiekvieną eilutę iš dokumento
    std::string eilute;
    int eilutes_nr = 1;
    while (std::getline(inputFile, eilute)) {
        // Eilutė konvertuojama į žodžius
        std::istringstream iss(eilute);
        std::string zodis;

     


        while (iss >> zodis) {
            if (!arHyperlink(zodis, hyperlinkai)) {
                // Pašaliname netinkamus simbolius iš žodžio
                std::string geras_zodis = NetinkamasSimbolis(zodis);

                // Padarome, kad žodis būtų iš mažųjų raidžių
                std::transform(geras_zodis.begin(), geras_zodis.end(), geras_zodis.begin(), ::tolower);

                // Patikriname, ar žodis nėra skaičius ir nėra tuščias
                if (!geras_zodis.empty()) {
                    auto& info = zodisInfo[geras_zodis];
                    ++info.first; // Pakeičiame žodžio pasikartojimo skaičių
                    info.second.insert(eilutes_nr); // Įrašome eilutės numerį
                }
            }
        }

        ++eilutes_nr; // Prieš pradedant skaityti kitą eilutę, padidiname eilutės numerį

    }

    inputFile.close();




    // Atidarome 'rezultatas.txt'
    std::ofstream outputFile("rezultatas.txt", std::ios::out | std::ios::binary);
    if (!outputFile.is_open()) {
        std::cerr << "Klaida atveriant failą: rezultatas.txt!" << std::endl;
        return 1;
    }

    // Rašome antrąštę 'rezultatas.txt'
    outputFile << "Žodis                  Pasikartojimų sk.\n";
    outputFile << std::string(40, '-') << "\n";

    // Pradedame ciklą, kuris eina per visus žodžius ir jų pasikartojimų informaciją iš žodyno "zodisInfo".
    for (const auto& pair : zodisInfo) {
        if (pair.second.first > 1) {
            // Apdorojame žodį, pašalinant iš jo neleistinus simbolius.
            std::string processedWord = NetinkamasSimbolis(pair.first);

            // Skaičiuojame žodžio "processedWord" ekrano plotį, atsižvelgiant į UTF-8 koduotę.
            int actualWidth = displayWidth(processedWord);

            // Koreguojame eilutės plotį, atsižvelgiant į tai, kad kai kurios raidės gali būti užkoduotos kaip kelios baitų sekos.
            int adjustedWidth = 20 - (actualWidth - processedWord.length());

            // Įrašome žodį ir jo pasikartojimų skaičių į failą, naudodami kairėje pusėje lygiuojamą formatavimą ir nustatytą plotį.
            outputFile << std::left << std::setw(adjustedWidth) << processedWord << std::setw(15) << pair.second.first << "\n";
        }
    }

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

    // Atidarome 'cross_reference.txt'
    std::ofstream crossReferenceFile("cross_reference.txt");
    if (!crossReferenceFile.is_open()) {
        std::cerr << "Klaida atveriant failą: cross_reference.txt!" << std::endl;
        return 1;
    }

    // Rašome antrąštę 'cross_reference.txt'
    crossReferenceFile << "Žodis              Pasikartojimų sk.  Eilučių numeriai\n";
    crossReferenceFile << std::string(60, '-') << "\n";

    for (const auto& pair : zodisInfo) {
        if (pair.second.first > 1) {
            std::string processedWord = NetinkamasSimbolis(pair.first);
            int actualWidth = displayWidth(processedWord);
            int adjustedWidth = 20 - (actualWidth - processedWord.length());
            crossReferenceFile << std::left << std::setw(adjustedWidth) << processedWord << std::setw(20) << pair.second.first;
            for (int eilute : pair.second.second) {
                crossReferenceFile << eilute << " ";
            }
            crossReferenceFile << "\n";
        }
    }

    crossReferenceFile.close();


    // kvieciame funkciją zodziu radimui su "aik"
    auto zodziai_su_Aik = raskAik(zodisInfo);

    // Žodžių su "aik" išvedimas į dokumentą
    std::ofstream out("SuAIK.txt");
    out << "Žodžiai turintys 'aik':\n" << "----------------------------------------\n";
    for (const auto& zodis : zodziai_su_Aik) {
        out << zodis << "\n";
    }
    out.close();


    std::cout << "Duomenys įrašyti į dokumentus: rezultatas.txt, cross_reference.txt, hyperlinkai.txt, SuAIK.txt";

    return 0;
}