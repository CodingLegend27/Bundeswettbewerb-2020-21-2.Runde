#include <bits/stdc++.h>
using namespace std;
typedef vector<int> vi;
typedef vector<string> vs;
typedef pair<int, int> pii;
typedef vector<pii> vpii;
typedef vector<vpii> vvpii;

#define what(x) cerr<<#x<<" is "<<x<<endl;

// maximales bipartites, gewichtetes Matching
// Verwendung der Ungarischen Methode

int MAXN; // maximale Anzahl der N Obstsorten
vi wunsche; // die Wunschsorten

int M; // Anzahl der beobachteten Spieße

// Unordered map zum Konvertieren von Obstbezeichnungen zu Nummern
unordered_map<string, int> words2num;
unordered_map<int, string> num2words;

// Anzahl der verwendeten Obstnamen (wichtig für die Konvertierung der Namen)
// Zähler beginnt bei MAXN+1, damit Schalen und Sorten nicht vertauscht werden können
int counter_used_words = MAXN+1;

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
	int adj [2*MAXN][2*MAXN];
	memset(adj, 0, sizeof(adj));



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
				adj[sorte][schale] ++;
				adj[schale][sorte] ++;
			}
		}
	}



	



	// cout<<"Hello World";

	return 0;
}