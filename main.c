#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<time.h>

#define TailleMaxDeLaChaine 20
#define ParkingRailSize 11
#define MapSize 6
#define MaxTrain 11
#define MaxParkingRails 11
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

// les prototypes des fonctions 

// les fonctions de implementation

ParkingRail CreerUnParkingRail(chaine Identifiant, int Capacity);
int PileRemplie(ParkingRail PK);
ParkingRail TrouverDesTrainDisponible(ParkingRail, Train*, int);
int Empiler(Train TrainCourant, ParkingRail * ParkingForTrain);
int AjouterQuais(ParkingRail * Quais,int,chaine, int, Train*, int);
void UpdateQuais( ParkingRail*, int, Train*, int);
Train Depiler (ParkingRail* ParkingForTrain);
void AfficherParkingRail(ParkingRail*, int);
int PileVide(ParkingRail * ParkingForTrain);

// les fonctions de passager

Passagers CreerPassager(int IDPassagerCourant, chaine Destination);
int SiListeVide(Passagers ListePassager);
int TailleListe(Passagers ListePassager);
Passagers AvoirDernierElementList(Passagers ListePassager);
Passagers ReservationPlace(Passagers, Train*, int, chaine);
int ExistIdDansListe(int IDRecherche,Passagers ListePassager);
Passagers AnnulerReservation(Passagers ListePassagers,int IDAnulation);
Passagers AjouterPassagerDansLaListe (Passager PassagerAAjouter, Passagers ListeDesPassagers);
void AfficherPassagers(Passagers);
Passager SupprimerPassagerListeAttenteEtLeRetourner(Passagers*,int);
Passagers DetruireLalisteDePassagers(Passagers p);

// les fonctions de train

Train CreerTrain(int IDTrain, int PlacesSupportees, chaine Destination);
Train ViderTrain(Train T);
int AjouterTrain(Train * Trains, int Id, int Capacitee, chaine Destination);
void AfficherTrains(Train * Trains, int nbrTrains);
int SiTrainRemplis(Train TrainCourant);
void MiseAJourTableauDesTrain(Train TrainPourDepart,Train * Trains,int NumTrain);
Passagers Embarquement (ParkingRail * Quaies, int NumQuaie, Passagers PassagerPretPourEmbarquement, chaine Destinations[MapSize][2], int NumDestination, Train * Trains, int NumTrain);
void TrainEnMarche(ParkingRail * Quaies,int NumQuai,  Train * Trains, int NumTrain);
int InitialiserLesDestinationsDisponibles(chaine Destinations[MapSize][2]);


// la fonction main

