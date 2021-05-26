#include <bits/stdc++.h>
using namespace std;
typedef vector<int> vi;
typedef vector<string> vs;
typedef pair<int, int> pii;
typedef vector<pii> vpii;
typedef vector<vpii> vvpii;
typedef vector<vi> vvi;
typedef tuple<int, int, int> tiii;
typedef tuple<int, int, int, int> tiiii;

#define what(x) cerr<<#x<<" is "<<x<<endl;

const int INF = numeric_limits<int>::max()/2;


int MAXN; // maximale Anzahl der N Obstsorten
vi wunsche; // die Wunschsorten

int M; // Anzahl der beobachteten Spieße

// Unordered map zum Konvertieren von Obstbezeichnungen zu Nummern
unordered_map<string, int> words2num;
unordered_map<int, string> num2words;

// Anzahl der verwendeten Obstnamen (wichtig für die Konvertierung der Namen)
// Zähler beginnt bei MAXN, damit Schalen und Sorten nicht vertauscht werden können
int counter_used_words;

void add_word_to_converter(string& word) {
	if (words2num.find(word) == words2num.end()) {
		// Wort muss noch hinzugefügt werden
		
		words2num[word] = counter_used_words;
		num2words[counter_used_words] = word;
		counter_used_words ++;
	}
}

// gibt die Strings in einer Zeile in einem Vector zurück
vs getline2vector_string(string& input_line) {
	istringstream buffer(input_line);
	vs arr ((istream_iterator<string>(buffer)),
			istream_iterator<string>());
	return arr;
}

// gibt die Ints in einer Zeile in einem Vector zurück
vi getline2vector_int(string& input_line) {
	// konvertiert die Eingabe zu einem Vector aus Strings,
	// dieser wird nun zu Zahlen konvertiert
	vs strings = getline2vector_string(input_line);
	vi arr;
	for (string s : strings) {
		int a = stoi(s);
		arr.push_back(a);
	}
	return arr;
}

bool sortbyforth(const tiiii& a, const tiiii& b) {
	int a1, a2, a3, a4, b1, b2, b3, b4;
	tie(a1, a2, a3, a4) = a;
	tie(b1, b2, b3, b4) = b;
	return a4 < b4;
}


