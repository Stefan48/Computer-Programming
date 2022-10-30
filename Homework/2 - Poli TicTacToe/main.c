// Popovici Stefan 313CA

#include <stdio.h>
#define N 10
#define MAX 100

int insert(char t[][MAX], int D, char p, int l, int c, int* il, int* ic)
// table,Dimension,player,line,column,insertion line,insertion column
// efectueaza mutarea corespunzatoare
// il,ic - celula in care se efectueaza insertia pe tabla de joc
// poate fi optimizata prin memorarea ultimei celule ocupate
// in ordinea algoritmului de parcurgere
{
    if (l < 0 || c < 0 || l >= D || c >= D)
        printf("INVALID INDEX\n");
    else if (t[l][c] != '-')
        printf("NOT AN EMPTY CELL\n");
    else
    {
        t[l][c] = p;
        *il = l;
        *ic = c;
        return 0;
    }
    int posl, posc;
    // position-line,position-column
    // variabile cu ajutorul carora efectuam parcurgerile necesare pe diagonale
    // Parcurgerea diagonalei principale:
    posl = 0;
    while (posl < D)
    {
        if (t[posl][posl] == '-')
        {
            t[posl][posl] = p;
            *il = posl;
            *ic = posl;
            return 0;
        }
        posl++;
    }
    int i = 1; // offset (fata de diagonala principala)
    while (i < D)
    {
        posl = 0;
        posc = i;
        while (posl < D && posc < D)
        {
            if (t[posl][posc] == '-')
            {
                t[posl][posc] = p;
                *il = posl;
                *ic = posc;
                return 0;
            }
            posl++;
            posc++;
        }
        posl = i;
        posc = 0;
        while (posl < D && posc < D)
        {
            if (t[posl][posc] == '-')
            {
                t[posl][posc] = p;
                *il = posl;
                *ic = posc;
                return 0;
            }
            posl++;
            posc++;
        }
        i++;
    }
    // Daca nu am gasit nicio celula libera:
    printf("FULL BOARD\n");
    *il = -1;
    *ic = -1;
    return 1;
}
int miniwin(char t[][MAX], int d, char p, int l, int c, char m[][N])
// table,dimension,player,line,column,macroboard
// verifica daca mutarea curenta a condus la un win in miniboardul respectiv
{
    int L, C; // L,C={0;...;d-1}
    // pozitia miniboardului in care facem verificarea
    L = l / d;
    C = c / d;
    if (m[L][C] != '-')
        return 0;
    // daca miniboardul fusese deja castigat anterior
    m[L][C] = p;
    int llu = L * d, clu = C * d;
    // line-left-up, column-left-up
    int i, j;
    int ok;
    // Verificarea coloanei:
    ok = 1;
    for (i = llu; i < llu + d; ++i)
        if (t[i][c] != p)
        {
            ok = 0;
            break;
        }
    if (ok)
        return 1;
    // Verificarea liniei:
    ok = 1;
    for (j = clu; j < clu + d; ++j)
        if (t[l][j] != p)
        {
            ok = 0;
            break;
        }
    if (ok)
        return 1;
    // Verificarea diagonalei principale:
    ok = 1;
    j = clu;
    for (i = llu; i < llu + d; ++i)
    {
        if (t[i][j] != p)
        {
            ok = 0;
            break;
        }
        j++;
    }
    if (ok)
        return 1;
    // Verificarea diagonalei secundare:
    ok = 1;
    j = clu + d - 1;
    for (i = llu; i < llu + d; ++i)
    {
        if (t[i][j] != p)
        {
            ok = 0;
            break;
        }
        j--;
    }
    if (ok)
        return 1;
    // Daca mutarea nu a condus la castigarea miniboardului:
    m[L][C] = '-';
    return 0;
}
void check_winner(char m[][N], int d)
// macroboard,dimension
// verifica cine a castigat meciul (macroboardul)
{
    int i, j;
    int ok;
    // Verificarea diagonalei principale:
    if (m[0][0] != '-')
    {
        ok = 1;
        for (i = 1; i < d; ++i)
            if (m[i][i] != m[i - 1][i - 1])
            {
                ok = 0;
                break;
            }
        if (ok)
        {
            printf("%c won\n", m[0][0]);
            return;
        }
    }
    // Verificarea diagonalei secundare:
    if (m[0][d - 1] != '-')
    {
        ok = 1;
        j = d - 2;
        for (i = 1; i < d; ++i)
        {
            if (m[i][j] != m[i - 1][j + 1])
            {
                ok = 0;
                break;
            }
            j--;
        }
        if (ok)
        {
            printf("%c won\n", m[0][d - 1]);
            return;
        }
    }
    // Daca victoria nu s-a petrecut prin completarea
    // vreunei diagonale de unul dintre jucatori
    // inseamna ca ne aflam in situatia in care fiecare
    // ar fi putut completa una sau mai multe
    // linii sau coloane, caz in care jocul s-ar termina cu remiza,
    // ceea ce verificam in continuare:
    int wonX = 0, won0 = 0;
    // Verificarea liniilor:
    for (i = 0; i < d; ++i)
        if (m[i][0] != '-')
        {
            ok = 1;
            for (j = 1; j < d; ++j)
                if (m[i][j] != m[i][j - 1])
                {
                    ok = 0;
                    break;
                }
            if (ok)
            {
                if (m[i][0] == 'X')
                    wonX++;
                else
                    won0++;
            }
        }
    if (wonX)
    {
        if (won0)
        {
            printf("Draw again! Let's play darts!\n");
            return;
        }
        printf("X won\n");
        return;
    }
    else if (won0)
    {
        printf("0 won\n");
        return;
    }
    // Verificarea coloanelor:
    for (j = 0; j < d; ++j)
        if (m[0][j] != '-')
        {
            ok = 1;
            for (i = 1; i < d; ++i)
                if (m[i][j] != m[i - 1][j])
                {
                    ok = 0;
                    break;
                }
            if (ok)
            {
                if (m[0][j] == 'X')
                    wonX++;
                else
                    won0++;
            }
        }
    if (wonX)
    {
        if (won0)
        {
            printf("Draw again! Let's play darts!\n");
            return;
        }
        printf("X won\n");
        return;
    }
    else if (won0)
    {
        printf("0 won\n");
        return;
    }
    printf("Draw again! Let's play darts!\n");
    return;
}

