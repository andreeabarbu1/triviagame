#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <menu.h>
#include <panel.h>
#include <ctype.h>
#include <time.h>

#define LATIME 30
#define LUNGIME 10

//definirea unei structuri cu optiunile meniului
char * optiune[] = {
	"New Game",
	"Resume Game",
	"Quit",
};

//afiseaza fereastra meniului
void afiseazaMeniu ( WINDOW *fereastraMeniu, int pozitie ) {
	init_pair ( 6, COLOR_MAGENTA, COLOR_WHITE );
	int x, y, i;
	x = 2;
	y = 2;
	box ( fereastraMeniu, 0, 0 );
	wbkgd ( fereastraMeniu, COLOR_PAIR (6) );
	for ( i = 0; i < 3; i++) {
		if ( pozitie == i + 1 ) {
			wattron ( fereastraMeniu, A_REVERSE | A_BOLD );
			mvwprintw ( fereastraMeniu, y, x, "%s", optiune[i] );
			wattroff ( fereastraMeniu, A_REVERSE | A_BOLD );
		}
		else
			mvwprintw ( fereastraMeniu, y, x, "%s", optiune[i] );
		++y;
	}
	wrefresh ( fereastraMeniu );
}

//afiseaza TRIVIA intr-un mod personalizat
void trivia ( ) {
	init_pair ( 11, COLOR_MAGENTA, COLOR_BLACK );
	attron ( A_BOLD | COLOR_PAIR (11) );
	mvprintw ( 0, 11, "_|_|_|_|_|  _|_|_|    _|_|_|  _|      _|  _|_|_|    _|_|    ");
	mvprintw ( 1, 11, "    _|      _|    _|    _|    _|      _|    _|    _|    _|  ");
	mvprintw ( 2, 11, "    _|      _|_|_|      _|    _|      _|    _|    _|_|_|_|  ");
	mvprintw ( 3, 11, "    _|      _|    _|    _|      _|  _|      _|    _|    _|  ");
	mvprintw ( 4, 11, "    _|      _|    _|  _|_|_|      _|      _|_|_|  _|    _|  ");
	attroff ( A_BOLD | COLOR_PAIR (11) );
}

//returneaza pozitia optiunii curente: pozitie[0] = 1 pt New Game, =2 pt Resiume
// =3 pt Quit si pozitie[1] = 10 daca a fost apasata tasta Enter
int * optiuneAleasa ( int tasta, int * pozitie ) {
	switch ( tasta ) {
		//apasarea sagetii in sus
		case KEY_UP: 
			if ( pozitie[0] == 1 )
				pozitie[0] = 3;
			else 
				pozitie[0] = pozitie[0] - 1;
			break;
		//apasarea sagetii in jos
		case KEY_DOWN:
			if ( pozitie[0] == 3 )
				pozitie[0] = 1;
			else 
				pozitie[0] = pozitie[0] + 1;
			break;
		//apasarea tastei ENTER
		case 10:
			pozitie[1] = 10;	
		default:
			refresh ( );
			break;
	}
	return pozitie;
}

//functie ce aloca dinamic vectorul de siruri de caractere 
char ** alocareDinamica ( int r, int col ) {	
	int i;
	char ** sir = ( char ** ) malloc ( sizeof ( char * ) * r );
	for ( i = 0; i < r; i++)
		sir[i] = ( char * ) malloc ( sizeof ( char ) * col );
	return sir;
}

//returneaza un sir ce retine pe prima pozitie litera corespunzatoare raspunsului ales
char * rsAles ( char c) {
	char * alegere;
	alegere = malloc ( 5 * sizeof ( char * ) );
	alegere[0] = '\0';
	if ( c == 'a' || c == 'A' ) {
		alegere[0] = 'A';
	}
	else 
	if ( c == 'b' || c == 'B' ) {
		alegere[0] = 'B';
	}
	else 
	if ( c == 'c' || c == 'C' ) {
		alegere[0] = 'C';
	}
	else 
	if ( c == 'd' || c == 'D' ) {
		alegere[0] = 'D';		
	}
	else 
	if ( c == 'q' || c == 'Q' ) {
		alegere[0] = 'q';
	}
	else 
	if ( c == 's' || c == 'S' ) {
		alegere[0] = 's';
	}
	else {
		alegere[0] = 'X';
	}
	alegere[1] = '\0';
	return alegere;
}

//functie ce elibereaza memoria vectorului de siruri de caractere
void dealocare ( char ** sir, int r ) {	
	int i;
	for ( i = 0; i < r; i++ )
		free ( sir[i] );
	free ( sir );
}

