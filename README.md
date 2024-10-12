### Copyright Manolache Maria-Catalina 313CAb 2023-2034

**Image editor in C pentru imagini de tip PPM si PGM:**
Scopul programului este de a realiza un editor photo ale carui functionalitati
de baza cuprind incarcarea in memorie a unei imagini, selectarea unei sectiuni
din imagine, decuparea imaginilor, aplicarea diverselor filtre etc.

Programul contine la inceput 7 functii elementare folosite pe parcursul
rezolvarii cerintelor, care vor fi explicate in randurile ce urmeaza, precum
si structurile folosite.

## Structuri:
In program voi folosi structura PHOTO, ce contine dimensiunile
pozei, precum si valoarea maxima a pixelilor (pentru pozele grayscale si RGB),
magic word-ul ce identifica tipul pozei, coordonatele selectiei curente si o
matrice de tipul MAT.
Structura MAT contine 3 variabile de tip int, ce vor reprezenta pixelii pentru
toate tipurile de poze. Pentru alb-negru si grayscale, singurul canal de 
culoare va fi reprezentat de campul .r, iar pozele RGB vor folosi .r .g .b.

## Functii elementare:
## clamp()
Functia clamp() incadreaza un anumit numar in intervalul [0,255]. Aceasta
va fi utilizata in cadrul operatiilor APPLY si EQUALIZE.
## allocate()
Functia allocate() primeste ca parametrii dimensiunile unei matrice de alocat
si auxiliarul in care aceasta va fi alocata, folosind calloc(). Am abordat
cazul in care alocarea esueaza, dandu-i auxiliarului NULL si incheiand executia
functiei. Asemanator, daca in cadrul alocarii fiecarui pointer spre un bloc de
coloane, aceasta esueaza, voi elibera tot ce am alocat pana in acest
punct, voi da auxiliarului NULL si voi incheia executia functiei.
## free_mat()
Functia free_mat() elibereaza o matrice oarecare de tipul MAT, in functie de 
numarul ei de linii.
## swap()
Functia swap() inverseaza valorile a doua variabile.
## word_number()
Functia word_number() va returna numarul de "cuvinte" desparite prin spatii
dintr-o comanda data ca parametru, folosind strtok().
## select_token()
Functia select_tokens() va fi folosita pentru a transforma inputul in
variabilele de selectie. Aceasta verifica daca tokenul citit se poate 
transforma intr-un numar. Daca tokenul este egal cu 0 (caracterul) si
functia atoi returneaza tot 0, inseamna ca tokenul este egal cu 0 (numarul).
Daca insa tokenul este diferit de 0 (caracterul) si atoi returneaza 0,
inseamna ca nu se poate transforma in numar, caz in care se
returneaza 1. Altfel, se poate converti, numarul fiind diferit de 0.
## FILE_TYPE()
Functia FILE_TYPE() returneaza tipul fisierului din care se va face LOAD.
Initial, presupun ca fisierul e binar (b), incerc sa-l deschid, iar daca
deschiderea va esua, voi tipari mesajul corespunzator, voi elibera variabila ce
stocheaza numele fisierului si voi returna tipul f (fail), adica deschiderea
a esuat. In caz contrar, citesc magic word-ul si verific daca acesta corespunde
fisierelor text, returnand t (text) in acest caz.

