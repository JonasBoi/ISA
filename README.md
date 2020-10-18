# Projekt do předmětu ISA (FIT VUT)
## Programování síťové služby - filtrujici DNS resolver
**Jméno:** Jonáš Sasín  
**Login:** xsasin05  
**Datum:** 18.10.2020  
**Jazyk**: C/C++  

"Program dns, který bude filtrovat dotazy typu A směřující na domény v rámci dodaného seznamu a jejich poddomény. Ostatní dotazy bude přeposílat v nezměněné podobě specifikovanému resolveru. Odpovědi na dříve přeposlané dotazy bude program předávat původnímu tazateli."

# Spuštění
Program se přeloží příkazem "make"  
a spustí příkazem  
  
**./dns -s server [-p port] -f filter_file**
  
Pro podrobnosti k možnostem spuštění: ./dns --help

# Odevzdané soubory
* src - adresář se zdrojovými soubory
    * dns.cpp
    * createSock.cpp / hpp
    * paramParse.cpp / hpp
    * parseDns.cpp / hpp
    * dnsPacketStruct.hpp
* Makefile - přeložení zdrojového kódu
* manual.pdf - dokumentace řešení projektu
* README - tento soubor - informace ke spuštění  

## Popis řešení
-> manual.pdf

## Návratové kódy
**0** - špatně zadané parametry programu, volání --help, soubor obsahující nežádoucí domény se nepodařilo otevřít

**1** - chyba při tvorbě socketů
