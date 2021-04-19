#include <bits/stdc++.h>
using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;
typedef pair<int, int> pii;
typedef vector<pii> vpii;
typedef tuple<int, int, int> tiii;

#define what(x) cerr<<#x<<" is "<<x<<endl;

const int INF = numeric_limits<int>::max();

// gesamte Anzahl aller Stände, die sich angemeldet haben
int N;

// Länge der Straße
int maxL;


// Größe des Segmentbaumes, die im Laufe des Programms noch ermittelt wird
int sizeSegmentTree;

// neutrales Element, das im Segmentbaum benötigt wird,
// dessen assoziative Operation 'max' ist.
const int neutral_element = -INF;

// Vector, in dem der Segmentbaum gespeichert wird (eindimensional)
// und dessen dazugehörige, bereits ausgeführte Operationen des jeweiligen Knotens
// z.B. werden später alle Elemente in einem Intervall um einen bestimmen Wert erhöht werden müssen;
// dies wird im Array operation für alle direkten und indirekten Kinderknoten zwischengespeichert
vi segment_tree, operation;

// neutrale Operation
int id = 0;


bool sorting(tiii& first, tiii& second){
    int a1, a2, b1, b2, c1, c2;
    tie(a1, b1, c1) = first;
    tie(a2, b2, c2) = second;

    if (a1 == a2) {
        if (b1 == b2) {
            return c1 < c2;
        }
        return b1 < b2;
    }
    return a1 < a2;
}

bool sortBySecond(const tiii& a, const tiii& b) {
    int a1, a2, a3, b1, b2, b3;
    tie(a1, a2, a3) = a;
    tie(b1, b2, b3) = b;
    return a2 < b2;
}

// Methode zum bestimmen des Index, an dem der Wert value in das Array arr
// eingefügt werden müsste, damit es sortiert bleiben würde

