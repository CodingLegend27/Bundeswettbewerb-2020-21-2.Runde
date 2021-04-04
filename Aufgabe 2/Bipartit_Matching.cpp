#include <bits/stdc++.h>
using namespace std;
typedef vector<int> vi;
typedef vector<string> vs;
typedef pair<int, int> pii;
typedef vector<pii> vpii;
typedef vector<vpii> vvpii;

#define what(x) cerr<<#x<<" is "<<x<<endl;

const int INF = numeric_limits<int>::max()/2;

// maximales bipartites, gewichtetes Matching
// Verwendung der Ungarischen Methode

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


int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);

	cin >> MAXN;
	// counter_used_words = MAXN;
	counter_used_words = 0;

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


	// vvpii adj(2*MAXN);

	// Adjazenzliste nicht so gut geeignet (wegen Laufzeit zum Ändern der Gewichtung eines Knoten)
	
	// Initialisieren eines 2D-Arrays mit 0
	// int adj [2*MAXN][2*MAXN];

	// Adjazenzliste mit Obst --> Schale; d.h. adj[Obst][Schale]
	int adj[MAXN][MAXN];
	// memset(adj, -1000, sizeof(adj));
	for (int i=0;i<MAXN;++i) {
		for (int j=0;j<MAXN;++j) {
			adj[i][j] = -INF;
		}
	}



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
				schale--;
				if (adj[sorte][schale] < 0) adj[sorte][schale] = 0;
				adj[sorte][schale] ++;
				
				// adj[schale][sorte] ++;
			}
		}
	}

	for (int i=0;i<MAXN;++i) {
		for (int j=0;j<MAXN;++j) {
			if(adj[i][j]>0) cout<<"i: "<<num2words[i]<<" j: "<<j+1<<" -> "<<adj[i][j]<<"\n";
		}
	}	



	vi u(MAXN), v(MAXN), p(MAXN), way(MAXN);

	// hungarian algorithm
	for (int i = 0; i < MAXN; ++i) {
		p[0] = i;
		int j0 = 0;

		vi minv (MAXN, INF);
		vi used (MAXN, false);

		do {
			used[j0] = true;
			int i0 = p[j0];
			int delta = INF;
			int j1;

			for (int j = 0; j < MAXN; ++j) {
				if (!used[j]) {
					int cur = adj[i0][j] - u[i0] - v[j];

					if (cur < minv[j]) {
						minv[j] = cur;
						way[j] = j0;
					}

					if (minv[j] < delta) {
						delta = minv[j];
						j1 = j;
					}

				}
			}

			for (int j = 0; j < MAXN; ++j) {
				if (used[j]) {
					u[p[j]] += delta;
					
					// what(v[j]); what(delta);
					// avoid overflow error
					if (abs(v[j] - delta) <= INF) v[j] -= delta;
				}
				else {
					minv[j] -= delta;
				}
			}

			j0 = j1;
		
		} while(p[j0] != 0);

		do {
			int j1 = way[j0];
			p[j0] = p[j1];
			j0 = j1;
		} while (j0);
	}

	vpii result;
	for (int i = 0; i < MAXN; ++i) {
		result.push_back({p[i], i});

		cout<<p[i]+1<<" -> "<<num2words[i]<<"\n";
	}



	// cout<<"Hello World";

	return 0;
}