int main()
{
    int dim, Dim, nr_moves;
    // dimensiune mini(macro)board,dimensiune tabla completa de joc,nr. mutari
    int line, col, m, ins_line, ins_col;
    char t[MAX][MAX], mb[N][N];
    // table, macroboard
    int table_full = 0; // 0 or 1
    char next = 'X', player;
    int turnsX = 0, turns0 = 0, winsX = 0, wins0 = 0;
    double attention;

    scanf("%d%d", &dim, &nr_moves);
    Dim = dim * dim;
    for (line = 0; line < Dim; ++line)
        for (col = 0; col < Dim; ++col)
            t[line][col] = '-';
    for (line = 0; line < dim; ++line)
        for (col = 0; col < dim; ++col)
            mb[line][col] = '-';
    for (m = 1; m <= nr_moves; ++m)
    {
        scanf(" %c%d%d", &player, &line, &col);
        if (!table_full)
        {
            if (player == next)
            {
                table_full = insert(t, Dim, player, line, col, &ins_line, &ins_col);
                // Verificam daca mutarea a condus la castigarea unui miniboard
                // pe baza celulei in care s-a facut insertia:
                if (ins_line != -1)
                    if (miniwin(t, dim, player, ins_line, ins_col, mb))
                        if (ins_line == line && ins_col == col)
                        // daca mutarea a fost valida si nu "automatizata"
                        {
                            if (player == 'X')
                                winsX++;
                            else
                                wins0++;
                        }
                if (player == 'X')
                {
                    turnsX++;
                    next = '0';
                }
                else
                {
                    turns0++;
                    next = 'X';
                }
            }
            else
                printf("NOT YOUR TURN\n");
        }
        else
            break;
        // mutarile ramase dupa umplerea tablei de joc nu conteaza
        // nici pentru afisarea mesajelor corespunzatoare
        // nici pentru calculul coeficientilor de atentie
    }
    // Afisarea macroboardului:
    for (line = 0; line < dim; ++line)
    {
        for (col = 0; col < dim; ++col)
            printf("%c", mb[line][col]);
        printf("\n");
    }
    // Afisarea rezultatului meciului:
    check_winner(mb, dim);
    // Calculul si afisarea "atentiei" jucatorilor:
    if (turnsX == 0)
        printf("X N/A\n");
    else
    {
        attention = (double)(winsX) / turnsX;
        printf("X %.10lf\n", attention);
    }
    if (turns0 == 0)
        printf("0 N/A\n");
    else
    {
        attention = (double)(wins0) / turns0;
        printf("0 %.10lf\n", attention);
    }

    return 0;
}