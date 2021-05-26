#include <bits/stdc++.h>
using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;
typedef tuple<int, int, int> tiii;

#define what(x) cerr<<#x<<" is "<<x<<endl;

const int INF = numeric_limits<int>::max()/2;

// gesamte Anzahl aller Stände, die sich angemeldet haben
int N;

// Länge der Straße
int maxL;


// Größe des Segmentbaumes, die im Laufe des Programms noch ermittelt wird
int sizeSegmentTree;

// neutrales Element, das im Segmentbaum benötigt wird,
// dessen assoziative Operation 'max' ist.
// analog dasselbe auch für den min_segment_tree mit INF als neutrales Element
const int max_neutral_element = -INF, min_neutral_element = INF;

// Vector, in dem der Segmentbaum gespeichert wird (eindimensional)
// und dessen dazugehörige, bereits ausgeführte Operationen des jeweiligen Knotens
// z.B. werden später alle Elemente in einem Intervall um einen bestimmen Wert erhöht werden müssen;
// dies wird im Array operation für alle direkten und indirekten Kinderknoten zwischengespeichert

// Es wird ein Segmentbaum für das Minimum der Intervalle und eines für das Maximum benötigt
vi max_segment_tree, operation;

// neutrale Operation für die apply Methode, die addiert --> 0 ist neutral
int id = 0;

vi starting_times, ending_times, weights;

vi optimal, pre;


// Wert des Einkommens (ganz am Ende des Programms)
int INCOME;

bool sortBySecondElseThird(const tiii& a, const tiii& b) {
    int a1, a2, a3, b1, b2, b3;
    tie(a1, a2, a3) = a;
    tie(b1, b2, b3) = b;

    if (a2 == b2) {
        return a3 < b3;
    }
    return a2 < b2;
}

// Methode zum bestimmen des am weitesten rechten Index, an dem der Wert value in das Array arr
// eingefügt werden müsste, damit es sortiert bleiben würde

// mit binärer Suche in log(n) Zeit, da das Array arr bereits sortiert ist
int binary_search(const vi& arr, const int value) {
    // Zu Beginn: obere Grenze N-1
    // untere Grenze 0
    int up = N-1, low = 0;

    // solange obere Grenze noch oberhalb
    // der unteren Grenze ist, läuft die Such noch;
    // ansonsten wird der Index zurückgegeben, wo value eingefügt werden muss
    while (low < up) {

        // Mittelwert neu bilden
        int mid = floor((low + up)/2);

        if (value < arr[mid]) {
            up = mid;
        }
        else {
            low = mid + 1;
        }
    }

    // dasselbe Element wurde nicht in arr gefunden,
    // daher wird der Index zurückgegeben, wo es eingefügt werden müsste
    return low-1;
}




// Hilfsmethoden für den Segmentbaum
int right(int i) {
    return 2*i+1;
}
int left(int i) {
    return 2*i;
}
int parent(int i) {
    return floor(i/2);
}

// Erstellen des Segmentbaums
// initiale Arrays für Segmentbaum mit maximalen Intervallwerten
void build(const vi& max_arr) {
    // 2-fache Größe die für den Segmentbaum berechnet wurde,
    // wird für den Segmentbaum benötigt
    max_segment_tree.resize(2*sizeSegmentTree);
    operation.resize(2*sizeSegmentTree);

    // hintere Hälfte des Segmentbaums initialisieren,
    // welcher alle Werte des übergebenen Arrays arr enthält
    for (int i = sizeSegmentTree; i < sizeSegmentTree*2; ++i) {
        max_segment_tree[i] = max_arr[i - sizeSegmentTree];

        // Initialisierung zu Beginn mit dem neutralen Element id
        operation[i] = id;
    }

    for (int i = sizeSegmentTree-1; i > 0; --i) {        
        // Maximum bzw. Minimum von den 'vorausgehenden Knoten' l und r wird gebildet
        // und in i gespeichert
        max_segment_tree[i] = max(max_segment_tree[left(i)], max_segment_tree[right(i)]);

        // neutrales Element id wird zugewiesen
        operation[i] = id;
    }
}