// mit binärer Suche in log(n) Zeit, da das Array arr bereits sortiert ist
int binary_search(const vi& arr, const int value) {
    // Zu Beginn: obere Grenze N-1
    // untere Grenze 0
    int up = N-1, low = 0;

    // solange obere Grenze noch oberhalb
    // der unteren Grenze ist, läuft die Such noch;
    // ansonsten wird der Index zurückgegeben, wo value eingefügt werden muss
    while (low <= up) {

        // Mittelwert neu bilden
        int mid = (low + up) / 2;

        // value wurde gefunden
        if (arr[mid] == value) {
            return mid;
        }
        else if (arr[mid] < value) {
            // untere Grenze neu setzen
            low = mid + 1;
        }
        else {
            // obere Grenze neu setzen
            up = mid - 1;
        }
    }
    cout << "finish";

    // dasselbe Element wurde nicht in arr gefunden,
    // daher wird der Index zurückgegeben, wo es eingefügt werden müsste
    return up + 1;
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
void build(const vi& arr) {
    // 2-fache Größe die für den Segmentbaum berechnet wurde,
    // wird für den Segmentbaum benötigt
    segment_tree.resize(2*sizeSegmentTree);
    operation.resize(2*sizeSegmentTree);

    // hintere Hälfte des Segmentbaums initialisieren,
    // welcher alle Werte des übergebenen Arrays arr enthält
    for (int i = sizeSegmentTree; i < sizeSegmentTree*2; ++i) {
        segment_tree[i] = arr[i - sizeSegmentTree];

        // Initialisierung zu Beginn mit dem neutralen Element id
        operation[i] = id;
    }

    for (int i = sizeSegmentTree-1; i > 0; --i) {
        int l = segment_tree[left(i)];
        int r = segment_tree[right(i)];
        
        // Maximum von den 'vorausgehenden Knoten' l und r wird gebildet
        // und in i gespeichert
        segment_tree[i] = max(l, r);


        cout<<i<<" l "<<l<<" r "<<r<<" -> "<<segment_tree[i]<<"\n";


        // neutrales Element id wird zugewiesen
        operation[i] = id;
    }
}

// In der Update-Methode des Segmentsbaum werden alle Elemente in dem übergebenen Bereich
// mit dem übergebenen Wert und der Operation + aktualisiert
// Da die Operation 'Addition' kommutativ ist, wird im Segmentbaum keine Lazy Propagation benötigt
// void update(int j, int x, int y=neutral_element, int a=1, int b=N, int i=1) {
//     segment_tree[i] = max(y, segment_tree[i]);

//     if (a == b) {
//         segment_tree[i] = x;
//         return;
//     }
//     int m = (a + b)/2;

//     if (y <= m) {
//         update(j, x, y, a, m, left(i));
//     }
//     else {
//         update(j, x, y, m+1, b, right(i));
//     }

//     // i wird wieder mit dem Maximum aus dem linken und rechten vorhergehenden Knoten aktualisiert
//     segment_tree[i] = max(segment_tree[left(i)], segment_tree[right(i)]);
// }



// In der Apply-Methode des Segementbaums werden alle Elemente in dem übergebenen Bereich (von l bis r)
// um den übergebenen Wert x erhöht, d.h. x wird addiert
// Da die Operation 'Addition' kommutativ ist, wird im Segmentbaum keine Lazy Propagation benötigt
void apply(int l, int r, int x, int a=1, int b=sizeSegmentTree, int i=1) {
    
    // Aktualisieren der Werte um x
    if (l <= a and b <= r) {
        operation[i] = x + operation[i];
        segment_tree[i] = x + segment_tree[i];
        return;
    }

    // aktueller Knoten nicht mehr im angegebenen Intervall
    if (r < a or b < l) {
        return;
    }

    // Mittelwert bilden
    int m = ceil((a + b)/2);

    // linker und rechter Kinderknoten werden rekursiv aufgerufen,
    // damit deren Wert auch aktualisiert werden, 
    // solange sie auch im angebenen Intervall befinden
    apply(l, r, x, a, m, left(i));
    apply(l, r, x, m + 1, b, right(i));

    // Neuberechnung des aktuellen Knotens i unter Einbeziehung des Werts operation[i],
    // da operation[i] noch hinzugefügt werden muss
    segment_tree[i] = operation[i] + max(segment_tree[left(i)], segment_tree[right(i)]);
}

// Abfrage im Segmentbaum nach dem Ergebnis im Intervall von l bis r
// --> Abfrage des maximalen Elements darin
// Verwendung der binären Suche --> log(n) Zeit

// Zu Beginn des rekursiven Aufrufs bei der Wurzel des Segmentbaums
// anschlie0end wird der Baum herabgestiegen,
// um Segmente zu finden, die das Intervall [a; b] aufteilen

// int query(int l, int r, int a=1, int b=sizeSegmentTree, int i=1) {
    // Aktueller Knoten i steht für den Bereich [a; b]

//     // Rückgabe des aktuellen Werts des Knotens
//     if (l <= a and b <= r) {
//         return segment_tree[i];
//     }

//     // aktueller Knoten für den Bereich [a; b] nicht mehr im angegebenen Intervall
//     if (r < a or b < l) {
//         return neutral_element;
//     }

//     // Mittelwert bilden
//     int m = ceil((a + b)/2);

//     // maximaler Wert des linken und rechten Kinderknoten ermitteln,
//     // dabei muss auch noch der Wert von operation[i] hinzugefügt werden
//     what(operation[i]);
//     return operation[i] + max(query(l, r, a, m, left(i)), query(l, r, m+1, b, right(i)));
// }
int query(const int l, const int r, int a=1, int b=sizeSegmentTree, int i=1) {
    // Falls [a; b] ein Teilsegment des gesuchten Bereichs ist
    if (l <= a and b <= r) {
        cout<<"i "<< i << " a "<<a<<" b "<<b<<" ->" << segment_tree[i]<<"\n";
        return segment_tree[i];
    }
    if (r < a or b < l) {
        return neutral_element;
    }

    int m=ceil((a+b) /2);
    cout << "i "<<i<<" zu l: "<<left(i)<<" zu r "<<right(i)<<"\n";
    return max(query(l, r, a, m, left(i)), query(l, r, m+1, b, right(i)));
}


int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    // maximale Straßenlänge ist 1000 Meter
    maxL = 1000;

    cin >> N;

    vector<tiii> arr(N);
    for (int i = 0; i < N; ++i) {
    	// Beginn, Ende und Länge werden eingelesen
    	int a, b, c; 
    	cin >> a >> b >> c;

    	arr[i] = {a, b, c};
    }

    // Sortieren nach ihrem Endzeitpunkt
    sort(arr.begin(), arr.end(), sortBySecond);

    // sorting noch bisschen testen

    for (int i=0;i<N;++i) {
        int a, b, c;
        tie(a, b, c) = arr[i];
        cout<<a<<" bis "<<b<<" mit "<<c<<"\n";
    }

    // die Start- und Endzeiten werden in Arrays gespeichert,
    // nachdem sie sortiert wurden
    vi starting_times(N), ending_times(N);

    int minStartTime = INF, maxEndTime = 0;

    for (int i = 0; i < N; ++i) {
        int start, end, weight;
        tie(start, end, weight) = arr[i];
        
        starting_times[i] = start;
        ending_times[i] = end;

        minStartTime = min(minStartTime, start);
        maxEndTime = max(maxEndTime, end);
    }

    // Array das für den einen Stand i denjenigen Stand pre[i] speichert,
    // der vor dem akutellen Stand i endet,
    // damit die Endzeit von pre[i] < starting_times[i] gilt
    vi pre(N);


    // REMOVE
    // int index = binary_search(ending_times, 3);
    // index --;

    // what(index);


    // Unterteilung Stundenweise, beginnend von minStartTime bis maxEndTime
    // somit ist intervallSize = 1 (Stunde)
    int intervallSize = 1;


    // Array speichert die aktuelle Höhe zum aktuellen Intervall
    sizeSegmentTree = (maxEndTime - minStartTime) / intervallSize;
    what(minStartTime); what(maxEndTime); what(sizeSegmentTree);

    // Erstellen des Segmentbaumes, mit einem leeren Array,
    // da noch keine Stände festgelegt wurden

    //TODO enable agin
    // build(vi(sizeSegmentTree, 0));


    // Test segment tree
    vi test(sizeSegmentTree);
    for (int i=0; i < sizeSegmentTree; ++i) {
        test[i] = i*2;
    }   

    build(test);

    for (int i=0;i<sizeSegmentTree*2;++i) {
        what(segment_tree[i]);
    }

    int res = query(1, 2);
    what(res);

    int res2 = query(2, 4);
    what(res2);

    int res3 = query(3, 5);
    what(res3);

    int res4 = query(0, 1);
    what(res4);


    for (int i = 0; i < N; ++i) {
        // Index des vorherigen Standes wird gesucht,
        // dessen Endzeit noch vor der Startzeit des aktuellen Stands ist
        int index_pre = binary_search(ending_times, starting_times[i]);
        // index verringern, damit das Element gefunden wird, 
        // das noch vor der aktuellen Startzeit endet
        index_pre --; 

        pre[i] = index_pre;
    }






    vi optimal(N);

    // base case für Dynamische Programmierung (rekursiv)
    optimal[0] = 0;












    return 0;
}