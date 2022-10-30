// Popovici Stefan 313CA

#include <stdio.h>
#include <math.h>

long long unsigned pow100(int exp)
// returneaza 100 la puterea exp
// implementata cu switch pentru un strop de eficienta
{
    long long unsigned p;
    switch (exp)
    {
        case 0:
            p = 1;
            break;
        case 1:
            p = 100;
            break;
        case 2:
            p = 10000;
            break;
        case 3:
            p = 1000000;
            break;
        case 4:
            p = 100000000;
            break;
        case 5:
            p = 10000000000;
            break;
        case 6:
            p = 1000000000000;
            break;
        case 7:
            p = 100000000000000;
            break;
        case 8:
            p = 10000000000000000;
            break;
        case 9:
            p = 1000000000000000000;
            break;
        default:
            break;
    }
    return p;
}
// O functie ce citeste prin parametru numerul de orine
// si returneaza tot prin parametru numarul de stelute ce trebuie afisate:
void nr_and_stars(int* nrord, int* nrstars)
{
    // nrord- numar de ordine
    // nrstars - numarul de stelute ce trebuie afisate (maxim 10)
    int nrviz, nrtot;
    // nrviz - numarul de episoade vizionate
    // nrtot - numarul total de episoade
    float proc; // procentul de episoade vizonate
    float fl; // floor
    scanf("%d%d%d", nrord, &nrviz, &nrtot);
    if (nrviz > nrtot)
        nrviz = nrtot;
    // Calculam procentul:
    proc = (float)nrviz / nrtot * 10.0f;
    fl = floor(proc);
    if (proc - fl < 0.5f)
        proc = fl;
    else
        proc = ceil(proc);
    *nrstars = proc;
}
void code_to_stars(long long unsigned* cod, int* nrstars)
// Inversul procesului de codificare
{
    *nrstars = *cod % 10;
    *cod /= 10;
    *nrstars += 10 * (*cod % 10);
    *cod /= 10;
    if (*nrstars == 0)
        *nrstars = 11;
    else if (*nrstars == 11)
        *nrstars = 0;
}
// O functie care afiseaza * sau . (folosita la afisarea histogramei):
void print_star(short int* nl, int i, int nrstars)
{
    if (nrstars == 11)
        return;
    if (*nl == 0)
        printf(" ");
    else
        *nl = 0;
    if (i > nrstars)
        printf(".");
    else
        printf("*");
}

int main()
{
    short int newline; // true/false
    // variabila folosita la afisare pentru afisarea corecta a spatiilor
    int n, i, nrord;
    int nrstars0, nrstars1, nrstars2, nrstars3, nrstars4;
    int nrstars5, nrstars6, nrstars7, nrstars8, nrstars9;
    // numarul de stelute ce trebuie afisat pentru fiecare coloana in parte
    unsigned long long code = 0;
    // numar de 20 de cifre; exista maxim 10 seriale deci e ok
    // 0 - nu exista
    // 11 - 0%
    // altfel (t) - t/10
    scanf("%d", &n);
    // Memoram datele necesare codificandu-le in variabila 'code':
    for (i = 0; i < n; ++i)
    {
        nr_and_stars(&nrord, &nrstars0);
        if (nrstars0)
            code += nrstars0 * pow100(nrord);
        else
            code += 11 * pow100(nrord);
    }
    // In continuare vom scrie excesiv de mult pentru ca avem alergie la vectori
    // As fi putut sa prelucrez codul la afisarea fiecarei linii
    // dar nu la fel de eficient dpdv al timpului de executie
    // Transformam codul creat, efectuand astfel si partea de sortare:
    code_to_stars(&code, &nrstars0);
    code_to_stars(&code, &nrstars1);
    code_to_stars(&code, &nrstars2);
    code_to_stars(&code, &nrstars3);
    code_to_stars(&code, &nrstars4);
    code_to_stars(&code, &nrstars5);
    code_to_stars(&code, &nrstars6);
    code_to_stars(&code, &nrstars7);
    code_to_stars(&code, &nrstars8);
    code_to_stars(&code, &nrstars9);
    // Afisarea histogramei:
    i = 10;
    while (i > 0)
    {
        newline = 1;
        print_star(&newline, i, nrstars0);
        print_star(&newline, i, nrstars1);
        print_star(&newline, i, nrstars2);
        print_star(&newline, i, nrstars3);
        print_star(&newline, i, nrstars4);
        print_star(&newline, i, nrstars5);
        print_star(&newline, i, nrstars6);
        print_star(&newline, i, nrstars7);
        print_star(&newline, i, nrstars8);
        print_star(&newline, i, nrstars9);
        --i;
        printf("\n");
    }
    // Afisarea numerelor de ordine:
    // Ne complicam putin, dar avem grija sa nu afisam un spatiu in plus
    newline = 0;
    if (nrstars0 != 11)
    {
        printf("0");
        newline = 1;
    }
    if (nrstars1 != 11)
    {
        if (newline)
            printf(" ");
        else
            newline = 1;
        printf("1");
    }
    if (nrstars2 != 11)
    {
        if (newline)
            printf(" ");
        else
            newline = 1;
        printf("2");
    }
    if (nrstars3 != 11)
    {
        if (newline)
            printf(" ");
        else
            newline = 1;
        printf("3");
    }
    if (nrstars4 != 11)
    {
        if (newline)
            printf(" ");
        else
            newline = 1;
        printf("4");
    }
    if (nrstars5 != 11)
    {
        if (newline)
            printf(" ");
        else
            newline = 1;
        printf("5");
    }
    if (nrstars6 != 11)
    {
        if (newline)
            printf(" ");
        else
            newline = 1;
        printf("6");
    }
    if (nrstars7 != 11)
    {
        if (newline)
            printf(" ");
        else
            newline = 1;
        printf("7");
    }
    if (nrstars8 != 11)
    {
        if (newline)
            printf(" ");
        else
            newline = 1;
        printf("8");
    }
    if (nrstars9 != 11)
    {
        if (newline)
            printf(" ");
        else
            newline = 1;
        printf("9");
    }
    printf("\n");
    return 0;
}