// folgende Methode aktualisiert beide Segmentbäume (min und max):
// In der Apply-Methode des Segementbaums werden alle Elemente in dem übergebenen Bereich (von l bis r)
// auf den übergebenen Wert gesetzt
// da die Operation 'Gleichsetzen' kommutativ ist, wird im Segmentbaum Lazy Propagation benötigt
void apply(int l, int r, int x, int y=id, int a=1, int b=sizeSegmentTree, int i=1) {    
    max_segment_tree[i] = y;

    y += operation[i];
    // y = operation[i];
    operation[i] = id;

    if (l <= a and b <= r) {
        operation[i] = x;
        y = x;
        max_segment_tree[i] = x;
        return;
    }

    // aktueller Knoten nicht mehr im angegebenen Intervall 
    if (r < a or b < l) {
        operation[i] = y;
        return;
    }

    // Mittelwert bilden    
    int m = (a + b)/2;

    // linker und rechter Kinderknoten werden rekursiv aufgerufen,
    // damit deren Wert auch aktualisiert werden, 
    // solange sie auch im angebenen Intervall befinden
    apply(l, r, x, y, a, m, left(i));
    apply(l, r, x, y, m+1, b, right(i));

    max_segment_tree[i] = max(max_segment_tree[left(i)], max_segment_tree[right(i)]);

    return;
}

// Abfrage im Segmentbaum nach dem Ergebnis im Intervall von l bis r
// --> Abfrage des maximalen Elements darin
// Verwendung der binären Suche --> log(n) Zeit

// Zu Beginn des rekursiven Aufrufs bei der Wurzel des Segmentbaums
// anschlie0end wird der Baum herabgestiegen,
// um Segmente zu finden, die das Intervall [a; b] aufteilen

int max_seg_query(const int& l, const int& r, int a=1, int b=sizeSegmentTree, int i=1) {
    if (l == r and l == 0) {
        return 0;
    }

    // Aktueller Knoten i steht für den Bereich [a; b]

    // Falls [a; b] ein Teilsegment des gesuchten Bereichs ist
    // -> Rückgabe des aktuellen Werts des Knotens
    if (l <= a and b <= r) {
        return max_segment_tree[i];
    }

    // aktueller Knoten für den Bereich [a; b] nicht mehr im angegebenen Intervall
    if (r < a or b < l) {
        return max_neutral_element;
    }

    // Mittelwert bilden
    int m = (a + b) /2;

    // maximaler Wert des linken und rechten Kinderknoten ermitteln,
    // dabei muss auch noch der Wert von operation[i] hinzugefügt werden
    return operation[i] + max(max_seg_query(l, r, a, m, left(i)), max_seg_query(l, r, m+1, b, right(i)));
}

int getOpt(const int& i) {
    if (i == -1) return 0;

    else if (0 <= i and i < optimal.size()) {
        return optimal[i];
    }

    else {
        // den maximalen Wert zurückgeben, 
        // entweder den aktuellen Wert zusammen mit dem Wert des passenden Vorgängers von i
        // oder i einfach übernehmen, d.h. der akutelle Stand i wird nicht gesetzt
        return max(weights[i] + getOpt(pre[i]), getOpt(i - 1));
    }
}


