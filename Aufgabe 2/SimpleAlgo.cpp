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
typedef vector<bool> vb;

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

		vi obstschalen = getline2vector_int(obstschalen_line);
		vs obstsorten_string = getline2vector_string(obstsorten_line);

		vi obstsorten;
		for (string sorte : obstsorten_string) {
			add_word_to_converter(sorte);
			obstsorten.push_back(words2num[sorte]);
		}

		for (int sorte : obstsorten) {
			for (int schale : obstschalen) {
				if (adj[sorte][schale] < 0) 
					adj[sorte][schale] = 0;
				
				adj[sorte][schale] ++;
			}
		}
	}

	// maxValue-Verbindung für die jede Obstsorte bestimmen
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

	// maxValue-Verbindung für jede Schale bestimmen
	vi maxVSchalen(MAXN);

	// für alle Schalen
	for (int j = 1; j < MAXN; ++j) {
		int maxV = -INF;

		for (int i = 1; i < MAXN; ++i) {
			maxV = max(maxV, adj[i][j]);
		}

		maxVSchalen[j] = maxV;
	}

	for (int i=1;i<MAXN;++i) {
		cout<<"Schale: "<<i<<" mit maxV"<<maxVSchalen[i]<<"\n";
	}

	vector<set<int> > sorte2schale(MAXN);
	vector<set<int> > schale2sorte(MAXN);
	for (int sorte = 1; sorte < MAXN; ++sorte) {
		for (int schale = 1; schale < MAXN; ++schale) {
			if (adj[sorte][schale] == maxVObstsorten[sorte] and adj[sorte][schale] == maxVSchalen[schale]) {
				// Schale ist möglich
				sorte2schale[sorte].insert(schale);
				schale2sorte[schale].insert(sorte);
			}
		}
	}

	for (int sorte = 1; sorte < MAXN; ++sorte) {
		cout << "Sorte: "<<num2words[sorte]<<"\n";

		for (int schale : sorte2schale[sorte]) cout << schale << " ";

		cout << "\n";
	}

	for (int schale = 1; schale < MAXN; ++schale) {
		cout << "Schale: "<<schale<<"\n";

		for (int sorte : schale2sorte[schale]) cout << num2words[sorte]<< " ";

		cout <<"\n";
	}

	bool works = true;

	for (int w : wunsche) {
		cout << "Sorte: " << num2words[w] << "\n";

		if (sorte2schale[w].size() == 1) {
			cout << "> ist sicher in Schale Nr. " << *sorte2schale[w].begin() << "\n";
		}
		else if (sorte2schale[w].empty()) {
			works = false;
			cout << "> kann keiner Schale zugewiesen werden!\n";
		}
		else {
			cout << "> kann sich in folgenden Schalen befinden: Nr. ";
			for (int schale : sorte2schale[w]) {
				cout << schale << " ";
			}
			cout << "\n";

			for (int schale : sorte2schale[w]) {
				if (schale2sorte[schale].size() > 1) {
					works = false;

					cout << ">> in Schale Nr. " << schale << " können sich auch folgende Obstsorten befinden: ";

					for (int sorte : schale2sorte[schale]) {
						if (sorte != w) {
							cout << num2words[sorte] << " ";
						}
					}
					cout << "\n";
				}
			}
		}

		cout << "\n";
	}

	// Fazit:
	cout << "\n>>> Zusammenfassung: Donalds Wünsche können ";
	if (!works) cout << "nicht ";
	cout << "eindeutig bestimmt werden!\n";

	return 0;
}