//eliminarea unei ferestre
void destroy_win ( WINDOW * win ) {
	wborder ( win, ' ', ' ', ' ',' ',' ', ' ', ' ',' ' );
	wrefresh ( win );
	delwin ( win );
} 

//afisarea intrebarilor si returnarea raspunsului curent
char * rscorect ( char ** s, int m, int n, WINDOW *fereastraJoc, int i) {
	char * raspuns;
	int j;
	init_pair ( 1, COLOR_CYAN, COLOR_WHITE );
	init_pair ( 2, COLOR_CYAN, COLOR_WHITE );
	raspuns = malloc ( 3 * sizeof ( char * ) );
	raspuns[0] = '\0';
	wattron ( fereastraJoc, COLOR_PAIR(1) | A_BOLD | A_DIM );
	//afisarea intrebarii
	mvwprintw( fereastraJoc, m, 3, "%s", s[i] );
	wattroff ( fereastraJoc, COLOR_PAIR(1) | A_BOLD | A_DIM );
	mvwprintw( fereastraJoc, m+1, 3, "\n");
	for( j = i + 1; j < i + 5; j++) {
		wattron ( fereastraJoc, COLOR_PAIR(2) | A_BOLD | A_DIM );
		//afisarea fiecarui raspuns
		mvwprintw ( fereastraJoc, m+2, n, "%s", s[j] );
		wattroff ( fereastraJoc, COLOR_PAIR(2) | A_BOLD | A_DIM );
		wrefresh ( fereastraJoc );	
		m = m + 1;			
	}
	//retine in prima pozitie a sirului raspuns raspunsul corect al intrebarii 
	raspuns[0] = s[j][0];
	raspuns[1] = '\0';
	return raspuns;
}

//afisarea timpului curent si al datei, sunt actualizate la apasarea oricarei taste
void afiseazaTimp ( WINDOW * win ) {
	init_pair ( 3, COLOR_BLUE, COLOR_BLACK);
	int ora, min, sec, zi, luna, an;
	time_t t;
	time ( &t );
	struct tm * local = localtime ( &t );
	ora = local -> tm_hour;
	min = local -> tm_min ;
	sec = local -> tm_sec;
	zi = local -> tm_mday;
	luna = local -> tm_mon + 1;
	an = local -> tm_year + 1900;
	wattron ( win, COLOR_PAIR(3) | A_STANDOUT | A_BOLD );
	mvwprintw ( win, 4, 5, "%02d:%02d:%02d  ", ora, min, sec );
	mvwprintw ( win, 5, 5, "%02d/%02d/%02d", zi, luna, an );
	wattroff ( win, COLOR_PAIR(3) | A_STANDOUT | A_BOLD );
}

//afiseaza scorul 
void afisareScor ( WINDOW * win, int scor ) {
	init_pair ( 4, COLOR_RED, COLOR_WHITE );
	mvwprintw ( win, 4, 30, "Score:        " );
	wattron ( win, COLOR_PAIR (4) | A_BLINK | A_BOLD | A_DIM );
	mvwprintw ( win, 4, 30, "Score: %d", scor );
	wattroff ( win, COLOR_PAIR(4) | A_BLINK | A_BOLD | A_DIM );
}

//functie ce returneaza fereastra scorului final dupa terminarea ultimei intrebari
WINDOW * afisareScorFinal ( WINDOW * win, int randuri, int coloane, int * ok) {
	int startX = 0, startY = 0;
	init_pair ( 7, COLOR_BLACK, COLOR_YELLOW );
	init_pair ( 8, COLOR_RED, COLOR_YELLOW );
	win = newwin ( randuri, coloane, startY, startX );
	wbkgd ( win, COLOR_PAIR(7) );
	wrefresh ( win );
	wattron ( win, A_BOLD | COLOR_PAIR(8) | A_DIM );
	//afisarea scorului final
	mvwprintw ( win, 4, 30, "Your SCORE:%d\n", ok[1] );
	//afisarea numarului intrebarilor la care s-a raspuns corect
	mvwprintw ( win, 6, 23, "Ai raspuns la: %d INTREBARI CORECTE", ok[2] );
	wrefresh ( win );
	wattron ( win, A_BOLD | COLOR_PAIR(8) | A_DIM );
	//afisarea scorului final
	mvwprintw ( win, 4, 30, "Your SCORE:%d\n", ok[1] );
	//afisarea numarului intrebarilor la care s-a raspuns gresit
	mvwprintw ( win, 7, 38, "%d INTREBARI GRESITE", ok[3] );
	mvwprintw ( win, 14, 2, ".#######.#######.##......###..######.###.#######....###...########.###.####");
	mvwprintw ( win, 15, 2, ".##......##......##.......##.##....#..##....##.....##.##..##.....#..##.####");
	mvwprintw ( win, 16, 2, ".##......##......##.......##.##.......##....##....##...##.##.....#..##.####");
	mvwprintw ( win, 17, 2, ".######..######..##.......##.##.......##....##...##.....#.########..##..##.");
	mvwprintw ( win, 18, 2, ".##......##......##.......##.##.......##....##...########.##...##...##.....");
	mvwprintw ( win, 19, 2, ".##......##......##.......##.##....#..##....##...##.....#.##....##..##.####");
	mvwprintw ( win, 20, 2, ".##......#######.#######.###..######.###....##...##.....#.##.....#.###.####");
	wattroff ( win, COLOR_PAIR(8) | A_BOLD | A_DIM );
	wrefresh ( win );
	return win;
}

