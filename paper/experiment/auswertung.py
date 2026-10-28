#!/usr/bin/env python3
"""Auswertung der /dev/random-Sammlung: Einschritttest.

Liest alle Dateien eines Verzeichnisses (namenssortiert, also bei
Zeitstempel-Dateinamen chronologisch), verkettet sie zu einer
Bitfolge (je Byte das höchstwertige Bit zuerst) und berechnet die
kumulierte Verteilung der Läufe und Teilläufe (Einschritttest,
Algorithmus 1 des Papers). Läuft werden über Dateigrenzen hinweg
fortgesetzt; dadurch ist die Folge eine einzige Bitfolge.

Nutzung:  python3 auswertung.py [datenverzeichnis]

Ausgabe:  Bitanzahl M, Häufigkeiten l_n, log2(l_n), theoretische
          Erwartung M*2^(1-n) sowie der Chi-Quadrat-Anpassungstest
          gegen die geometrische Verteilung der Lauflängen.
"""

import math
import os
import sys

# --- Lookup-Tabellen je Byte (Bits MSB-zuerst) -------------------
# RUNS_IN[b]   : Laufaengen innerhalb des Bytes b
# FIRST[b]     : erstes Bit des Bytes b
# LAST[b]      : letztes Bit des Bytes b
RUNS_IN = []
FIRST = bytearray(256)
LAST = bytearray(256)
for b in range(256):
    bits = [(b >> (7 - i)) & 1 for i in range(8)]
    FIRST[b] = bits[0]
    LAST[b] = bits[7]
    runs = [1]
    for i in range(1, 8):
        if bits[i] == bits[i - 1]:
            runs[-1] += 1
        else:
            runs.append(1)
    RUNS_IN.append(tuple(runs))


def laufhistogramm(verzeichnis):
    """Zählt die Läufe je Länge über alle Dateien (sortiert nach Name)."""
    dateien = sorted(
        f for f in os.listdir(verzeichnis)
        if os.path.isfile(os.path.join(verzeichnis, f))
    )
    hist = {}
    cur = 0            # aktuelle offene Lauflänge
    last_bit = -1      # letztes Bit der Folge (-1: noch keines)
    n_dateien = 0
    n_bytes = 0
    for name in dateien:
        n_dateien += 1
        with open(os.path.join(verzeichnis, name), 'rb') as f:
            while True:
                block = f.read(1 << 20)
                if not block:
                    break
                n_bytes += len(block)
                for byte in block:
                    runs = RUNS_IN[byte]
                    if cur == 0:
                        # Start der Folge
                        for r in runs[:-1]:
                            hist[r] = hist.get(r, 0) + 1
                        cur = runs[-1]
                    elif FIRST[byte] == last_bit:
                        if len(runs) >= 2:
                            # offener Lauf endet innerhalb des Bytes
                            hist[cur + runs[0]] = hist.get(cur + runs[0], 0) + 1
                            for r in runs[1:-1]:
                                hist[r] = hist.get(r, 0) + 1
                            cur = runs[-1]
                        else:
                            cur += runs[0]
                    else:
                        hist[cur] = hist.get(cur, 0) + 1
                        for r in runs[:-1]:
                            hist[r] = hist.get(r, 0) + 1
                        cur = runs[-1]
                    last_bit = LAST[byte]
    if cur:
        hist[cur] = hist.get(cur, 0) + 1
    return hist, n_dateien, n_bytes


def einschritt_aus_histogramm(hist, M):
    """Kumulierte Verteilung l_n = Summe_k (k-n+1) * g_k aus dem Histogramm."""
    P = max(hist)
    L = [0] * P
    for k, g in hist.items():
        for n in range(1, k + 1):
            L[n - 1] += (k - n + 1) * g
    return L


def main():
    verzeichnis = sys.argv[1] if len(sys.argv) > 1 else 'daten'
    hist, n_dateien, n_bytes = laufhistogramm(verzeichnis)
    M = 8 * n_bytes
    L = einschritt_aus_histogramm(hist, M)

    # Konsistenz: l_1 muss exakt M sein (jedes Bit gehört zu genau
    # einem Teillauf der Laenge 1).
    assert L[0] == M, (L[0], M)

    print(f"Dateien:          {n_dateien}")
    print(f"Bytes:            {n_bytes}")
    print(f"Bits M:           {M}")
    print(f"Längster Lauf P:  {len(L)}")
    print()
    print(" n |      l_n |  log2(l_n) |  M*2^(1-n)")
    print("---+----------+-----------+----------")

    # Chi-Quadrat gegen die theoretische Verteilung l_n = M*2^(1-n).
    # Nur Bins mit Erwartung >= 10 (Approximationsbedingung);
    # Rest zum letzten Bin hinzuaddieren.
    chi2 = 0.0
    df = 0
    for n in range(1, len(L) + 1):
        e = M * 2.0 ** (1 - n)
        if e >= 10:
            chi2 += (L[n - 1] - e) ** 2 / e
            df += 1
        rest = L[n - 1] if e < 10 else None
        if n <= 25:
            print(f"{n:2d} | {L[n - 1]:8d} | "
                  f"{math.log2(L[n - 1]):9.2f} | {e:8.0f}")

    print()
    print(f"Chi-Quadrat:      {chi2:.1f} bei {df} Freiheitsgraden "
          f"(Erwartungswert der Statistik: {df})")
    print(f"Verhaeltnis:      {chi2 / df:.2f} "
          f"(Werte um 1 sprechen fuer die geometrische Verteilung)")


if __name__ == '__main__':
    main()
