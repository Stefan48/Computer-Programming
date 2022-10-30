// Popovici Stefan 313CA
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define RADIUS_MAX 15
#define DIM_MAX 2*RADIUS_MAX+1
#define ELVES_MAX 150
#define NAMESIZE_MAX 20
#define MOVESCOUNT_MAX 50

FILE *fin,*fout;

typedef struct cell
{
    int height;
    int gloves;
    int in_radius;
    int occupied; // how many elves on cell
    int id[5]; // ids of those elves
}Cell;

typedef struct player
{
    char name[NAMESIZE_MAX];
    int x,y;
    int hp;
    int stamina;
    int dry;
    int dmg;
    int wins;
    
}Player;

void swap(int *a,int *b)
{
    if(*a!=*b)
    {
        *a^=*b;
        *b^=*a;
        *a^=*b;
    }
}
double distance(double x1,double y1,double x2,double y2)
{
    return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}
int inRadius(int posx,int posy,int centerx,int centery,int radius)
{
    return distance((double)posx,(double)posy,(double)centerx,(double)centery)<=(double)radius;
}
void endGame(Player elf[],int no_of_players)
{
    for(int i=0;i<no_of_players;++i) if(elf[i].dry)
    {
        fprintf(fout,"%s has won.\n",elf[i].name);
        break;
    }
    exit(EXIT_SUCCESS);
}
void initializeMap(Cell map[][DIM_MAX],int *radius,int *no_of_players,int *active_players)
{
    fscanf(fin,"%d%d",radius,no_of_players);
    *active_players=*no_of_players;
    int dim=2*(*radius)+1;
    int i,j;
    for(i=0;i<dim;++i)
    {
        for(j=0;j<dim;++j)
        {
            fscanf(fin,"%d%d",&map[i][j].height,&map[i][j].gloves);
            // Center of glacier is (radius,radius).
            map[i][j].in_radius=inRadius(i,j,*radius,*radius,*radius);
            map[i][j].occupied=0;
        }
    }
}
void landOnCell(Cell map[][DIM_MAX],Player *elf,int id)
{
    map[elf->x][elf->y].id[map[elf->x][elf->y].occupied]=id;
    map[elf->x][elf->y].occupied++;
    elf->dmg=map[elf->x][elf->y].gloves;
    map[elf->x][elf->y].gloves=0;
}
void initializePlayers(Cell map[][DIM_MAX],Player elf[],int no_of_players,int *active_players)
{
    char str[20];
    for(int i=0;i<no_of_players;++i)
    {
        fscanf(fin,"%s",str);
        strcpy(elf[i].name,str);
        fscanf(fin,"%d%d%d%d",&elf[i].x,&elf[i].y,&elf[i].hp,&elf[i].stamina);
        elf[i].dry=1;
        elf[i].dmg=0;
        elf[i].wins=0;
        // verify if in radius
        if(map[elf[i].x][elf[i].y].in_radius)
            landOnCell(map,&elf[i],i);
        else
        {
            elf[i].hp=0;
            elf[i].dry=0;
            (*active_players)--;
            fprintf(fout,"%s has missed the glacier.\n",elf[i].name);
        }
    }
}
// function returns winner:
int playersBattle(Player elf[],int id1,int id2)
{
    if(elf[id1].dmg==0)
    {
        // if no battle takes place
        if(elf[id2].dmg==0) return 0;
        // if first elf has no gloves
        elf[id1].hp=0;
        elf[id1].dry=0;
        elf[id2].stamina+=elf[id1].stamina;
        elf[id1].stamina=0;
        elf[id2].wins++;
        fprintf(fout,"%s sent %s back home.\n",elf[id2].name,elf[id1].name);
        return 2;
    }
    // if second elf has no gloves
    if(elf[id2].dmg==0)
    {
        elf[id2].hp=0;
        elf[id2].dry=0;
        elf[id1].stamina+=elf[id2].stamina;
        elf[id2].stamina=0;
        elf[id1].wins++;
        fprintf(fout,"%s sent %s back home.\n",elf[id1].name,elf[id2].name);
        return 1;
    }
    // real battle
    // calculating the number of hits required to take down each elf
    int hits1=elf[id2].hp/elf[id1].dmg;
    if(elf[id2].hp%elf[id1].dmg) hits1++;
    int hits2=elf[id1].hp/elf[id2].dmg;
    if(elf[id1].hp%elf[id2].dmg) hits2++;
    // If elves have equal stamina, the last one to enter the cell attacks first.
    // if second elf attacks first
    if(elf[id1].stamina<=elf[id2].stamina)
    {
        if(hits1>=hits2)
        {
            elf[id1].hp=0;
            elf[id1].dry=0;
            elf[id2].hp-=(hits2-1)*elf[id1].dmg;
            elf[id2].stamina+=elf[id1].stamina;
            elf[id1].stamina=0;
            elf[id2].wins++;
            fprintf(fout,"%s sent %s back home.\n",elf[id2].name,elf[id1].name);
            return 2;
        }
        // if(hits1<hits2)
        elf[id2].hp=0;
        elf[id2].dry=0;
        elf[id1].hp-=hits1*elf[id2].dmg;
        elf[id1].stamina+=elf[id2].stamina;
        elf[id2].stamina=0;
        elf[id1].wins++;
        fprintf(fout,"%s sent %s back home.\n",elf[id1].name,elf[id2].name);
        return 1;
    }
    // if first elf attacks first
    if(hits1<=hits2)
    {
        elf[id2].hp=0;
        elf[id2].dry=0;
        elf[id1].hp-=(hits1-1)*elf[id2].dmg;
        elf[id1].stamina+=elf[id2].stamina;
        elf[id2].stamina=0;
        elf[id1].wins++;
        fprintf(fout,"%s sent %s back home.\n",elf[id1].name,elf[id2].name);
        return 1;
    }
    // if(hits1>hits2)
    elf[id1].hp=0;
    elf[id1].dry=0;
    elf[id2].hp-=hits2*elf[id1].dmg;
    elf[id2].stamina+=elf[id1].stamina;
    elf[id1].stamina=0;
    elf[id2].wins++;
    fprintf(fout,"%s sent %s back home.\n",elf[id2].name,elf[id1].name);
    return 2;
}
void leaveCell(Cell map[][DIM_MAX],Player elf[],int id)
{
    int j=0;
    while(map[elf[id].x][elf[id].y].id[j]!=id) j++;
    while(j<map[elf[id].x][elf[id].y].occupied-1)
    {
        map[elf[id].x][elf[id].y].id[j]=map[elf[id].x][elf[id].y].id[j+1];
        j++;
    }
    map[elf[id].x][elf[id].y].occupied--;
}
int moveUp(Cell map[][DIM_MAX],Player elf[],int id)
{
    // if the elf falls off the glacier
    if(elf[id].x==0)
    {
        elf[id].hp=0;
        elf[id].dry=0;
        leaveCell(map,elf,id);
        fprintf(fout,"%s fell off the glacier.\n",elf[id].name);
        return 0;
    }
    // if not enough stamina
    if(elf[id].stamina<abs(map[elf[id].x][elf[id].y].height-map[elf[id].x-1][elf[id].y].height))
        return 1;
    // if the elf falls off the glacier
    if(!map[elf[id].x-1][elf[id].y].in_radius)
    {
        elf[id].hp=0;
        elf[id].dry=0;
        leaveCell(map,elf,id);
        fprintf(fout,"%s fell off the glacier.\n",elf[id].name);
        return 0;
    }
    // else the elf moves
    leaveCell(map,elf,id);
    elf[id].stamina-=abs(map[elf[id].x][elf[id].y].height-map[elf[id].x-1][elf[id].y].height);
    elf[id].x--;
    return 2;
}
int moveDown(Cell map[][DIM_MAX],int radius,Player elf[],int id)
{
    // if the elf falls off the glacier
    if(elf[id].x==2*radius)
    {
        elf[id].hp=0;
        elf[id].dry=0;
        leaveCell(map,elf,id);
        fprintf(fout,"%s fell off the glacier.\n",elf[id].name);
        return 0;
    }
    // if not enough stamina
    if(elf[id].stamina<abs(map[elf[id].x][elf[id].y].height-map[elf[id].x+1][elf[id].y].height))
        return 1;
    // if the elf falls off the glacier
    if(!map[elf[id].x+1][elf[id].y].in_radius)
    {
        elf[id].hp=0;
        elf[id].dry=0;
        leaveCell(map,elf,id);
        fprintf(fout,"%s fell off the glacier.\n",elf[id].name);
        return 0;
    }
    // else the elf moves
    leaveCell(map,elf,id);
    elf[id].stamina-=abs(map[elf[id].x][elf[id].y].height-map[elf[id].x+1][elf[id].y].height);
    elf[id].x++;
    return 2;
}
int moveLeft(Cell map[][DIM_MAX],Player elf[],int id)
{
    // if the elf falls off the glacier
    if(elf[id].y==0)
    {
        elf[id].hp=0;
        elf[id].dry=0;
        leaveCell(map,elf,id);
        fprintf(fout,"%s fell off the glacier.\n",elf[id].name);
        return 0;
    }
    // if not enough stamina
    if(elf[id].stamina<abs(map[elf[id].x][elf[id].y].height-map[elf[id].x][elf[id].y-1].height))
        return 1;
    // if the elf falls off the glacier
    if(!map[elf[id].x][elf[id].y-1].in_radius)
    {
        elf[id].hp=0;
        elf[id].dry=0;
        leaveCell(map,elf,id);
        fprintf(fout,"%s fell off the glacier.\n",elf[id].name);
        return 0;
    }
    // else the elf moves
    leaveCell(map,elf,id);
    elf[id].stamina-=abs(map[elf[id].x][elf[id].y].height-map[elf[id].x][elf[id].y-1].height);
    elf[id].y--;
    return 2;
}
int moveRight(Cell map[][DIM_MAX],int radius,Player elf[],int id)
{
    // if the elf falls off the glacier
    if(elf[id].y==2*radius)
    {
        elf[id].hp=0;
        elf[id].dry=0;
        leaveCell(map,elf,id);
        fprintf(fout,"%s fell off the glacier.\n",elf[id].name);
        return 0;
    }
    // if not enough stamina
    if(elf[id].stamina<abs(map[elf[id].x][elf[id].y].height-map[elf[id].x][elf[id].y+1].height))
        return 1;
    // if the elf falls off the glacier
    if(!map[elf[id].x][elf[id].y+1].in_radius)
    {
        elf[id].hp=0;
        elf[id].dry=0;
        leaveCell(map,elf,id);
        fprintf(fout,"%s fell off the glacier.\n",elf[id].name);
        return 0;
    }
    // else the elf moves
    leaveCell(map,elf,id);
    elf[id].stamina-=abs(map[elf[id].x][elf[id].y].height-map[elf[id].x][elf[id].y+1].height);
    elf[id].y++;
    return 2;
}
// function returns if elf[id] is still dry at the end
// returns 2 if he gets a win on his way:
int moveToCell(Cell map[][DIM_MAX],int radius,Player elf[],int id,char direction)
{
    int ok;
    switch(direction)
    {
        case 'U':
            ok=moveUp(map,elf,id);
            break;
        case 'D':
            ok=moveDown(map,radius,elf,id);
            break;
        case 'L':
            ok=moveLeft(map,elf,id);
            break;
        case 'R':
            ok=moveRight(map,radius,elf,id);
            break;
        default:
            break;
    }
    if(ok==0) return 0;
    if(ok==2)
    {
        map[elf[id].x][elf[id].y].id[map[elf[id].x][elf[id].y].occupied]=id;
        map[elf[id].x][elf[id].y].occupied++;
        // looks for better gloves
        if(map[elf[id].x][elf[id].y].gloves>elf[id].dmg)
            swap(&map[elf[id].x][elf[id].y].gloves,&elf[id].dmg);
    }
    // if he meets another elf, they battle
    if(map[elf[id].x][elf[id].y].occupied>=2)
    {
        int j=0;
        if(map[elf[id].x][elf[id].y].id[j]==id) j++;
        int battle_result=playersBattle(elf,map[elf[id].x][elf[id].y].id[j],id);
        switch(battle_result)
        {
            case 1:
                for(j=0;j<map[elf[id].x][elf[id].y].occupied;++j)
                    if(map[elf[id].x][elf[id].y].id[j]==id) break;
                for(int k=j+1;k<map[elf[id].x][elf[id].y].occupied;++k)
                    map[elf[id].x][elf[id].y].id[k-1]=map[elf[id].x][elf[id].y].id[k];
                map[elf[id].x][elf[id].y].occupied--;
                return 0;
            case 2:
                for(int i=j+1;i<map[elf[id].x][elf[id].y].occupied;++i)
                    map[elf[id].x][elf[id].y].id[i-1]=map[elf[id].x][elf[id].y].id[i];
                map[elf[id].x][elf[id].y].occupied--;
                return 2;
            default:
                return 1;
        }
    }
    return 1;
}
void cmdMove(Cell map[][DIM_MAX],int radius,Player elf[],int no_of_players,int *active_players)
{
    int id;
    char moves[MOVESCOUNT_MAX];
    fscanf(fin,"%d%s",&id,moves);
    if(!elf[id].dry) return;
    int i=0;
    int ok;
    while(moves[i]!='\0')
    {
        ok=moveToCell(map,radius,elf,id,moves[i]);
        if(ok!=1)
        {
            (*active_players)--;
            if(*active_players==1) endGame(elf,no_of_players);
            if(!ok) break;
        }
        i++;
    }
}
void cmdSnowstorm(Cell map[][DIM_MAX],Player elf[],int no_of_players,int *active_players)
{
    int K;
    int X,Y,R,DMG;
    fscanf(fin,"%d",&K);
    X=K&255; K>>=8;
    Y=K&255; K>>=8;
    R=K&255; K>>=8;
    DMG=K&255;
    for(int i=0;i<no_of_players;++i) if(elf[i].dry)
    {
        if(inRadius(elf[i].x,elf[i].y,X,Y,R))
        {
            elf[i].hp-=DMG;
            if(elf[i].hp<=0)
            {
                elf[i].hp=0;
                elf[i].dry=0;
                fprintf(fout,"%s was hit by snowstorm.\n",elf[i].name);
                leaveCell(map,elf,i);
                (*active_players)--;
            }
        }
    }
    if(*active_players<2) endGame(elf,no_of_players);
}
void cmdMeltdown(Cell map[][DIM_MAX],int *radius,Player elf[],int no_of_players,int *active_players)
{
    int stamina_bonus;
    fscanf(fin,"%d",&stamina_bonus);
    int i,j;
    for(i=0;i<no_of_players;++i) if(elf[i].dry)
    {
        if(!inRadius(elf[i].x,elf[i].y,*radius,*radius,*radius-1))
        {
            elf[i].hp=0;
            elf[i].dry=0;
            fprintf(fout,"%s got wet because of global warming.\n",elf[i].name);
            leaveCell(map,elf,i);
            (*active_players)--;
        }
        else elf[i].stamina+=stamina_bonus;
    }
    if(*active_players<2) endGame(elf,no_of_players);
    // translation of the map
    (*radius)--;
    int dim=2*(*radius)+1;
    for(i=0;i<dim;++i)
        for(j=0;j<dim;++j)
        {
            map[i][j].height=map[i+1][j+1].height;
            map[i][j].gloves=map[i+1][j+1].gloves;
            // Center of glacier is (radius,radius).
            map[i][j].in_radius=inRadius(i,j,*radius,*radius,*radius);
            map[i][j].occupied=map[i+1][j+1].occupied;
            if(map[i][j].occupied)
            {
                for(int k=0;k<map[i][j].occupied;++k)
                {
                    map[i][j].id[k]=map[i+1][j+1].id[k];
                    elf[map[i][j].id[k]].x=i;
                    elf[map[i][j].id[k]].y=j;
                }
            }
        }
}
int sortedPlayers(Player top_elf[],int id1,int id2)
{
    if(top_elf[id1].dry!=top_elf[id2].dry) return top_elf[id1].dry;
    if(top_elf[id1].wins!=top_elf[id2].wins) return top_elf[id1].wins>top_elf[id2].wins;
    return strcmp(top_elf[id1].name,top_elf[id2].name)<0;
}
void cmdPrintScoreboard(Player elf[],int no_of_players)
{
    Player top_elf[no_of_players];
    int i,j;
    Player aux;
    for(i=0;i<no_of_players;++i) top_elf[i]=elf[i];
    for(i=0;i<no_of_players;++i)
        for(j=i+1;j<no_of_players;++j)
            if(!sortedPlayers(top_elf,i,j))
            {
                aux=top_elf[i];
                top_elf[i]=top_elf[j];
                top_elf[j]=aux;
            }
    fprintf(fout,"SCOREBOARD:\n");
    for(i=0;i<no_of_players;++i)
    {
        fprintf(fout,"%s",top_elf[i].name);
        if(top_elf[i].dry) fprintf(fout,"\tDRY\t");
        else fprintf(fout,"\tWET\t");
        fprintf(fout,"%d\n",top_elf[i].wins);
    }
}
void Play(Cell map[][DIM_MAX],int *radius,Player elf[],int no_of_players,int *active_players)
{
    char str[20];
    while(fscanf(fin,"%s",str)>0)
    {
        if(strcmp(str,"MOVE")==0)
        {
            cmdMove(map,*radius,elf,no_of_players,active_players);
        }
        else if(strcmp(str,"SNOWSTORM")==0)
        {
            cmdSnowstorm(map,elf,no_of_players,active_players);
        }
        else if(strcmp(str,"MELTDOWN")==0)
        {
            cmdMeltdown(map,radius,elf,no_of_players,active_players);
        }
        else if(strcmp(str,"PRINT_SCOREBOARD")==0)
        {
            cmdPrintScoreboard(elf,no_of_players);
        }
    }
}
int main()
{
    fin=fopen("snowfight.in","r");
    fout=fopen("snowfight.out","w");
    int radius,no_of_players,active_players;
    Cell map[DIM_MAX][DIM_MAX];
    initializeMap(map,&radius,&no_of_players,&active_players);
    Player elf[ELVES_MAX];
    initializePlayers(map,elf,no_of_players,&active_players);
    if(active_players<2) endGame(elf,no_of_players);
    Play(map,&radius,elf,no_of_players,&active_players);
    fclose(fin);
    fclose(fout);
    return 0;
}