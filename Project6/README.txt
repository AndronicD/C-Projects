NUME: ANDRONIC DRAGOS SEBASTIAN
GRUPA: 315CD

FUNCTII AJUTATOARE
FIND_MANAGER: INTOARECE ADRESA UNUI NOD DUPA NUMELE SAU
SORT: SORTEAZA VECTORUL TEAM
F_COMP: FUNCTIE DE COMPARARE PENTRU SORTARE
PRINT_EMPLOYEE: AFISEAZA IN FISIER UN ANGAJAT
ADD_TO_QUEUE: ADAUGA IN COADA
FIND_BY_MANAGER: GASESTE NOD DIN COADA
GET_MAX: RETURNEAZA MAXIMUL DE ANGAJATI DINTR-UN ARBORE
FIND_BY_MAX: GASESTE DUPA UN MAXIM 


TASK1
HIRE: ALOC MEMORIE UNUI NOD DIN ARBORE, DACA MANAGERUL E NULL ATUNCI ESTE
NODUL RADACINA SI RETURNEZ ARBORELE, IAR DACA NU, GASESC MANAGERUL SI IL ADAUG
IN ECHIPA

FIRE: DACA E PRIMUL OM DIN IERARHIE RETURNEZ ARBORELE, DACA NU ADAUG ECHIPA 
NODULUI CAUTAT IN ECHIPA MANAGERULUI NODULUI SI ELIMIN NODUL CERUT 

PROMOTE: DACA FACE PARTE DIN PRIMELE DOUA NIVELE RETURNEZ ARBORELE, DACA NU
ADAUG ECHIPA ANGAJATULUI CAUTAT IN ECHIPA MANAGERULUI SAU, IL ELIMIN DIN ECHIPA 
MANAGERULUI SI APOI IL ADAUG IN ECHIPA MANAGERULUI MANAGERULUI

MOVE_EMPLOYEE: GASESC NODURILE DE ANAGAJAT SI MANAGERUL CERUT, MUT ECHIPA ANAGAJATULUI
IN ECHIPA MANAGERULUI SAU, IL SCOT PE ANGAJAT DIN ECHIPA MANAGERULUI ACTUAL SI IL MUT
IN ECHIPA MANAGERULUI NOU

MOVE_TEAM: GASESC NODURILE DE ANGAJAT SI MANAGERUL CERUT, ELIMIN NODUL ANGAJATULUI
DIN ECHIPA MANAGERULUI CURENT SI APOI IL ADAUG IN ECHIPA MANAGERULUI CERUT

FIRE_TEAM: GASESC ANGAJATUL, IL ELIMIN DIN ECHIPA MANAGERULUI CURENT SI APOI
CU FUNCTIA DESTROY_TREE ELIBEREZ MEMORIA SUBARBORELUI ANGAJATULUI

TASK2
GET_EMPLOYEES_BY_MANAGER: ADAUG TOTI ANAGJATII UNUI MANAGER INTR-O COADA, II SORTEZ
SI DUPA II AFISEZ

GET_EMPLOYEES_BY_LEVEL: ADAUG TOTI ANGAJATII DE PE UN NIVEL INTR-O COADA, II SORTEZ
SI DUPA II AFISEZ

GET_BEST_MANAGER: GASESC MAXIMUL NUMARULUI DE ANGAJATI PE CARE IL AU TOTI MANAGERII
DUPA II GASESC PE TOTI ANGAJATII CU ACELASI NUMAR DE ANGAJATI, II ADUAG INTR-O COADA
II SORTEZ SI II AFISEZ

DESTROY_TREE: ELIBEREZ MEMORIA ARBORELUI

PREORDER_TRAVERSAL: MA FOLOSESC DE FUNCTIA PRINT_WITH_MANAGER CARE DOAR ADAUGA
NUMELE MANAGERULUI SI ANGAJATULUI IN FISIERUL DAT CA PARAMETRU, AFISEZ NODUL RADACINA
SI DACA AM AFISAT TOT ARBORELE MAI ADAUG SI LINIE NOUA
