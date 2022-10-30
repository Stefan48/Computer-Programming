// Popovici Stefan 313CA

#include <stdio.h>
#include <math.h>

int main()
{
    int n, i; // n - numarul de valori de citit
    int neg = 0; // retine daca am citit sau nu numere negative
    int contMin = 1, contMax = 1;
    // numarul de aparitii pentru valorile minim si maxim
    int l = 1, lMax = 1;
    // lungimea secventei curente si a celei mai lungi secvente crescatoare
    float x, sumA = 0.0f, prodG = 1.0f, sumH = 0.0f, sumP = 0.0f, sum_stdev = 0.0f;
    // x - valorile citite
    // sum.. - sumele pe care le calculam in vederea obtinerii mediilor cerute
    float MA, MG, MH, MP, stdev; // mediile cerute si abaterea standard
    float Min = 0.0f, Max = 0.0f; // valorile minim si maxim
    float ult;
    // retine valoarea citita anterior
    // (pentru a verifica monotonia secventei curente)
    scanf("%d", &n);
    // i=1
    scanf("%f", &x);
    // Citim primul numar si efectuam initializarile necesare
    sumA += x;
    if (x < 0.0f)
        neg = 1;
    else if (!neg)
        prodG *= x;
    sumH += 1.0 / x;
    sumP += x * x;
    Min = Max = ult = x;
    // In continuare citim restul numerelor si calculam
    for (i = 2; i <= n; i++)
    {
        scanf("%f", &x);
        sumA += x;
        if (x < 0.0f)
            neg = 1;
        else if (!neg)
            prodG *= x;
        sumH += 1.0 / x;
        sumP += x * x;
        if (x < Min)
        {
            Min = x;
            contMin = 1;
        }
        else if (x == Min)
            contMin++;
        if (x > Max)
        {
            Max = x;
            contMax = 1;
        }
        else if (x == Max)
            contMax++;
        if (x >= ult)
        {
            l++;
            if (l > lMax)
                lMax = l;
        }
        else
            l = 1;
        ult = x;
    }
    // Finalizarea calculelor si afisarea:
    MA = sumA / n;
    printf("%.4f\n", MA);
    if (neg)
        printf("-\n");
    else
    {
        MG = pow(prodG, (float)1.0f / n);
        printf("%.4f\n", MG);
    }
    MH = n / sumH;
    printf("%.4f\n", MH);
    MP = sqrt(sumP / n);
    printf("%.4f\n", MP);
    sum_stdev = sumP + MA * (n * MA - 2.0f * sumA);
    stdev = sqrt(sum_stdev / n);
    printf("%.4f\n", stdev);
    printf("%.4f %d\n%.4f %d\n%d\n", Min, contMin, Max, contMax, lMax);
    return 0;
}