int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    // maximale Straßenlänge ist frei wählbar
    // cout<<"> Bitte geben Sie die Straßenlänge in Meter ein\n"<<flush;
    // cin >> maxL;

    // oder:
    maxL = 1000;

    INCOME = 0;

    cin >> N;

    // Speicher für benutzte Stände
    vi used_elements;

    used_elements.resize(N, false);

    vector<tiii> arr(N);
    for (int i = 0; i < N; ++i) {
    	// Beginn, Ende und Länge werden eingelesen
    	int a, b, c; 
    	cin >> a >> b >> c;

    	arr[i] = {a, b, c};
    }

    // Sortieren nach ihrem Endzeitpunkt
    // bei gleichem Endzeitpunkt, ist die Länge entscheidend
    sort(arr.begin(), arr.end(), sortBySecondElseThird);

    // die Start- und Endzeiten werden in Arrays gespeichert,
    // nachdem sie sortiert wurden
    // vi starting_times(N), ending_times(N);
    starting_times.resize(N);
    ending_times.resize(N);
    weights.resize(N);

    int minStartTime = INF, maxEndTime = 0;


    for (int i = 0; i < N; ++i) {
        int start, end, length;
        tie(start, end, length) = arr[i];
        
        // Einkommen durch den Stand wird berechnet
        int weight = (end - start) * length;

        starting_times[i] = start;
        ending_times[i] = end;
        weights[i] = weight;

        minStartTime = min(minStartTime, start);
        maxEndTime = max(maxEndTime, end);
    }

    // + + + Segmentbaum + + + //

    // Unterteilung Stundenweise, beginnend von minStartTime bis maxEndTime
    // somit ist intervallSize = 1 (Stunde)
    int intervallSize = 1;


    // Array speichert die aktuelle Höhe zum aktuellen Intervall
    int sizeTimeInterval = (maxEndTime - minStartTime) / intervallSize;


    // es kann sein, das sizeTimeInterval keine 2er Potenz ist, 
    // daher werden noch zusätzliche leere Speicherplätze mit hinzugefügt,
    // damit die Größe des Segmentbaums eine 2er Potenz
    int potenz = ceil(log2(sizeTimeInterval));

    // rest --> auffüllen
    sizeSegmentTree = pow(2, potenz);

    int rest = sizeSegmentTree - sizeTimeInterval;

    // Erstellen des Segmentbaumes, mit einem leeren Array,
    // da noch keine Stände festgelegt wurden
    build(vi(sizeSegmentTree, 0));


    cout << "> Folgende Stände sind eingeplant (im Format Start- und Endzeitpunkt + deren Länge \n";

    int counterUsed = 0;
    int counterUnpossible = 0;
    
    bool firstTime = true;

    while(true) {

        counterUsed = 0;
        counterUnpossible = 0;
        vector<tiii> newArr;
        vi newStarting_times, newEnding_times, newWeights;
        for (int i = 0; i < N; ++i) {

            if (used_elements[i]) {
                counterUsed++;
            }
            else {
                int& length = get<2>(arr[i]);

                int usedheight = max_seg_query(starting_times[i]-minStartTime, ending_times[i]-minStartTime-1);                
            
                if (usedheight + length > maxL) {
                    // kein Platz mehr für das aktuelle Elemente
                    counterUnpossible++;
                }
                else {
                    newArr.push_back(arr[i]);
                    newStarting_times.push_back(starting_times[i]);
                    newEnding_times.push_back(ending_times[i]);
                    newWeights.push_back(weights[i]);
                }
            }
        }

        N -= counterUsed;
        N -= counterUnpossible;

        used_elements.clear();
        used_elements.resize(N, false);

        starting_times = newStarting_times;
        ending_times = newEnding_times;
        weights = newWeights;

        if (N == 0) {
            break;
        }
        else if (!firstTime) {
            if (counterUnpossible <= 0 and counterUsed <= 0) {
                break;
            }
        }


        // + + + Weighted Job Scheduling Problem + + + // 

        // Array das für den einen Stand i denjenigen Stand pre[i] speichert,
        // der vor dem akutellen Stand i endet,
        // damit die Endzeit von pre[i] < starting_times[i] gilt
        pre.resize(N);

        for (int i = 0; i < N; ++i) {
            // Index des vorherigen Standes wird gesucht,
            // dessen Endzeit noch vor der Startzeit des aktuellen Stands ist
            int index_pre = binary_search(ending_times, starting_times[i]);
            // index verringern, damit das Element gefunden wird, 
            // das noch vor der aktuellen Startzeit endet
            // index_pre --; 

            pre[i] = index_pre;
        }


        // base case für Dynamische Programmierung (rekursiv)
        // optimal[0] = 0;
        for (int i = 0; i < N; ++i) {
            int opt_i = getOpt(i);
            optimal.push_back(opt_i);
        }

        // was nun geschieht: 
        // - eine optimale Reihe wurde gefunden,
        //  -> diese wird nun gespeichert
        // - und die Höhen im Segmentbaum werden aktualisiert

        vi solution;
        // Backtracking der Lösung; (Lösung = Anordnung der Stände)
        for (int i = N-1; i >= 0;) {
            int following_getOpt;
            if (i > -1) following_getOpt = getOpt(i - 1);
            else following_getOpt = 0;

            if (weights[i] + getOpt(pre[i]) > following_getOpt) {
                solution.push_back(i);
                // cout << "sol\n";

                used_elements[i] = true;

                // es wird mit dem Vorgänger von i fortgesetzt
                i = pre[i];
            }
            else --i;
        }

        INCOME += optimal[N-1];
        optimal.clear();


        // Erhöhung der Höhen
        for (int i : solution) {
            int start = starting_times[i];
            int end = ending_times[i];
            int weight = weights[i];

            int& length = get<2>(arr[i]);

            cout << start << " " << end << " " << length << "\n";

            int aktHeight = max_seg_query(start-minStartTime, end-minStartTime-1);
            if (aktHeight<0) aktHeight = 0;

            // Segmentbaum für das Intervall [start; end] auf length + aktHeight setzen
            // length entspricht der Breite des aktuellen Standes
            // das Ende wird nicht eingeschlossen, sprich nicht erhöht, 
            // da es sich sonst mit der anschließenden Höhe eines Startzeitpunkt s, s=end, doppelt erhöht werden würde,
            // obwohl dies in der Realität nicht der Fall wäre --> nachfolgender Stand mit end1=start2
            apply(start-minStartTime, end-minStartTime-1, length+aktHeight);

        }

        firstTime = false;

    }

    cout << ">> gesamtes Einkommen beträgt " << INCOME;

    return 0;
}