//afisarea optiunei Skip in fereastra jocului
void afisareSkip ( WINDOW * win ) {
	init_pair ( 5, COLOR_MAGENTA, COLOR_WHITE );
	wattron ( win, COLOR_PAIR (5) | A_DIM | A_BOLD );
	mvwprintw ( win, 4, 52, "Skip the question" );
	mvwprintw ( win, 5, 49, "(pentru a sari intrebarea " );
	mvwprintw ( win, 6, 52, " apasa tasta 's')" );
	wattroff ( win, COLOR_PAIR (5) | A_DIM | A_BOLD );
}

//afisarea intrebarilor si returnarea unui vector ce retine pe fiecare pozitie cate o optiune
int * afiseazaIntrebari ( int sareIntrebarea, int nrIntrbC, int nrIntrbG, int j, char ** s, char * rsCorect, char * alegere, WINDOW * fereastraJoc, int m, int n, int scorX, int scor, int k ) {
	int i, c, *ok;
	ok = malloc ( 5 * sizeof ( int * ) );
	//retine i pentru a reveni la pozitie
	ok[0] = 1;
	//nr intrebarilor corecte
	ok[2] = 0;
	//nr intrebarilor gresite
	ok[3] = 0;
	//retine valoarea pt a vedea daca a fost sarita intrebarea: 0(nu a fost folosita) sau 1(a fost folosita)
	ok[4] = 0;
	ok[4] = sareIntrebarea;
	ok[2] = nrIntrbC;
	ok[3] = nrIntrbG;
	for ( i = j; i < k; i = i + 6 ) {
		//retine in rsCorect raspunsul corect al intrebarii 
		strcpy ( rsCorect, rscorect ( s, m, n, fereastraJoc, i ) );
		while ( c = wgetch ( fereastraJoc ) ) {
			afiseazaTimp ( fereastraJoc );
			scorX = scor;
			//retine in alegere raspunsul ales
			strcpy ( alegere, rsAles ( c ) );
			wrefresh ( fereastraJoc );
			//daca au fost selectate doar tastele : A, B, C, D
			if ( alegere[0] != 'X'  && alegere[0] != 'q' && alegere[0] != 's' ) {
				//daca a fost selectata varianta corecta scorul creste
				if( rsCorect[0] == alegere[0]){ 
					scor = scor + 10;
					afisareScor ( fereastraJoc, scor );
					//nr intrebarilor corecte creste cu o unitate
					ok[2] ++;
				}
				//daca a fost selectat un raspuns gresit scorul scade
				else {
					scor = scor - 5;
					afisareScor ( fereastraJoc, scor );
					//nr intrebarilor gresite scade cu o unitate
					ok[3] ++;
					}
			}
			//daca a fost apasata tasta 's' si optiunea Skip nu a fost folosita
			//sare la intrebarea urmatoare fara sa fie afectat scorul
			if ( alegere[0] == 's' && ok[4] == 0 ) {
				ok[4] = 1;
				break;
			}
			else
			//daca a fost selectata alta tasta in afara de A, B, C, D
			//scorul nu s-ar fi modificat, daca a ramas acelasi se reia
			if ( scorX != scor )
				break; 
			else
			//pt apasarea tastei 'q' se retinea pozitia i si scorul folosite 
			//ulterior pt afisarea intrebarii curente daca se selecteaza Resume
			if ( alegere[0] == 'q') {
				ok[0] = i;
				ok[1] = scor;
				return ok;
			}
		}
	}
	ok[1] = scor;
	return ok;
}
	
