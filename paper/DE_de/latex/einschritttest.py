# Referenzimplementierung der Algorithmen aus dem Paper
# "Auswertung endlicher Bitfolgen".
#
#  - einschritt(bits):      Einschritttest, Variante fuer Bitfolgen
#                           (Algorithmus 2)
#  - einschritt_laeufe(r):  Einschritttest, Variante fuer Folgen von
#                           Laeufen (Algorithmus 1)
#  - b_n_iterativ(N):      iterativer Generator fuer B_N
#                           (Algorithmus 3)
#  - b_n_rekursiv(r):      rekursiver Uebergang B_N -> B_{N+1}
#                           (Algorithmus 4)
#  - in_klasse(r, N):       Mitgliedschaftstest fuer B_N
#
# Alle Funktionen arbeiten auf Folgen von Bits (0/1) bzw. auf
# Folgen von Laufaengen. Rueckgabe jeweils die kumulierte
# Verteilung bzw. die Folge der Laufaengen.


def einschritt(bits):
    """Kumulierte Verteilung der Läufe und Teilläufe einer Bitfolge.

    Zählt bei jedem Bit der Folge alle Stellen 1..z, wobei z die
    Länge des bis zu diesem Bit begonnenen Laufs ist.
    """
    L, z, vorgaenger = [], 0, None
    for b in bits:
        if b == vorgaenger:
            z += 1
        else:
            z = 1
            vorgaenger = b
        for i in range(z):
            if i == len(L):
                L.append(0)
            L[i] += 1
    return L


def einschritt_laeufe(laeufe):
    """Kumulierte Verteilung aus der Folge der Lauflängen.

    Jeder Lauf der Länge l erhöht die Stelle i (1 <= i <= l)
    um l - i + 1.
    """
    L = []
    for l in laeufe:
        while len(L) < l:
            L.append(0)
        for i in range(1, l + 1):
            L[i - 1] += l - i + 1
    return L


def b_n_iterativ(N):
    """Klassenvertreter von B_N als Folge von Lauflängen (Algorithmus 3).

    Ein Lauf der Länge N, dann für i = N-2, ..., 1 je 2^(N-i-2)
    Läufe der Länge i.
    """
    laeufe = [N]
    for i in range(N - 2, 0, -1):
        laeufe += [i] * (2 ** (N - i - 2))
    return laeufe


def b_n_rekursiv(laeufe):
    """Übergang B_N -> B_{N+1} (Algorithmus 4).

    Erweitert jeden Lauf um ein Bit und ergänzt 2^(N-2) Läufe der
    Länge 1 (N = laengster Lauf der Eingabe). Die Fälle N = 1 und
    N = 2 erfordern keine Ergänzung.
    """
    N = max(laeufe)
    ergaenzung = 0 if N < 2 else 2 ** (N - 2)
    return [l + 1 for l in laeufe] + [1] * ergaenzung


def in_klasse(laeufe, N):
    """Mitgliedschaftstest: liegt die Folge der Lauflängen in B_N?

    Prüft, ob die kumulierte Verteilung exakt exponentiell fällt:
    l_n = 2^(N-n) für alle 1 <= n <= N.
    """
    L = einschritt_laeufe(laeufe)
    return len(L) == N and all(
        L[n - 1] == 2 ** (N - n) for n in range(1, N + 1)
    )


def laeufe_zu_bits(laeufe, start=0):
    """Realisierung einer Folge von Lauflängen als Bitfolge."""
    bits, b = [], start
    for l in laeufe:
        bits += [b] * l
        b = 1 - b
    return bits


def laeufe_von(bits):
    """Lauflängen einer Bitfolge."""
    laeufe, z = [], 1
    for i in range(1, len(bits)):
        if bits[i] == bits[i - 1]:
            z += 1
        else:
            laeufe.append(z)
            z = 1
    laeufe.append(z)
    return laeufe


if __name__ == "__main__":
    # Selbsttest: Generatoren erzeugen Elemente aus B_N,
    # beide Varianten des Einschritttests stimmen ueberein.
    import random

    for N in range(1, 17):
        r = b_n_iterativ(N)
        assert in_klasse(r, N), N
        assert einschritt(laeufe_zu_bits(r)) == einschritt_laeufe(r), N
        if N >= 2:
            assert in_klasse(b_n_rekursiv(r), N + 1), N

    random.seed(0)
    for _ in range(20):
        bits = [random.randint(0, 1) for _ in range(5000)]
        assert einschritt(bits) == einschritt_laeufe(laeufe_von(bits))

    print("Alle Selbsttests bestanden.")
