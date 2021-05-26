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

        // // value wurde gefunden
        // if (arr[mid] == value) {
        //     return mid;
        // }
        // else if (arr[mid] < value) {
        //     // untere Grenze neu setzen
        //     low = mid + 1;
        // }
        // else {
        //     // obere Grenze neu setzen
        //     up = mid - 1;
        // }
    }
    cout << "finish";

    // dasselbe Element wurde nicht in arr gefunden,
    // daher wird der Index zurückgegeben, wo es eingefügt werden müsste
    return low-1;
    //return up+1;
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


// folgende Methode aktualisiert beide Segmentbäume (min und max):
// In der Apply-Methode des Segementbaums werden alle Elemente in dem übergebenen Bereich (von l bis r)
// auf den übergebenen Wert gesetzt
// da die Operation 'Gleichsetzen' kommutativ ist, wird im Segmentbaum Lazy Propagation benötigt
void apply(int l, int r, int x, int y=id, int a=1, int b=sizeSegmentTree, int i=1) {    
    max_segment_tree[i] = y;

    y = operation[i];
    operation[i] = id;

    if (l <= a and b <= r) {
        operation[i] = x;
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

int max_seg_query(const int l, const int r, int a=1, int b=sizeSegmentTree, int i=1) {
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

    sizeSegmentTree = 16;
    build(vi(sizeSegmentTree, 0));

    apply(0, sizeSegmentTree, 0);
    // apply(2, 3, 5);
    // apply(3, 3, 3);
    // apply(0, 0, 4);

    int max1 = max_seg_query(0, 0);
    what(max1);




    return 0;


}