int main ( int argc, char ** argv ) {
	int startX = 0, startY = 0, tasta, randuri, coloane, pozitieInitiala = 1;
	int scor = 0, k = 0, incepeJocul = 0, m, n, optiuneCurenta, i, j = 0, nrCaractere;
	int nrIntrbG = 0, nrIntrbC = 0, sareIntrebarea = 0;
	int aux = 0, scorX = 0, c;
	int * pozitie = malloc ( sizeof ( int ) * 2 );
	int * ok = malloc ( 5 * sizeof ( int * ) );
	char ** s, line[1000], alegere[5], rsCorect[5];
	WINDOW * meniu, * fereastraJoc, * fereastraScor, *fereastra;
	FILE *f1;
	pozitie[0] = 1;
	pozitie[1] = 0;
	startY = ( 24 - LUNGIME ) / 2;
	startX = ( 80 - LATIME ) / 2;
	s = alocareDinamica ( 2, 100 );
	//citirea intrebarilor si raspunsurilor din fisiere
	if (argc < 2) {
		printf("[Eroare]: Nu s-au dat argumente in linia de comanda.\n");
		return 1;
	}
	for ( i = 2; i <= argc; i++ ) {
		f1 = fopen ( argv[i], "r" );
		if ( f1 != NULL) {
			line[0] = '\0';
			while ( fgets ( line, sizeof line, f1 ) != NULL ) {
				line[ strlen ( line ) ] = '\0';
				nrCaractere = strlen ( line );
				//realocarea memoriei vectorului de siruri
				s[k] = realloc ( s[k], ( ( nrCaractere + 1 ) * sizeof ( char ) ) );
				s[k][0] = '\0';
				//retinerea in fiecare pozitie intrebarea sau raspunsul liniei curente
				strcpy ( s[k], line ); 
				k++;
				s = realloc ( s, sizeof ( char * ) * ( k + 2 ) ); 
			}
		fclose ( f1 );
		} /* else {
			printf("[Eroare]: Fisierul %s nu poate fi deschis.\n", argv[i]);
			return 1;
		}*/
	}
	//deschiderea modului ncurses
	initscr ( );
	clear ( );
	noecho ();
	cbreak ( );	
	curs_set (0);
	start_color ( );
	//retine in randuri si coloane valoarea maxima a ecranului
	getmaxyx ( stdscr, randuri, coloane );
	//init_pair initializeaza o pereche de culori
	init_pair ( 7, COLOR_BLACK, COLOR_YELLOW );
	init_pair ( 12, COLOR_BLACK, COLOR_WHITE );
	meniu = newwin ( LUNGIME, LATIME, startY, startX );
	fereastra = newwin (randuri, coloane, startX, startY );
	mvprintw ( randuri - 2, 0, " Foloseste sagetile SUS si JOS pentru a naviga, apasa ENTER pentru a selecta o\n optiune");
	trivia ( );
	refresh ( );
	afiseazaMeniu ( meniu, pozitieInitiala );
	//selectarea optiunii meniului
	LOOP: while (1) {
		sareIntrebarea = 0;
		nrIntrbG = 0;
		nrIntrbC = 0;
		i = 0;
		keypad ( meniu, TRUE );
		refresh ( );
	 	tasta = wgetch ( meniu );
		pozitie  = optiuneAleasa ( tasta, pozitie );
		 //daca este selectata Resume si nu a inceput un joc nou nu se intampla nimic
		if ( pozitie[0] == 2 && pozitie[1] == 10 && incepeJocul == 0 ) {
			pozitie[1] = 0;
			goto LOOP;
		}
		afiseazaMeniu ( meniu, pozitie[0] );
		//daca este selectata optiunea Quit se iese din joc
		if ( pozitie[0] == 3 && pozitie[1] == 10) {
			endwin();
			return 0;
		}
		else
		//daca e selectata optiunea New game se incepe un joc nou
		if ( pozitie[0] == 1 && pozitie[1] == 10 ) {
			scor = 0;
			// se retine in variabila "incepeJocul" ca a inceput jocul
			incepeJocul = 1;
			destroy_win ( meniu );
			clear ( );
			refresh ( );
			//dupa ce a fost distrusa fereastra "meniu", se creeaza "fereastraJoc"
			fereastraJoc = newwin ( randuri, coloane, 0, 0 );
			init_pair ( 12, COLOR_BLACK, COLOR_WHITE );
			wbkgd ( fereastraJoc, COLOR_PAIR (12) );
			refresh ( );
			//
			LOOP1: {
			//este afisat timpul si data curenta
			afiseazaTimp ( fereastraJoc );
			wrefresh ( fereastraJoc );
			keypad ( fereastraJoc, TRUE );
			afisareScor ( fereastraJoc, scor) ;
			afisareSkip ( fereastraJoc );
			m = (randuri-2) / 2;
			n = 20;
			//afisarea intrebarilor, actualizarea scorului, retinerea optiunilor selectate 
			ok = afiseazaIntrebari ( sareIntrebarea, nrIntrbC, nrIntrbG, i, s, rsCorect, alegere, fereastraJoc, m, n, scorX, scor, k );
			//aux retine scorul curent
			aux = ok[1];
			//daca nu a fost apasata tasta 'q'
			if ( ok[0] == 1 ) {	
				wclear ( fereastraJoc );
				wrefresh ( fereastraJoc );
				destroy_win ( fereastraJoc );
				bkgd ( COLOR_BLACK );
				refresh ( );
				//se afiseaza fereastra scorului final
				fereastraScor = afisareScorFinal ( fereastraScor, randuri, coloane, ok );
				//odata cu apasarea unei taste dispare fereastra scorului final
				while ( c = wgetch ( fereastraScor ) ) {
					wclear ( fereastraScor );
					destroy_win ( fereastraScor );
					bkgd ( COLOR_BLACK );
					//cream fereastraJoc, setam bkgd
					fereastraJoc = newwin ( randuri, coloane, 0, 0 );
					wbkgd ( fereastraJoc, COLOR_PAIR (12) );
					refresh ( );
					wrefresh ( fereastraJoc );
					LOOP2: {
						pozitie[0] = 1;
						pozitie[1] = 0;
						optiuneCurenta = 0;
						//cream si afisam meniul
						meniu = newwin ( LUNGIME, LATIME, startY, startX );
						afiseazaMeniu ( meniu, pozitieInitiala );
						//reluam de la inceput programul pentru selectarea unei optiuni a meniului
						goto LOOP;
					}
				}
			}
			//daca s-a apasat tasta 's', intrebarea curenta este sarita
			if ( alegere[0] == 's' && sareIntrebarea == 0 ) {
				sareIntrebarea == 1;
				i = ok[0] + 1;
				//se continua afisarea intrebarilor
				goto LOOP1;
			}
			else
			//daca este apasata tasta 'q' se revine la meniu
			if ( alegere[0] == 'q' ) {
				i = j;
				scor = aux;//se retine scorul pentru a se afisa nemodificat la selectarea opt Resume
				wclear ( fereastraJoc );
				wrefresh ( fereastraJoc );
				destroy_win ( fereastraJoc );
				bkgd ( COLOR_BLACK );
				refresh ( );
				pozitie[0] = 1;
				pozitie[1] = 0;
				optiuneCurenta = 0;
				//se creaza si afiseaza fereastra meniului
				meniu = newwin ( LUNGIME, LATIME, startY, startX );
				afiseazaMeniu ( meniu, pozitieInitiala );
				//reluam de la inceput programul pentru selectarea unei optiuni a meniului
				goto LOOP; 
			}
		}
		}
		//daca se selecteaza Resume si New Game a fost selectat cel putin o data
		//se revine la jocul inceput anterior
		if (pozitie[0] == 2 && pozitie[1] == 10 && incepeJocul == 1 ) {
			if ( ok[i] != 1) {
			//se sterge fereastra meniu
			destroy_win ( meniu );
			//se retin pozitia i, numarul intrebarilor corecte si gresite si daca a fost
			//utilizata optiunea Skip the question si scorul din jocul anterior
			i = ok[0];
			nrIntrbC = ok[2];
			nrIntrbG = ok[3];
			sareIntrebarea = ok[4];
			scor = aux;
			fereastraJoc = newwin ( randuri, coloane, 0, 0 );
			wbkgd ( fereastraJoc, COLOR_PAIR (12) );
			refresh ( );
			wrefresh ( fereastraJoc );
			//continuarea jocului prin afisarea intrebarii la care s-a ramas
			goto LOOP1;
		}
		else 
			goto LOOP2;	
		}
	}
	clrtoeol ( );
	refresh ( );
	//inchiderea modului ncurses
	endwin ( );
	free ( pozitie );
	free ( ok );
	free ( alegere );
	free ( rsCorect );
	//dealocarea memoriei sirului cu intrebari si raspunsuri
	dealocare ( s, k + 1 );
	return 0;
}