int main()
{
    //Declaration des structures de donnees
    Train Trains[MaxTrain];// tableau de train
    ParkingRail Quais[MaxParkingRails]; // tableau de quais
    int NumTrain = 0;//Nombre de train qu'il y'a dans la gare
    int NumQuai = 0;// nombre de quais / Parking rail qu'il y'a dans la gare
    chaine Destinations[MapSize][2]; // matrice des destinations
    int NumDestinations = InitialiserLesDestinationsDisponibles(Destinations);

    // Ajout des trains

    NumTrain = AjouterTrain(Trains,NumTrain,15,"Tokyo");
    NumTrain = AjouterTrain(Trains,NumTrain,10,"Helsinki");
    NumTrain = AjouterTrain(Trains,NumTrain,10,"Helsinki");
    NumTrain = AjouterTrain(Trains,NumTrain,10,"Tokyo");
    NumTrain = AjouterTrain(Trains,NumTrain,10,"Berlin");
    NumTrain = AjouterTrain(Trains,NumTrain,8,"Nairobi");
    NumTrain = AjouterTrain(Trains,NumTrain,10,"Denver");
    NumTrain = AjouterTrain(Trains,NumTrain,10,"Nairobi");
    NumTrain = AjouterTrain(Trains,NumTrain,10,"Berlin");
    NumTrain = AjouterTrain(Trains,NumTrain,10,"Ouergla");

    // Ajouter les Quais

    NumQuai = AjouterQuais(Quais,NumQuai,"Q1",2,Trains, NumTrain);
    NumQuai = AjouterQuais(Quais,NumQuai,"El Moudjahid",1,Trains, NumTrain);
    NumQuai = AjouterQuais(Quais,NumQuai,"Roberto Carlos",1,Trains,NumTrain);
    Passagers ListeDesPassagerEnAttente=NULL; // declarer la liste des passagers en attente

    //Commencer la simulation

    int NbrHeur = 0;
    while(1){// nous pouvons remplacer le 1 par un nombre d'iteration bien precis !
        AfficherParkingRail(Quais,NumQuai); // afficher l'etat des quais pour voir ou se trouve les train.
        NbrHeur++;
        printf("************Heure %d***********\n\n",NbrHeur);
        srand(time(NULL));// appeler la fonction random pour generer des nombres aleatoire de reservations et d'annulations
        int NombreAleatoireDeReservations = rand() % 10 + 5;
        int NombreAleatoireDAnulations = rand() % 5;
        // nous devons nous assurer que le nombres de reservations soit strictement superieurs aux nombre d'annulations
        printf("Nombre de reservations : %d\n",NombreAleatoireDeReservations);
        printf("Nombre d'annulations : %d\n",NombreAleatoireDAnulations);
        for (int i=0;i<NombreAleatoireDeReservations;i++){
            int ChoisirUneDestinationAleatoire=rand()%(NumDestinations+1);
            ListeDesPassagerEnAttente=ReservationPlace(ListeDesPassagerEnAttente,
            Trains,NumTrain,Destinations[ChoisirUneDestinationAleatoire][0]);
            // reserver des places et ajouter des passagers dans la liste d'attente, a ce stade, aucun passager n'est dans le train, rien que la variable � place prises � est incrementer pour dire que les places sont reservees
        }
        for (int i=0;i<NombreAleatoireDAnulations;i++){
            int ChoisirUnIDPourAnulationAleatoire = rand() % TailleListe(ListeDesPassagerEnAttente);
            ListeDesPassagerEnAttente = AnnulerReservation(ListeDesPassagerEnAttente,ChoisirUnIDPourAnulationAleatoire);
            // briser la liste chainee � ListeDesPassagersEnAttente � et enlever les passagers qui souhaites annuler leurs reservations.
        }
        printf("\n Nombre de passagers en attente %d\n",TailleListe(ListeDesPassagerEnAttente));
        AfficherPassagers(ListeDesPassagerEnAttente);
        // affichier l��tat des passagers en attente avant embarquement et apr�s que les rEservations et annulations soient faites
        ListeDesPassagerEnAttente = Embarquement(Quais, NumQuai,ListeDesPassagerEnAttente, Destinations,NumDestinations, Trains,NumTrain);
        // Embarquer les passager !
        AfficherTrains(Trains,NumTrain);
        //afficher l��tat des trains pour voir si ils ont pris des passagers avec eux.
        TrainEnMarche(Quais,NumQuai, Trains, NumTrain);
        // lancer les trains qui sont les premiers dans les quais
        printf("\n Nombre de passagers en attente apres embarquement%d\n",TailleListe(ListeDesPassagerEnAttente));
        AfficherPassagers(ListeDesPassagerEnAttente);
        // afficher l'etat des passagers en attente apres embarquement, dans le cas ou votre programme marche correctement, il ne devrai y avoir aucun passager qui demande une destination qui vient d'etre propose par un train sur le depart sauf si le train est remplis a 100%.
        UpdateQuais( Quais, NumQuai, Trains, NumTrain );
        //Mettre a jour les quais en recuperant les train en attente et en avancant les tarins dans la pile pour proposer de nouvelle destinations.
        system("pause");// arreter le programme pour avoir le temps de lire les affichages
        // vous pouvez ajouter un � clearscreen � ici mais ceci vous empechera d'avoir une vue sur l'historique
    }
    //*/
    return 0;
}


// les corps des fonctions

// les corps des fonctions de l'implementation

// la fonction 1

ParkingRail CreerUnParkingRail(chaine Identifiant, int Capacity)
{
	ParkingRail Rail;

	strcpy(Rail.Identifiant, Identifiant);
	Rail.IndexPremierTrain = -1;
	Rail.Capacity = Capacity;

	return Rail;
}

// la fonction 2