int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);

	cin >> MAXN;
	MAXN++;
	// counter_used_words = MAXN;
	counter_used_words = 1;

	// Einlesen von Donalds Wünschen in einer Zeile
	cin.ignore();
	string donalds_wunsche_line; 
	getline(cin, donalds_wunsche_line);

	// istringstream buffer(donalds_wunsche_line);
	// vs donalds_wunsche((istream_iterator<string>(buffer)), 
	// 					istream_iterator<string>());

	vs donalds_wunsche = getline2vector_string(donalds_wunsche_line);


	for (string word : donalds_wunsche) {
		// Donalds Wünsche zum Konvertieren hinzugefügt
		add_word_to_converter(word);

		// Nummer des Wunsches wird zu 'wunsche' hinzugefügt
		wunsche.push_back(words2num[word]);
	}

	vvi adj(MAXN, vi(MAXN, -1));

	// Anzahl der Beobachtungen
	cin >> M;
	cin.ignore();

	for (int i = 0; i<M; ++i) {
		string obstschalen_line, obstsorten_line;
		getline(cin, obstschalen_line);
		getline(cin, obstsorten_line);

		// what(obstschalen_line); what(obstsorten_line);

		vi obstschalen = getline2vector_int(obstschalen_line);
		vs obstsorten_string = getline2vector_string(obstsorten_line);

		vi obstsorten;
		for (string sorte : obstsorten_string) {
			add_word_to_converter(sorte);
			obstsorten.push_back(words2num[sorte]);
		}

		for (int sorte : obstsorten) {
			for (int schale : obstschalen) {
				// schale--;
				if (adj[sorte][schale] < 0) adj[sorte][schale] = 0;
				adj[sorte][schale] ++;

				cout<<"Schale: "<<schale<<" -> Sorte: "<<num2words[sorte]<<"\n";

				// if (adj[schale][sorte] < 0) adj[schale][sorte] = 0;
				// adj[schale][sorte] ++;
			}
		}
		cout<<"\n"; 

	}

	vi maxVObstsorten(MAXN);

	// für alle Obstsorten
	for (int i = 1; i < MAXN; ++i) {
		int maxV = -INF;

		for (int j = 1; j < MAXN; ++j) {
			maxV = max(maxV, adj[i][j]);
		}

		maxVObstsorten[i] = maxV;
	}
	for (int i=1;i<MAXN;++i) {
		cout<<"Sorte "<<num2words[i]<<" mit maxV "<<maxVObstsorten[i]<<"\n";
	}


	// gewicht, sorte, schale
	// vector<tiii> kanten;
	vi numSorteInPossibleSchalen(MAXN, 0);

	for (int i = 1; i < MAXN; ++i) {
		for (int j = 1; j < MAXN; ++j) {
			if (adj[i][j] == maxVObstsorten[i]) {
				// kanten.push_back({adj[i][j], i, j});
				numSorteInPossibleSchalen[i]++;
			}
			else {
				// adj[i][j] != maxVObstsorten[i]
				adj[i][j] = -1;
			}
		}
	}

	// sorte, schale, gewicht, anzahlSorteinPossibleSchalen
	vector<tiiii> kanten;
	for (int i = 1; i < MAXN; ++i) {
		for (int j = 1; j < MAXN; ++j) {
			if (adj[i][j] == maxVObstsorten[i]) {
				kanten.push_back({i, j, adj[i][j], numSorteInPossibleSchalen[i]});
			}
		}
	}
	sort(kanten.begin(), kanten.end(), sortbyforth);

	// sort(kanten.begin(), kanten.end(), greater<tiii>()); 



	// sorte zu schale
	vvi final_adj(MAXN, vi(MAXN, false));


	for (tiiii k : kanten) {
		int sorte, schale, gewicht, num;
		tie(sorte, schale, gewicht, num) = k;
		cout<<"sorte "<<num2words[sorte]<<" zu "<<schale<<" mit "<<gewicht<<" insg. "<<num<<"\n";

		final_adj[sorte][schale] = true;
	}


	vi usedSorte(MAXN, false), usedSchale(MAXN, false);

	vi isWish(MAXN, false);
	for (int i : wunsche) {
		isWish[i] = true;
	}

	// sorte, schale
	vpii final_kanten;

	// TODO usedSorte und sorte2schale lässt sich zusammenfassen!

	vi sorte2schale(MAXN, -1);

	bool finished = false;
	bool failed = false;

	while(!finished and !failed) {
		bool changed = false;

		for (int sorte=1; sorte < MAXN; ++sorte) {
			if (usedSorte[sorte]) continue;

			int numPossible = 0;
			int gesuchteSchale = -1;
			for (int schale = 0; schale < MAXN; ++schale) {
				if (final_adj[sorte][schale]) {
					numPossible++;
					gesuchteSchale = schale;
				}
			}

			what(num2words[sorte]); what(numPossible);

			if (numPossible == 1) {
				changed = true;
				final_kanten.push_back({sorte, gesuchteSchale});
				sorte2schale[sorte] = gesuchteSchale; 

				usedSorte[sorte] = true;

				for (int i = 1; i < MAXN; ++i) {
					final_adj[i][gesuchteSchale] = false;
				}
			}
		}

		bool wunscheBelegt = true;
		for (int i : wunsche) {
			if (!usedSorte[i]) {
				wunscheBelegt = false;
				break;
			}
		}
		if (wunscheBelegt) finished = true;

		if (!changed) failed = true;
	}

	if (finished) {
		cout<<"Eindeutige Bestimmung aller Donalds Obstsorten möglich:\n";

		for (int sorte : wunsche) {
			cout<<"> Sorte "<<num2words[sorte]<<" in Schale Nr. "<<sorte2schale[sorte]<<"\n";
		}
		return 0;
	}


	// Adjazenzmatrix für die Abhängigkeiten
	vvi dependencies(MAXN, vi());


	if (failed) {
		cout<<"KEINE eindeutige Bestimmung aller Donalds Obstsorten möglich:\n";

		for (int sorte : wunsche) {
			// Sorte nicht eindeutig bestimmt
			if (!usedSorte[sorte]) {
				cout<<"> Sorte "<<num2words[sorte]<<" kann sich in den Schalen Nr."; 
				vi possibleSchalen;
				for (int schale = 1; schale < MAXN; ++schale) {
					if (final_adj[sorte][schale]) {
						possibleSchalen.push_back(schale);
						cout<<" "<<schale;
					}
				}
				cout<<" befinden\n";

				set<int> possibleSorten;

				for (int schale : possibleSchalen) {
					cout<<" >> in Schale Nr. "<<schale<<" können sich aber auch die Sorten";
					for (int i = 1; i < MAXN; ++i) {
						if (i == sorte) continue;

						if (final_adj[i][schale]) {
							cout<<" "<<num2words[i];

							possibleSorten.insert(i);
						}	
					}
					cout<<" befinden\n";
				}

				cout<<"  >>> für die Bestimmung der Sorte "<<num2words[sorte]<<" ist die Bestimmung der Sorten";
				for (int s : possibleSorten) {
					cout<<" "<<num2words[s];

				}

				cout<<" notwendig!\n";

			}
			else {
				// diese Sorte wurde eindeutig bestimmt
				cout<<"> Sorte "<<num2words[sorte]<<" befindet sich sicher in Schale Nr. "<<sorte2schale[sorte]<<"\n";
			}

			cout<<"\n";

		}
	}

	// TODO
	// Abhängigkeitsbaum erstellen, falls es nicht ganz gelöst wurde
	// denjenigen Knoten (oder mehrere) finden, durch dessen eindeutige Bestimmung die anderen auch gelöst werden können
	// oder gibt es mehrere solche und müssen alle, oder nur manche bestimmt werden ?

	for (int i = 1; i < MAXN; ++i) {
		if (usedSorte[i]) continue;

		set<int> possbileSchalen;

		for (int j = 1; j < MAXN; ++j) {
			
			if (usedSchale[j]) {
				what(usedSchale[j]);
				continue;
			}

			if (final_adj[i][j]) {
				possbileSchalen.insert(j);
			}
		}

		// for (int schale : possbileSchalen) {
		// 	for (int andereSorte = 1; andereSorte < MAXN; ++andereSorte) {
		// 		if (i == andereSorte) continue;

		// 		if (final_adj[andereSorte][schale]) {
		// 			dependencies[i].push_back(andereSorte);
		// 		}
		// 	}
		// }

		for (int andereSorte = 1; andereSorte < MAXN; ++andereSorte) {
			if (i == andereSorte) continue;

			for (int  schale : possbileSchalen) {
				if (final_adj[andereSorte][schale]) {
					dependencies[i].push_back(andereSorte);

					// break, damit eine Sorte nicht zweimal von derselben Sorte 'abhängig' ist
					break;
				}
			}
		}
	}


	for (int i=1; i<MAXN;++i) {
		if (dependencies[i].empty()) continue;
		cout<<num2words[i]<<"\n";
		for (int next : dependencies[i]) {
			// what(next);
			cout<<" -> "<<num2words[next]<<"\n";
		}
	}

	// Ermittlung der Abhängigkeiten der Wünsche
	// vi visited(MAXN, false);
	queue<int> q;

	for (int wunsch : wunsche) {
		if (usedSorte[wunsch]) continue;
		
		vi visited(MAXN, false);
		visited[wunsch] = true;

		// if (!visited[wunsch]) continue;

		q.push(wunsch);
		visited[wunsch] = true;

		while(!q.empty()) {
			int akt = q.front();
			q.pop();

			for (int next : dependencies[akt]) {
				if (!visited[next]) {
					visited[next] = true;
					q.push(next);
				}
			}
		}
		visited[wunsch] = false;

		// Knoten die erreicht wurden, 
		// machen die Obstsorte abhängig
		cout << "> Sorte "<<num2words[wunsch]<<" ist abhängig von:";
		for (int i = 1; i < MAXN; ++i) {
			if (visited[i]) {
				cout<<" "<<num2words[i];
			}
		}
		cout<<"\n";
	}






	// for (tiii k : kanten) {
	// 	int gewicht, sorte, schale;
	// 	tie(gewicht, sorte, schale) = k;

	// 	if (!usedSchale[schale] and !usedSorte[sorte]) {
	// 		final_kanten.push_back({sorte, schale});

	// 		usedSorte[sorte] = true;
	// 		usedSchale[schale] = true;
	// 	}
	// 	else if (!usedSchale[schale]) {
	// 		// cout<<"else 1\n";
	// 		cout<<"Schale "<<schale<<" ist nicht benutzt, Sorte "<<num2words[sorte]<<" schon\n";
	// 		// what(schale);
	// 	}
	// 	else if (!usedSorte[sorte]) {
	// 		cout<<"Sorte "<<num2words[sorte]<<" ist nicht benutzt, Schale "<<schale<<" schon\n";
	// 		// what(sorte);
	// 	}
	// }


	// int index = 0;

	// while(true) {

	// }

	what(final_kanten.size());

	for (int i = 0; i < final_kanten.size(); ++i) {
		// Ausgabe der Sorte und dazugehörige Schale
		cout<<num2words[final_kanten[i].first]<<" -> "<<final_kanten[i].second<<"\n";
	}






	return 0;
}