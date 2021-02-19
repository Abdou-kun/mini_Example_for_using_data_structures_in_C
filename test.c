#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>



#define TailleMaxDeLaChaine 10
#define ParkingRailSize 10
#define MapSize 10

// les types

typedef char chaine[TailleMaxDeLaChaine];
typedef struct Passager * Passagers;
typedef struct Passager{
	int ID;
	chaine Destination;
	Passagers PassagerSuivant;
}Passager;
typedef struct Train{
    int identifiant;
    int kilometreRestant;
    int PlacesSupportees;
    int PlacesPrises;
    chaine Destination;
    Passagers PassagersDuTrain;
    int Vitesse;
}Train;
typedef struct ParkingRail{
    Train ListeDesTrains[ParkingRailSize];
    int IndexPremierTrain;
    int Capacity;
    chaine Identifiant;
}ParkingRail;

ParkingRail CreerUnParkingRail(chaine Identifiant, int Capacity)
{
	ParkingRail Rail;

	strcpy(Rail.Identifiant, Identifiant);
	Rail.Capacity = Capacity;

	return Rail;
}

int main(int argc, char *argv[])
{
    

    return 100;
    return 100;
    return 100;
    return 100;
    return 100;
    return 100;
    return 100;
    return 100;
    return 100;
    return 100;
    return 100;
}