int PileRemplie(ParkingRail PK)
{
	if(PK.IndexPremierTrain == PK.Capacity) return 0;
		else return 1;	
}

// la fonction 3

ParkingRail TrouverDesTrainDisponible(ParkingRail ParkingForTrain, Train * Trains, int numTrain)
{
	for(int i=0;i<numTrain;i++){

	    if((Trains[i].kilometreRestant == -1) && (!PileRemplie(ParkingForTrain))){

	        Trains[i].kilometreRestant = 0;//train dans le quai
	        Empiler(Trains[i],&ParkingForTrain);
	        //ParkingForTrain.Capacity=ParkingForTrain.Capacity-1;// diminuer la capacite du quai
	    }
	}
	return ParkingForTrain;
}

// la fonction 4

int Empiler(Train TrainCourant, ParkingRail* ParkingForTrain)
{
	if(0 == PileRemplie) return 0;
		else{
			++ParkingForTrain->IndexPremierTrain;
			ParkingForTrain->ListeDesTrains[ParkingForTrain->IndexPremierTrain] = TrainCourant;
			return 1;
		}
}

// la fonction 5

int AjouterQuais(ParkingRail * Quais,int NumQuai,chaine IdentifiantQuai, int capacity, Train * Trains, int NumTrain)
{
	Quais[NumQuai] = CreerUnParkingRail(IdentifiantQuai,capacity);
	//creer un quai avec un identifiant et une capacite
	Quais[NumQuai] = TrouverDesTrainDisponible(Quais[NumQuai],Trains,NumTrain);
	// rechercher des trains disponible pour remplir le quai
	NumQuai = NumQuai+1;

	return NumQuai;
}

// la fonctions 6

void UpdateQuais( ParkingRail * Quais, int NumQuai, Train * Trains, int NumTrain )
{
	for (int j=0;j<NumTrain;j++){
	    for(int i=0;i<NumQuai;i++){
	        if((Trains[j].kilometreRestant==-1)){
	            if(!PileRemplie(Quais[i])){
	                Trains[j].kilometreRestant=0;
	                Empiler(Trains[j],&Quais[i]);
	            }
	        }
	    }
	}
}

// la fonction 7

Train Depiler (ParkingRail* ParkingForTrain)
{
	Train t;
	if(PileVide(ParkingForTrain)) printf("la quai est vide");
		else{
			t = ParkingForTrain->ListeDesTrains[ParkingForTrain->IndexPremierTrain];
			--ParkingForTrain->IndexPremierTrain;
			return t;
		}
}
// la fonctions 8

void AfficherParkingRail(ParkingRail* ParkingRails, int NumParkingRail){

/******************************************************************************/
	printf("Le nombre de Quais dans la gare est de %d\n \n",NumParkingRail);

	for (int i=0;i<NumParkingRail;i++){

		printf("Le nombre de train dans le quai %s est de %d \n", ParkingRails[i].Identifiant, ParkingRails[i].IndexPremierTrain + 1);

		for (int j=ParkingRails[i].IndexPremierTrain;j>=0;j--){
		    printf("le train ID=%d prendra le depart en %d position\n",ParkingRails[i].ListeDesTrains[j].identifiant, ParkingRails[i].IndexPremierTrain - j + 1);
		}
	}
}

// la fonction 9

int PileVide(ParkingRail * ParkingForTrain)

{
	if(ParkingForTrain->IndexPremierTrain == -1) return 1;
		else return 0;
}
// les corps des fonctions de passager

// la fonction 10

Passagers CreerPassager(int IDPassagerCourant, chaine Destination)
{
	Passagers p = (Passagers)malloc(sizeof(Passager));

	p->ID = IDPassagerCourant;
	strcpy(p->Destination, Destination);
	p->PassagerSuivant = NULL;

	return p;
}

// la fonction 11

int SiListeVide(Passagers ListePassager)
{
	if(ListePassager == NULL) return 1;
		else return 0;
}

// la fonction 12

int TailleListe(Passagers ListePassager)
{
	int n=0;

	while(ListePassager != NULL){
		n++;
		ListePassager = ListePassager->PassagerSuivant;
	}

	return n;
}

// la fonction 13

