# Arduino Projekt

## Anotace

Cílem této práce je implementovat program pro Arduino, který bude řídit dva semafory pro auta a dva semafory pro chodce na křižovatce. Program musí zajistit správné střídání semaforů pro auta a synchronizaci semaforů pro chodce s opačnými semafory pro auta. Dále musí být implementován noční režim, ve kterém budou semafory pro auta blikat oranžově a semafory pro chodce budou vypnuté. Program musí také logovat důležité události do sériového monitoru.

## Zadání

### Semafory pro auta

1. Každý semafor pro auta má tři světla: červené, oranžové a zelené.
2. Semafory pro auta se střídají v činnosti. Když je jeden semafor v činnosti (zelené světlo), druhý semafor musí být na červené.
3. Po skončení zeleného světla na jednom semaforu musí následovat oranžové světlo, než se přepne na červené a druhý semafor začne svou činnost.

### Semafory pro chodce

1. Každý semafor pro chodce má dvě světla: červené a zelené.
2. Každý semafor pro chodce má tlačítko, které chodci mohou stisknout, aby požádali o přechod.
3. Semafory pro chodce jsou synchronizovány s opačným semaforem pro auta. Když je semafor pro auta v činnosti, semafor pro chodce na opačné straně může být také v činnosti (zelené světlo), pokud bylo stisknuto tlačítko.
4. Pokud tlačítko není stisknuto, semafory pro chodce zůstávají na červené (pokud není aktivován noční režim).

### Noční režim

1. Program musí mít tlačítko pro aktivaci nočního režimu.
2. V nočním režimu budou semafory pro auta blikat oranžově a semafory pro chodce budou vypnuté.

### Časování

1. Doba trvání červeného světla: 5 sekund.
2. Doba trvání oranžového světla: 1 sekunda.
3. Doba trvání zeleného světla: 10 sekund.
4. Doba trvání červeného světla pro chodce: 15 sekund.
5. Čekací doba po stisknutí tlačítka pro chodce: 2 sekundy.

### Požadavky na hardware

1. Semafor pro auta 1: červené světlo (pin 2), oranžové světlo (pin 3), zelené světlo (pin 4).
2. Semafor pro chodce 1: červené světlo (pin 5), zelené světlo (pin 6), tlačítko (pin 7).
3. Semafor pro auta 2: červené světlo (pin 8), oranžové světlo (pin 9), zelené světlo (pin 10).
4. Semafor pro chodce 2: červené světlo (pin 11), zelené světlo (pin 12), tlačítko (pin 13).
5. Tlačítko pro noční režim: pin 14.

### Logování

1. Program musí logovat důležité události do sériového monitoru, aby bylo možné sledovat správnou funkčnost programu.