## Functii principale:
## LOAD
Functia LOAD() incarca in memorie matricea, bazandu-se pe tipul returnat
de functia FILE_TYPE(). Initial, se elibereaza matricea incarcata anterior
in memorie. Apoi, in functie de tip, se realizeaza operatia de LOAD.
Pentru 'f', se incheie executia programului. Pentru 't', se deschide fisierul
de unde se va citi, verificand daca aceasta a reusit, apoi se citeste folosind
fscanf fiecare element al pozei. In cazul pozelor grayscale si rgb, se va citi
si valoarea maxima. Dupa alocarea matricei, se verifica tipul magic word-ului
si se citeste in functie de el. In final, se inchide fisierul, se afiseaza 
mesajul corespunzator si se selecteaza intreaga poza.
Operatiunea este asemanatoare pentru tipul 'b', binar, singura diferenta fiind
reprezentata de folosirea functiei fread, care va citi in elementele matricei
elemente de tipul unsigned char.
## SELECT
Functia SELECT() va realiza selectia in functie de coordonatele citite de la
tastatura. Voi citi intreaga linie folosind fgets, pentru a putea desprinde
tokenii si a aborda cazurile in care se selecteaza ALL, sau 4 coordonate.
Verific daca s-a citit un singur cuvant, folosind functia word_number,
caz in care realizez operatia SELECT ALL, verificand totodata daca exista o
poza incarcata in memorie. 
Daca s-au citit 4 cuvinte, atunci folosesc select_tokens pentru a da fiecarei
coordonate valoarea sa, verificand daca vreuna dintre aceste functii returneaza
1, caz in care se va afisa un mesaj corespunzator. Verific, de asemenea, daca
exista o poza incarcata in memorie.
Daca se citesc un numar de cuvinte diferit de 1 sau 4, atunci comanda este
invalida si se va afisa mesajul corespunzator.
In final, se vor verifica coordonatele, schimbandu-le ordinea in cazul in care
nu sunt furnizate corect si se va realiza selectarea propriu-zisa.
## CROP
Functia CROP() va realiza decuparea fotografiei conform unei selectii,
verificand daca exista o poza incarcata in memorie.
Pentru cazul in care poza este rgb: Aloc o matrice auxiliara,
de dimensiunile selectate, in care adaug elementele din matricea originala
cuprinse in intervalul selectat. Eliberez vechea matrice, actualizand
dimensiunile si alocand o noua matrice. Adaug in aceasta elementele din aux, pe
care o eliberez si reselectez intreaga poza.
Acest algoritm este analog in cazul in care imaginea este de tip alb-negru sau
grayscale.
## EDGE, SHARPEN, BLUR, GAUSSIAN_BLUR
Functiile aplica kernel-ul corespunzator operatiei selectate.
Initial, verific daca imaginea introdusa nu este rgb, afisand un mesaj
corespunzator. Aloc o auxiliara in care introduc valorile cuprinse in selectia
data. Daca aceasta cuprinde marginile, se vor copia neafectate in auxiliara. In
rest, se va aplica kernel-ul operatiei. Apoi, actualizez vechea matrice cu
valorile obtinute, folosind clamp() pentru a le readuce in [0,255].
In final, eliberez auxiliara si afisez mesajul corespunzator.
## APPLY
In cadrul functiei APPLY() voi apela toate functiile de mai sus. Citesc de la
tastatura intregul rand, pentru a-l prelucra, apoi apelez functiile. Daca nu se
citeste nicio optiune, sau aceasta nu se afla printre cele valide, afisez 
mesajele corespunzatoare.
## HISTOGRAM
Functia HISTOGRAM() realizeaza histograma pozei, verificand daca exista o
imagine incarcata in memorie. De asemenea, verific daca imaginea este rgb sau
parametrii nu sunt valizi. Construiesc vectorul de frecventa al valorilor
pixelilor si vectorul dist[] al pixelilor cu valori distincte, pastrand de
asemenea maximul frecventelor. In final, afisez histograma dupa modelul cerut,
folosind formula data.
## EQUALIZE
Functia EQUALIZE() realizeaza egalizarea pozei, verificand daca exista o
imagine incarcata in memorie sau daca poza este de tip rgb. Construiesc 
vectorul de frecventa al valorilor pixelilor, apoi calculez fiecare valoare 
noua a pixelilor, in functie de formula precizata. Suma frecventelor va fi
initializata cu 0 pentru fiecare pixel, iar rezultatul imparitii va fi readus
in intervalul [0,255] folosind clamp() si fiecare pixel va fi actualizat. In
final, va fi afisat mesajul corespunzator. 
## ROTATE
Functia ROTATE() realizeaza rotirea pozei verificand daca exista o imagine
incarcata in memorie, daca unghiul este corespunzator sau daca selectia este
patratica sau completa. Functia realizeaza doar rotirea cu 0 sau 360 de grade.
## SAVE_BIN
Functia SAVE_BIN() realizeaza salvarea in format binar. Verific daca fisierul
in care se va salva s-a deschis, apoi convertesc magic word-ul, daca acesta 
corespunde tipului ascii, in cel corespunzator tipului binar. Scriu in fisier
detaliile corespunzatoare pozei (dimensiune, valoare maxima), apoi verific daca
fisierul este de tip alb negru/ grayscale sau rgb pentru a tipari matricea in
fisier.
## SAVE_ASCII
Functia SAVE_ASCII() realizeaza salvarea in format ascii. Verific daca fisierul
in care se va salva s-a deschis, apoi convertesc magic word-ul, daca acesta 
corespunde tipului binar, in cel corespunzator tipului ascii. Scriu in fisier
detaliile corespunzatoare pozei (dimensiune, valoare maxima), apoi verific daca
fisierul este de tip alb negru/ grayscale sau rgb pentru a tipari matricea in
fisier.
## SAVE
In cadrul functiei SAVE() voi apela toate functiile de mai sus. Citesc intreaga
linie folosind fgets, pentru a putea desprinde tokenii, pe care ii numar. 
Salvez primul token in aux, apoi verific daca am 2 cuvinte si al doilea este
ascii, apeland SAVE_ASCII. Daca am un singur cuvant, apelez SAVE_BIN.
## EXIT
Functia EXIT() realizeaza eliberarea matricei, verificand initial daca exista o
poza incarcata in memorie.

## main()
In functia main() declar variabila ce va stoca comenzile si initializez 
matricea pozei cu NULL. In cadrul unei bucle infinite, apelez fiecare functie.
Daca citesc EXIT, voi apela functia corespunzatoare si voi iesi din bucla,
Daca citesc o comanda necorespunzatoare, voi consuma intreaga linie, restul
fiind si el invalid, si voi afisa mesajul corespunzator.