Passagers AvoirDernierElementList(Passagers ListePassager)
{
	while(ListePassager->PassagerSuivant != NULL) ListePassager = ListePassager->PassagerSuivant;

	return ListePassager;
}

// la fonction 14

Passagers ReservationPlace(Passagers ListePassagers, Train * Trains, int NumTrain, chaine Destination){

	//nous supposons que la destination n'existe pas
	int SiDestinationTrouvee = 0;
	//Verifier si la destination est disponible
	for (int i=0;i<NumTrain;i++){
	    if ((SiDestinationTrouvee == 0) && (strcmp(Trains[i].Destination,Destination) == 0))
		    // la destination existe dans la gare
	        //(!SiTrainRemplis(Trains[i]))&&
			// j'avais mis cette condition ensuite je me suis dis que c'est pas la peine puisque le passager va etre mis dans la liste d'attente, si le train est maintenant remplis il ne le sera peut etre pas lors de l'embarquement ... apres, si vous voulez modifier ...
	        //(Trains[i].kilometreRestant<=0))
			// on check tout les trains, ceux qui sont en voyage et ceux qui sont dans le quai pour dire que la destination existe, maintenant pour la disponibilite immediate ... c'est autre chose ...
	        {
	        SiDestinationTrouvee=1;
	        //Trains[i].PlacesPrises=Trains[i].PlacesPrises+1;
			// l'incrementation de cette variable se fera lors de l'embarquement ...
	        }
	}
	if(SiDestinationTrouvee){// ajouter dans la liste des passagers en attente d'embarquement
	    //Liste Vide, Cr�ation du premier �lement
	    if(ListePassagers == NULL){
	        ListePassagers = CreerPassager(0, Destination);

	        return ListePassagers;
	    }
	    //Liste Non vide, aller au dernier element de la liste et ajouter un passager ou ajouter a la tete de la liste, votre choix !
	    Passagers DernierElmnt = AvoirDernierElementList(ListePassagers);
	    DernierElmnt->PassagerSuivant = CreerPassager(DernierElmnt->ID + 1, Destination);
	}
	else //destination non trouve
	    printf("La destination %s n'est pas disponible dans notre gare\n",Destination);

	return ListePassagers;
}

// la fonction 15

int ExistIdDansListe(int IDRecherche,Passagers ListePassager)
{
	if(SiListeVide(ListePassager))  printf("La liste est vide");
		else{
				while(ListePassager != NULL){
					if(IDRecherche == ListePassager->ID) return 1;
					ListePassager = ListePassager->PassagerSuivant;
				}	
			}
	return 0;
}

// la fonction 16

Passagers AnnulerReservation(Passagers ListePassagers,int IDAnulation)
{
	Passagers q,p = ListePassagers;

	if(ExistIdDansListe(IDAnulation, p)){
		if(p->ID = IDAnulation)
		{
			q = p;
			p = p->PassagerSuivant;
			free(q);
		}else{
			while((p->PassagerSuivant != NULL) && (p->ID != IDAnulation)){
				q=p;
				p = p->PassagerSuivant;
			}
			if(p != NULL){
				q->PassagerSuivant = p->PassagerSuivant;
				free(p);
			}
		}
	}
	return ListePassagers;
}

// la fonction 17

Passagers AjouterPassagerDansLaListe (Passager PassagerAAjouter, Passagers ListeDesPassagers)
{
	Passagers p = (Passagers)malloc(sizeof(Passager));

	*p = PassagerAAjouter;
	p->PassagerSuivant = ListeDesPassagers;
	ListeDesPassagers = p;

	return ListeDesPassagers;
}
// la fonction 18

void AfficherPassagers(Passagers ListePassagers)
{
	Passagers ParcoursPassager=ListePassagers;

	if (TailleListe(ParcoursPassager)==0){
	    printf("\n**Aucune reservation faite !\n");
	    return;

	}
	printf("\nListe des reservations \n");
	while (ParcoursPassager!=NULL){
	    printf("Le passager avec l'ID %d a reserve la destination %s ! \n",ParcoursPassager->ID,ParcoursPassager->Destination);
	    ParcoursPassager=ParcoursPassager->PassagerSuivant;
	}
	printf("\n");
}

// la fonction 19

Passager SupprimerPassagerListeAttenteEtLeRetourner(Passagers* ListePassagers, int iDPassager)
{
    Passager PassagerARetourner;

    if ((*ListePassagers)->ID == iDPassager){
        PassagerARetourner = **(ListePassagers);
        *ListePassagers = (*ListePassagers)->PassagerSuivant;
        //printf("cas 0");
        return PassagerARetourner;
    }

    Passagers PassagerParent = *ListePassagers;
    Passagers PassagerCourant = (*ListePassagers)->PassagerSuivant;

    while ((!SiListeVide(PassagerCourant)) && (PassagerCourant->ID != iDPassager)){
        PassagerParent = PassagerCourant;
        PassagerCourant = PassagerParent->PassagerSuivant;
    }
    if(SiListeVide(PassagerCourant)){
        printf("aucune suppression Faite !!! faite ! ");
    }
    //printf("suppression de passager %d",PassagerCourant->ID);
    PassagerParent->PassagerSuivant = PassagerCourant->PassagerSuivant;
    PassagerARetourner = *PassagerCourant;
    free(PassagerCourant);
	//printf("je suis dans la suppression taille liste est : %d", TailleListe(*ListePassagers));
	return PassagerARetourner;
}

// une fonction supplementaire 

Passagers DetruireLalisteDePassagers(Passagers p)
{
	Passagers q;

	if(!SiListeVide(p)){
		while (p != NULL)
		{
			q = p;
			p = p->PassagerSuivant;
			free(q);
		}
	} 
	return p;
}

// les corps des fonctions de train

// la fonction 20

Train CreerTrain(int IDTrain, int PlacesSupportees, chaine Destination)
{
	Train t;

	t.identifiant = IDTrain;
	t.PlacesSupportees = PlacesSupportees;
	strcpy(t.Destination, Destination);
	t.kilometreRestant = -1;
	t.PlacesPrises = 0;
	t.PassagersDuTrain = NULL;

	return t;
}

// la fonction 21

Train ViderTrain(Train T)
{
	T.PlacesPrises = 0;
	T.kilometreRestant = -1;
	DetruireLalisteDePassagers(T.PassagersDuTrain);
}
// la fonction 22

int AjouterTrain(Train * Trains, int Id, int Capacitee, chaine Destination)
{
	Trains[Id] = CreerTrain(Id, Capacitee, Destination);

	return Id + 1;
}

// la fonction 23

void AfficherTrains(Train * Trains, int nbrTrains)
{
	printf("Il y'a %d trains dans la gare \n\n", nbrTrains);
	for (int i=0;i<nbrTrains;i++){
	    // calcul du pourcentage de remplissage
	    float Pourcentage = ((float)Trains[i].PlacesPrises / (float)Trains[i].PlacesSupportees) * 100;

	    if (Trains[i].kilometreRestant == -1)
	        printf("Train ID= %d est disponible pour %s \n",Trains[i].identifiant, Trains[i].Destination);
	    else if (Trains[i].kilometreRestant == 0)
	        	printf("Train ID= %d dans le quai, remplis a %.2f %%, en attente de depart vers %s \n",Trains[i].identifiant,Pourcentage, Trains[i].Destination);
	    	else
				//printf("Le train avec ID= %d est en voyage vers %s, il lui reste %d Km pour rentrer. \n",Trains[i].identifiant,Trains[i].Destination, Trains[i].kilometreRestant);// premier affichage ... j'ai pas trop aim� :/
				printf("Train ID %d en Trajet passagers: %d, %.2f %%, ===== reste:%d Km ====> %s\n\n",Trains[i].identifiant,Trains[i].PlacesPrises,Pourcentage,Trains[i].kilometreRestant,Trains[i].Destination);
	}
}

// fonction 24

int SiTrainRemplis(Train TrainCourant)
{
	if(TrainCourant.PlacesSupportees == TrainCourant.PlacesPrises) return 1;
		else return 0;
}

// fonction 25

void MiseAJourTableauDesTrain(Train TrainPourDepart,Train * Trains,int NumTrain)
{
	for (int i=0;i<NumTrain;i++){
	    if (TrainPourDepart.identifiant==Trains[i].identifiant){
	        Trains[i]=TrainPourDepart;
	        return;
	    }
    }
}
// fonction 26

Passagers Embarquement(ParkingRail * Quaies, int NumQuaie, Passagers PassagerPretPourEmbarquement, chaine Destinations[MapSize][2], int NumDestination, Train * Trains, int NumTrain)
{
    printf("Liste Des Prochains Departs\n");
    for (int i=0;i<NumQuaie;i++){
        Train TrainPourDepart = Depiler(&Quaies[i]);
        if(TrainPourDepart.identifiant == -1)
            printf("Le quai %s est vide \n",Quaies[i].Identifiant);
        else{
        printf("Prochain train,ID=%d, a destination de %s dans le quai %s\n",TrainPourDepart.identifiant,TrainPourDepart.Destination,Quaies[i].Identifiant);
            //RecupererLaDistance
            for(int i=0;i<(NumDestination+1);i++){
                if(strcmp(TrainPourDepart.Destination,Destinations[i][0]) == 0){
                    TrainPourDepart.kilometreRestant = atoi(Destinations[i][1]);
                }
            }
            Passagers ParcourirPassager = PassagerPretPourEmbarquement;
            while(ParcourirPassager != NULL){
                if((strcmp(ParcourirPassager->Destination,TrainPourDepart.Destination) == 0) && (!SiTrainRemplis(TrainPourDepart))){
                    //Enlever de la liste des passager en attente et le mettre dans la liste des passagers dans le train
                    Passager PassagerTmp = SupprimerPassagerListeAttenteEtLeRetourner(&PassagerPretPourEmbarquement,ParcourirPassager->ID);
                    //Le mettre dans le train
                    TrainPourDepart.PassagersDuTrain = AjouterPassagerDansLaListe(PassagerTmp,TrainPourDepart.PassagersDuTrain);
                    TrainPourDepart.PlacesPrises = TrainPourDepart.PlacesPrises + 1;
                }
                ParcourirPassager = ParcourirPassager->PassagerSuivant;
            }
            MiseAJourTableauDesTrain(TrainPourDepart,Trains,NumTrain);
        }
    }
    printf("\n");
	return PassagerPretPourEmbarquement;
}

// fonction 29

void TrainEnMarche(ParkingRail * Quaies,int NumQuai,  Train * Trains, int NumTrain){
	// Vitesse constante pour tout les trains de 5 ! votre job est de la rendre dynamique en utilisant a la plce la variable Train[i].vitesse
	for (int i=0;i<NumTrain;i++){

	    if(Trains[i].kilometreRestant == 5){
	        Trains[i].kilometreRestant = Trains[i].kilometreRestant - 5;
	        int ChoixAleatoireQuai = rand() % (NumQuai);
	        Trains[i] = ViderTrain(Trains[i]);

	        if(Empiler(Trains[i],&Quaies[ChoixAleatoireQuai]))
	            printf("Train Id=%d revient de %s rentre dans le Quai %s\n",Trains[i].identifiant,Trains[i].Destination,Quaies[ChoixAleatoireQuai].Identifiant);
				else{
					printf("\ntout les quaies sont remplis\n Train ID=%d en attente ",Trains[i].identifiant);
					Trains[i].kilometreRestant = - 1;
				}
	    }
	    else if(Trains[i].kilometreRestant > 5)
	        	Trains[i].kilometreRestant=Trains[i].kilometreRestant - 5;
	}
}

// la fonction 30

int InitialiserLesDestinationsDisponibles(chaine Destinations[MapSize][2])
{
	strcpy(Destinations[0][0], "Tokyo");
	strcpy(Destinations[0][1], "100");
	strcpy(Destinations[1][0], "Ouergla");
	strcpy(Destinations[1][1], "15");
	strcpy(Destinations[2][0], "Helsinki");
	strcpy(Destinations[2][1], "40");
	strcpy(Destinations[3][0], "Denver");
	strcpy(Destinations[3][1], "45");
	strcpy(Destinations[4][0], "Rio");
	strcpy(Destinations[4][1], "10");
	strcpy(Destinations[5][0], "Berlin");
	strcpy(Destinations[5][1], "20");

	return 6;
}