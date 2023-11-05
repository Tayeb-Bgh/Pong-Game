#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Declaration des fonction~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// Declaration de la fonction qui gere la generation aleatoire des obstacles
void AfficherObstacle(int numberObstacle, Vector2* positionObstacle, int largeurObstacle, int hauteurObstacle, Rectangle* obstacle, int randomX, int randomY, int hauteurEcran, int mode)
{
    if (mode == 1)
    {
#define randomXmin 500
#define randomXmax 700
    }
    else
    {
#define randomXmin 400
#define randomXmax 700
    }

#define DistanceX 60
#define DistanceY 200

    randomY = rand() % (hauteurEcran - hauteurObstacle - (hauteurObstacle + 115) + 1) + hauteurObstacle + 115;

    if (numberObstacle == 0)
    {
        randomX = rand() % (randomXmax - randomXmin + 1) + randomXmin;
    }
    else
    {
        randomX = obstacle[numberObstacle - 1].x + largeurObstacle + DistanceX;
    }
    if (numberObstacle == 2)
    {
        randomY = obstacle[numberObstacle - 2].y - DistanceY;
    }

    obstacle[numberObstacle].x = randomX;
    obstacle[numberObstacle].y = randomY;
    obstacle[numberObstacle].width = largeurObstacle;
    obstacle[numberObstacle].height = hauteurObstacle;
}

// Declaration de la fonction joueur1() qui gere les d?placement du 1er joueur du mode multi ou solo
void joueur1(Rectangle* raquette1, int vitesseRaquette, int hauteurEcran, int hauteurRaquette)
{
    // Mise a jour des entrees (clavier )
    if (IsKeyDown(KEY_W))
    {
        if (raquette1->y >= 90)
            raquette1->y -= vitesseRaquette;
    }
    if (IsKeyDown(KEY_S))
    {
        if (raquette1->y <= hauteurEcran - hauteurRaquette)
            raquette1->y += vitesseRaquette;
    }
}

// D?claration de la fonction joueur2() qui g?re les d?placement du 2eme joueur du mode multi
void joueur2(Rectangle* raquette1, int vitesseRaquette, int hauteurEcran, int hauteurRaquette)
{
    // Mise a jour des entrees (clavier et souris)
    if (IsKeyDown(KEY_UP))
    {
        if (raquette1->y >= 90)
            raquette1->y -= vitesseRaquette;
    }
    if (IsKeyDown(KEY_DOWN))
    {
        if (raquette1->y <= hauteurEcran - hauteurRaquette)
            raquette1->y += vitesseRaquette;
    }
}

// D?claration de la fonction Bot() qui gere les deplacement de la raquette du bot
void Bot(Rectangle* raquette, Vector2 positionBalle, int vitesseRaquette)
{
    // Si la balle est au-dessus de la raquette, d?placez la raquette vers le haut
    if (positionBalle.y < raquette->y + raquette->height / 2 - 50)
    {
        if (raquette->y >= 90)
            raquette->y -= vitesseRaquette;
    }

    // Sinon, si la balle est en dessous de la raquette, d?placez la raquette vers le bas
    else if (positionBalle.y > raquette->y + raquette->height / 2 + 50)
    {
        if (raquette->y <= 800 - 115)
            raquette->y += vitesseRaquette;
    }
}


int main(void)
{
    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~D?claration de tout les element d'en on a besoin pour le jeu~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    // D?claration d'une variable qui permet de parcourir les boucles plus facilement
    int i;
    int p = 2;
    // D?claration du conteur qui parcourt les Obstacles
    int numberObstacle = 0;
    // D?claration du conteur qui r?gle le bug de la balle coinc? dans la racket
    bool BugRaquette1 = true;
    bool BugRaquette2 = true;
    bool BugObstacle[3] = { true };
    // D?claration des conteurs de rate des 2 racket
    int joueurRate = 0;
    int botRate = 0;
    // Declaration du Compteur pour le temps ecoule depuis l'affichage du message (j'ai definie sa limite a 3 seconde)
    int timerMsg = 0;
    const int MAX_TIME = 60 * 3;
    // Declaration du Compteur de la game
    int timerGame = 0;
    // Declaration du Compteur qui empeche que le meme obstacle se g?n?re a l'infinie
    int U = 0;
    // Declaration Coordonnees des obstacles qui seront generer
    int randomX = 0;
    int randomY = 0;
    // Declaration de la variable Dash
    int dash = 3;
    int dash2 = 3;
    // Declaration de la variable qui va nous permettre de switch entre les Ecrans
    int Ecran = 0;
    int mode = 0;
    bool standard;
    bool replay = false;
    int stockMode;
    int stockEcran;
    bool stockStandard;
    // Declaration de la variable qui va nous permettre de generer que 1 seul son qunad la souris passe sur l'ecran
    bool sonjouer = false;
    // Declaration du score
    char botRateAffiche[2];
    char joueurRateAffiche[2];


    // Initialisation de la fenetre et du mode graphique
    #define largeurEcran 1200
    #define hauteurEcran 800
    InitWindow(largeurEcran, hauteurEcran, "Pong");
    InitAudioDevice();

    SetTargetFPS(60);

    // Creation des raquettes
    #define largeurRaquette 15
    #define hauteurRaquette 115
    #define vitesseRaquette 10
    Rectangle raquette1 = { 10, (hauteurEcran - hauteurRaquette) / 2, largeurRaquette, hauteurRaquette };
    Rectangle raquette2 = { largeurEcran - 10 - largeurRaquette, (hauteurEcran - hauteurRaquette) / 2, largeurRaquette, hauteurRaquette };

    // Creation de la balle
    #define rayonBalle 10
    #define vitesseBalleX 14
    #define vitesseBalleY 14
    Vector2 positionBalle = { largeurEcran / 2, hauteurEcran / 2 };
    Vector2 vitesseBalle = { vitesseBalleX, vitesseBalleY };

    // Creation des obstacles
    #define largeurObstacle 10
    #define hauteurObstacle 160
    Vector2 positionObstacle[3];
    Rectangle obstacle[3];


    // Importation et d?claration des fond d'image du mode SinglePlayer
    Texture2D overlay3Single = LoadTexture("ressources/overlay3dash.png");
    Texture2D overlay2Single = LoadTexture("ressources/overlay2dash.png");
    Texture2D overlay1Single = LoadTexture("ressources/overlay1dash.png");
    Texture2D overlay0Single = LoadTexture("ressources/overlay0dash.png");
    Texture2D backgroundWinSingle = LoadTexture("ressources/winner.png");
    Texture2D backgroundLooseSingle = LoadTexture("ressources/looser.png");

    // Importaton et d?claration des fons d'image du mode MultiPlayer
    Texture2D overlay3Multi_G = LoadTexture("ressources/overlaydash3player1.png");
    Texture2D overlay2Multi_G = LoadTexture("ressources/overlaydash2player1.png");
    Texture2D overlay1Multi_G = LoadTexture("ressources/overlaydash1player1.png");
    Texture2D overlay0Multi_G = LoadTexture("ressources/overlaydash0player1.png");
    Texture2D overlay3Multi_D = LoadTexture("ressources/overlaydash3player2.png");
    Texture2D overlay2Multi_D = LoadTexture("ressources/overlaydash2player2.png");
    Texture2D overlay1Multi_D = LoadTexture("ressources/overlaydash1player2.png");
    Texture2D overlay0Multi_D = LoadTexture("ressources/overlaydash0player2.png");
    Texture2D backgroundWinPlayer1 = LoadTexture("ressources/player1wins.png");
    Texture2D backgroundWinPlayer2 = LoadTexture("ressources/player2wins.png");
    Texture2D fond = LoadTexture("ressources/fond.png");
    Texture2D background = LoadTexture("ressources/background.png");

    // Importation d'une Police
    Font police = LoadFont("ressources/ethnocentric rg.otf");

    // Declaration des Rectangle du menu
    Rectangle rectangleSingle = { largeurEcran / 2 - 160,hauteurEcran / 2 - 100,350,97 };
    Rectangle rectangleMulti = { largeurEcran / 2 - 160,hauteurEcran / 2 + 30 ,350,100 };
    Rectangle rectangleExit = { largeurEcran / 2 - 160,hauteurEcran / 2 + 160 ,350,100 };
    Rectangle rectangleReplay = { largeurEcran / 2 - 190,hauteurEcran / 2 - 70,366,100 };
    Rectangle rectangleMenu = { largeurEcran / 2 - 190,hauteurEcran / 2 + 70,366,100 };
    Rectangle rectangleStandard = { largeurEcran / 2 - 230,hauteurEcran / 2 - 10,460,100 };
    Rectangle rectangleCustom = { largeurEcran / 2 - 230,hauteurEcran / 2 + 130,460,100 };

    // Declaration des couleurs utilise dans le programme
    Color couleurArrierePlan = { 84, 119, 120,255 };
    Color couleurRectangleFonce = { 11,73,108,255 };
    Color couleurRectangle = { 10,51,74,255 };
    Color couleurEcriture = { 168,218,220,255 };

    // Declaration des sons utilise
    Sound son = LoadSound("ressources/Rebond.mp3");
    Sound sonbouton = LoadSound("ressources/clickbon.mp3");
    Sound sonboutonOff = LoadSound("ressources/boutonsound.mp3");
    Sound sonPerdue = LoadSound("ressources/perdu.mp3");
    Sound sonGagne = LoadSound("ressources/gagner.mp3");
    Sound sonMarquer = LoadSound("ressources/marquer.mp3");

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Boucle Pricipal Du Jeu~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    while (!WindowShouldClose())
    {
        if (Ecran == 2)
        {
            timerGame++;

            if (mode == 1)
            {
                // On utilise la fonction joueur1() pour mettre a jour la position de la raquette 1
                joueur1(&raquette1, vitesseRaquette, hauteurEcran, hauteurRaquette);

                // On utilise la fonction Bot() pour mettre a jour la position de la raquette 2
                Bot(&raquette2, positionBalle, vitesseRaquette);
            }
            else
            {
                // On utilise la fonction joueur() pour mettre a jour la position de la raquette 1
                joueur1(&raquette1, vitesseRaquette, hauteurEcran, hauteurRaquette);

                // On utilise la fonction Bot() pour mettre a jour la position de la raquette 2
                joueur2(&raquette2, vitesseRaquette, hauteurEcran, hauteurRaquette);
            }

            // Mise a jour de la position de la balle //
            if (timerGame <= 1 * 80)
            {
                positionBalle.x += vitesseBalle.x / 2;
                positionBalle.y += vitesseBalle.y / 2;
            }
            else
            {
                positionBalle.x += vitesseBalle.x;
                positionBalle.y += vitesseBalle.y;
            }

            if (positionBalle.y < 81 + rayonBalle)
                positionBalle.y = 81 + rayonBalle;

            if (positionBalle.y > hauteurEcran - rayonBalle)
                positionBalle.y = hauteurEcran - rayonBalle;

            // Gestion des collisions de la balle avec les bords de l'ecran //
            if ((positionBalle.y == 81 + rayonBalle) || (positionBalle.y == hauteurEcran - rayonBalle))
            {
                vitesseBalle.y *= -1;
                PlaySound(son);
            }

            // V?rifie si la balle sort de l'ecran
            if (positionBalle.x <= 5 || positionBalle.x > largeurEcran - 10)
            {
                // Si c'est le joueur qui a manqu? la balle, incremente le compteur des rates du joueur
                if (vitesseBalle.x <= 5)
                {
                    PlaySound(sonMarquer);
                    joueurRate++;
                }

                // Si c'est le bot qui a manqu? la balle, incremente le compteur des rates du bot
                else if (positionBalle.x > largeurEcran - 10)
                {
                    PlaySound(sonMarquer);
                    botRate++;
                }


                // Remet la balle en jeu au centre de l'ecran
                positionBalle = (Vector2){ largeurEcran / 2, hauteurEcran / 2 };

                // Initialiser les variables de bug
                BugRaquette1 = true;
                BugRaquette2 = true;
                BugObstacle[0] = true;
                BugObstacle[1] = true;
                BugObstacle[2] = true;

                // Donne une direction aleatoire et diminue la vitesse de la balle
                float randomNumber = (float)rand() / RAND_MAX * 2 - 1;
                vitesseBalle.x = -1 * (vitesseBalleX / 2);
                vitesseBalle.y = randomNumber * (vitesseBalleY / 2);

            }

            //collisions de la balle avec les raquettes
            if (CheckCollisionCircleRec(positionBalle, rayonBalle, raquette1))
            {
                BugObstacle[0] = true;
                BugObstacle[1] = true;
                BugObstacle[2] = true;
                BugRaquette2 = true;

                if (BugRaquette1 == true)
                {
                    // Detecter ou la balle a frappe la raquette
                    float collisionPointY = positionBalle.y - raquette1.y;
                    float collisionPointRatio = collisionPointY / hauteurRaquette;

                    // Modifier la vitesse de la balle en fonction de l'endroit ou elle a frappe la raquette
                    vitesseBalle.x *= -1;
                    vitesseBalle.x = vitesseBalleX;

                    if (collisionPointRatio < 0.2)
                        vitesseBalle.y = -vitesseBalleY;
                    else if (collisionPointRatio > 0.8)
                        vitesseBalle.y = vitesseBalleY;
                    else if (collisionPointRatio <= 0.4 && collisionPointRatio >= 0.2)
                        vitesseBalle.y = -vitesseBalleY + 3;
                    else if (collisionPointRatio >= 0.6 && collisionPointRatio <= 0.8)
                        vitesseBalle.y = vitesseBalleY - 3;
                    else
                        vitesseBalle.y = 0;

                    PlaySound(son);
                }

                BugRaquette1 = false;
            }

            if (CheckCollisionCircleRec(positionBalle, rayonBalle, raquette2))
            {
                BugObstacle[0] = true;
                BugObstacle[1] = true;
                BugObstacle[2] = true;
                BugRaquette1 = true;

                if (BugRaquette2 == true)
                {
                    // D?tecter ou la balle a frappe la raquette
                    float collisionPointY2 = positionBalle.y - raquette2.y;
                    float collisionPointRatio2 = collisionPointY2 / hauteurRaquette;

                    // D?tecter ou la balle a frapp? la raquette
                    float collisionPointY = positionBalle.y - raquette1.y;
                    float collisionPointRatio = collisionPointY / hauteurRaquette;

                    // Modifier la vitesse de la balle en fonction de l'endroit ou elle a frappe la raquette
                    vitesseBalle.x *= -1;
                    vitesseBalle.x = -vitesseBalleX;

                    if (collisionPointRatio2 < 0.2)
                        vitesseBalle.y = -vitesseBalleY;
                    else if (collisionPointRatio2 > 0.8)
                        vitesseBalle.y = vitesseBalleY;
                    else if (collisionPointRatio2 <= 0.4 && collisionPointRatio2 >= 0.2)
                        vitesseBalle.y = -vitesseBalleY + 3;
                    else if (collisionPointRatio2 >= 0.6 && collisionPointRatio2 <= 0.8)
                        vitesseBalle.y = vitesseBalleY - 3;
                    else
                        vitesseBalle.y = 0;

                    PlaySound(son);
                }

            }

            if (standard == false)
            {
                // Apparition d'obstacle a chaque 5 seconde de jeu
                if (timerGame >= 60 * 5 && U == 0)
                {
                    AfficherObstacle(numberObstacle, &positionObstacle, largeurObstacle, hauteurObstacle, &obstacle, randomX, randomY, hauteurEcran, mode);
                    U++;
                    numberObstacle++;
                }

                if (timerGame >= 60 * 10 && U == 1)
                {
                    AfficherObstacle(numberObstacle, &positionObstacle, largeurObstacle, hauteurObstacle, &obstacle, randomX, randomY, hauteurEcran, mode);
                    U++;
                    numberObstacle++;
                }

                if (timerGame >= 60 * 15 && U == 2)
                {
                    AfficherObstacle(numberObstacle, &positionObstacle, largeurObstacle, hauteurObstacle, &obstacle, randomX, randomY, hauteurEcran, mode);
                    numberObstacle++;
                    U++;
                }

                // Collision de la balle avec les obstacles
                for (i = 0; i < 3; i++)
                {

                    if (CheckCollisionCircleRec(positionBalle, rayonBalle, obstacle[i]))
                    {
                        for (int j = 0; j < 3; j++)
                        {
                            if (j != p)
                                BugObstacle[j] = true;
                        }

                        BugRaquette1 = true;
                        BugRaquette2 = true;

                        if (BugObstacle[i] == true)
                        {
                            // On utilise rand() pour generer un nombre aleatoire entre -1 et 1
                            float randomNumber = (float)rand() / RAND_MAX * 2 - 1;
                            // On change la trajectoire de la balle en fonction de ce nombre aleatoire
                            vitesseBalle.x *= -1;
                            vitesseBalle.y = randomNumber * vitesseBalleY;

                            BugObstacle[i] = false;
                            p = i;

                            PlaySound(son);
                        }
                    }
                }
            }
            // Vu que la fonction Drawtext s'utilise que avec les chaine de caractere alors je dois faire entrer mes variable BotMisses et playerMisses dans des chaine de caractere
            sprintf_s(botRateAffiche,sizeof(botRateAffiche), "%d", botRate);
            sprintf_s(joueurRateAffiche,sizeof(joueurRateAffiche), "%d", joueurRate);

            if (mode == 0)
            {
                if (dash2 >= 1)
                {
                    if (IsKeyPressed(KEY_ENTER))
                    {
                        dash2 -= 1;
                        vitesseBalle.x = vitesseBalle.x / 2;
                        vitesseBalle.y = vitesseBalle.y / 2;
                    }
                }
            }

            if (dash >= 1)
            {
                if (IsKeyPressed(KEY_SPACE))
                {
                    dash -= 1;
                    vitesseBalle.x = vitesseBalle.x / 2;
                    vitesseBalle.y = vitesseBalle.y / 2;
                }
            }

            // Remettre sur le menu principal du jeu 3 seconde apres que l'un des joueur est perdu

            if ((joueurRate >= 5 || botRate >= 5) && replay == true)
            {
                for (i = 0; i < 3; i++)
                {
                    obstacle[i].x = 0;
                    obstacle[i].y = 0;
                    obstacle[i].width = 0;
                    obstacle[i].height = 0;
                }
                joueurRate = 0;
                botRate = 0;
                timerGame = 0;
                U = 0;
                numberObstacle = 0;
                dash2 = 3;
                dash = 3;
                replay = false;
            }
        }


        // Dessin des ?elements du jeu
        BeginDrawing();

        // Dessin du premier menu ou on doit choisir entre jouer seul ou a 2
        if (Ecran == 0)
        {
            ClearBackground(couleurArrierePlan);
            DrawTexture(fond, 0, 0, WHITE);

            if (!CheckCollisionPointRec(GetMousePosition(), rectangleSingle) && !CheckCollisionPointRec(GetMousePosition(), rectangleMulti) && !CheckCollisionPointRec(GetMousePosition(), rectangleExit))
                sonjouer = false;

            // Afficher le bouton SinglePlayer
            if (CheckCollisionPointRec(GetMousePosition(), rectangleSingle))
            {
                if (!IsSoundPlaying(sonboutonOff) && sonjouer == false)
                {
                    PlaySound(sonboutonOff);
                    sonjouer = true;
                }

                DrawRectangleRec(rectangleSingle, couleurRectangleFonce);
                if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) || IsKeyReleased(KEY_ENTER))
                {
                    Ecran = 1;
                    mode = 1;
                    stockMode = mode;
                    PlaySound(sonbouton);
                }
            }
            else
                DrawRectangleRec(rectangleSingle, couleurRectangle);

            DrawTextEx(police, " Single", (Vector2) { largeurEcran / 2 - 160, hauteurEcran / 2 - 100 }, 50, 0, couleurEcriture);
            DrawTextEx(police, "       Player", (Vector2) { largeurEcran / 2 - 160, hauteurEcran / 2 - 58 }, 47, 0, couleurEcriture);

            // Afficher le bouton MultiPlayer
            if (CheckCollisionPointRec(GetMousePosition(), rectangleMulti))
            {
                if (!IsSoundPlaying(sonboutonOff) && sonjouer == false)
                {
                    PlaySound(sonboutonOff);
                    sonjouer = true;
                }

                DrawRectangleRec(rectangleMulti, couleurRectangleFonce);
                if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) || IsKeyReleased(KEY_ENTER))
                {
                    Ecran = 1;
                    mode = 0;
                    stockMode = mode;
                    PlaySound(sonbouton);
                }

            }
            else
                DrawRectangleRec(rectangleMulti, couleurRectangle);

            DrawTextEx(police, " Multi", (Vector2) { largeurEcran / 2 - 160, hauteurEcran / 2 + 27 }, 50, 0, couleurEcriture);
            DrawTextEx(police, "       Player", (Vector2) { largeurEcran / 2 - 160, hauteurEcran / 2 + 72 }, 47, 0, couleurEcriture);

            // Afficher le bouton Exit
            if (CheckCollisionPointRec(GetMousePosition(), rectangleExit))
            {
                if (!IsSoundPlaying(sonboutonOff) && sonjouer == false)
                {
                    PlaySound(sonboutonOff);
                    sonjouer = true;
                }

                DrawRectangleRec(rectangleExit, couleurRectangleFonce);
                if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) || IsKeyReleased(KEY_ENTER))
                {
                    PlaySound(sonbouton);
                    break;
                    
                    printf("fermer");
                }
            }
            else
                DrawRectangleRec(rectangleExit, couleurRectangle);

            DrawTextEx(police, "Exit", (Vector2) { largeurEcran / 2 - 100, hauteurEcran / 2 + 180 }, 65, 20, RED);
        }

        // Dessin du 2eme menu ou on doit choisir si on veut jouer au mode standard ou au mode custom
        if (Ecran == 1)
        {
            ClearBackground(couleurArrierePlan);
            DrawTexture(fond, 0, 0, WHITE);

            if (!CheckCollisionPointRec(GetMousePosition(), rectangleStandard) && !CheckCollisionPointRec(GetMousePosition(), rectangleCustom))
                sonjouer = false;

            DrawTextEx(police, "Choose a mode :", (Vector2) { largeurEcran / 2 - 250, hauteurEcran / 2 - 100 }, 50, 0, couleurEcriture);

            // Creation du bouton Standard
            if (CheckCollisionPointRec(GetMousePosition(), rectangleStandard))
            {
                if (sonjouer == false)
                {
                    PlaySound(sonboutonOff);
                    sonjouer = true;
                }

                DrawRectangleRec(rectangleStandard, couleurRectangleFonce);
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_ENTER))
                {
                    standard = true;
                    Ecran = 2;
                    dash = 0;
                    dash2 = 0;
                    stockStandard = standard;
                    PlaySound(sonbouton);
                }
            }
            else
                DrawRectangleRec(rectangleStandard, couleurRectangle);

            DrawTextEx(police, "Standard", (Vector2) { largeurEcran / 2 - 210, hauteurEcran / 2 + 5 }, 65, 0, couleurEcriture);

            // Creation du bouton Custom
            if (CheckCollisionPointRec(GetMousePosition(), rectangleCustom))
            {
                if (!IsSoundPlaying(sonboutonOff) && sonjouer == false)
                {
                    PlaySound(sonboutonOff);
                    sonjouer = true;
                }

                DrawRectangleRec(rectangleCustom, couleurRectangleFonce);
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_ENTER))
                {
                    standard = false;
                    Ecran = 2;
                    stockStandard = standard;
                    PlaySound(sonbouton);
                }
            }
            else
                DrawRectangleRec(rectangleCustom, couleurRectangle);

            DrawTextEx(police, "Custom", (Vector2) { largeurEcran / 2 - 190, hauteurEcran / 2 + 150 }, 65, 10, couleurEcriture);
        }

        // Dessin de l'interface de jeu
        if (Ecran == 2)
        {

            ClearBackground(RAYWHITE);

            if (mode == 1)
            {
                switch (dash)
                {
                case 3: DrawTexture(overlay3Single, 0, 0, WHITE);
                    break;
                case 2: DrawTexture(overlay2Single, 0, 0, WHITE);
                    break;
                case 1: DrawTexture(overlay1Single, 0, 0, WHITE);
                    break;
                case 0: DrawTexture(overlay0Single, 0, 0, WHITE);
                    break;
                }
            }

            if (mode == 0)
            {
                switch (dash)
                {
                case 3: DrawTexture(overlay3Multi_G, 0, 0, WHITE);
                    break;
                case 2: DrawTexture(overlay2Multi_G, 0, 0, WHITE);
                    break;
                case 1: DrawTexture(overlay1Multi_G, 0, 0, WHITE);
                    break;
                case 0: DrawTexture(overlay0Multi_G, 0, 0, WHITE);
                    break;
                }
                switch (dash2)
                {
                case 3: DrawTexture(overlay3Multi_D, 0, 0, WHITE);
                    break;
                case 2: DrawTexture(overlay2Multi_D, 0, 0, WHITE);
                    break;
                case 1: DrawTexture(overlay1Multi_D, 0, 0, WHITE);
                    break;
                case 0: DrawTexture(overlay0Multi_D, 0, 0, WHITE);
                    break;
                }
            }
            // Dessin du fond de jeu
            DrawTexture(background, 0, 0, WHITE);

            DrawRectangleRec(raquette1, couleurEcriture);
            DrawRectangleRec(raquette2, couleurEcriture);
            DrawRectangleRec(obstacle[0], BLACK);
            DrawRectangleRec(obstacle[1], BLACK);
            DrawRectangleRec(obstacle[2], BLACK);
            DrawCircleV(positionBalle, rayonBalle, WHITE);

            if (mode == 1)
            {
                DrawText(botRateAffiche, largeurEcran / 2 - 120, 15, 65, WHITE);
                DrawText(joueurRateAffiche, largeurEcran / 2 + 90, 15, 65, WHITE);
            }
            else if (mode == 0)
            {
                DrawText(botRateAffiche, largeurEcran / 2 - 76, 15, 65, WHITE);
                DrawText(joueurRateAffiche, largeurEcran / 2 + 43, 15, 65, WHITE);
            }
        }

        // Affichage du menu de victoire ou de defaite
        if (joueurRate == 5 || botRate == 5)
        {
            // Jouer le son de victoire ou de defaite a la fin de la game
            if (joueurRate == 5)
                PlaySound(sonPerdue);
            else
                PlaySound(sonGagne);

            while (replay != true)
            {
                // Verifie si le joueur ou le bot ont rate 5 fois
                if (joueurRate >= 5)
                {
                    // Affiche le message "vous avez perdu"
                    BeginDrawing();

                    ClearBackground(BLACK);
                    if (mode == 1)
                        DrawTexture(backgroundLooseSingle, 0, 0, WHITE);
                    else
                        DrawTexture(backgroundWinPlayer2, 0, 0, WHITE);

                    if (!CheckCollisionPointRec(GetMousePosition(), rectangleReplay) && !CheckCollisionPointRec(GetMousePosition(), rectangleMenu))
                        sonjouer = false;

                    // Affichage du bouton Replay
                    if (CheckCollisionPointRec(GetMousePosition(), rectangleReplay))
                    {
                        if (!IsSoundPlaying(sonboutonOff) && sonjouer == false)
                        {
                            PlaySound(sonboutonOff);
                            sonjouer = true;
                        }

                        DrawRectangleRec(rectangleReplay, couleurRectangleFonce);
                        if (IsKeyReleased(KEY_ENTER) || IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
                        {

                            mode = stockMode;
                            standard = stockStandard;
                            Ecran = 2;
                            replay = true;
                            PlaySound(sonbouton);
                        }
                    }
                    else
                        DrawRectangleRec(rectangleReplay, couleurRectangle);

                    DrawTextEx(police, "Replay", (Vector2) { largeurEcran / 2 - 155, hauteurEcran / 2 - 50 }, 65, 0, couleurEcriture);

                    // affichage du bouton Menu
                    if (CheckCollisionPointRec(GetMousePosition(), rectangleMenu))
                    {
                        if (!IsSoundPlaying(sonboutonOff) && sonjouer == false)
                        {
                            PlaySound(sonboutonOff);
                            sonjouer = true;
                        }

                        DrawRectangleRec(rectangleMenu, couleurRectangleFonce);
                        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) || IsKeyReleased(KEY_ENTER))
                        {
                            Ecran = 0;
                            replay = true;
                            PlaySound(sonbouton);
                        }
                    }
                    else
                        DrawRectangleRec(rectangleMenu, couleurRectangle);

                    DrawTextEx(police, "Menu", (Vector2) { largeurEcran / 2 - 152, hauteurEcran / 2 + 90 }, 65, 20, RED);

                    EndDrawing();
                }
                else if (botRate >= 5)
                {
                    // Affiche le message "vous avez gagn?"
                    BeginDrawing();

                    ClearBackground(BLACK);
                    if (mode == 1)
                        DrawTexture(backgroundWinSingle, 0, 0, WHITE);
                    else
                        DrawTexture(backgroundWinPlayer1, 0, 0, WHITE);

                    Rectangle rectangleReplay = { largeurEcran / 2 - 190,hauteurEcran / 2 - 70,366,100 };
                    if (CheckCollisionPointRec(GetMousePosition(), rectangleReplay))
                    {
                        PlaySound(sonboutonOff);
                        DrawRectangleRec(rectangleReplay, couleurRectangleFonce);
                        if (IsKeyReleased(KEY_ENTER) || IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
                        {
                            mode = stockMode;
                            standard = stockStandard;
                            Ecran = 2;
                            replay = true;
                            PlaySound(sonbouton);
                        }
                    }
                    else
                        DrawRectangleRec(rectangleReplay, couleurRectangle);

                    DrawTextEx(police, "Replay", (Vector2) { largeurEcran / 2 - 155, hauteurEcran / 2 - 50 }, 65, 0, couleurEcriture);

                    // affichage du bouton Menu
                    if (CheckCollisionPointRec(GetMousePosition(), rectangleMenu))
                    {
                        PlaySound(sonboutonOff);
                        DrawRectangleRec(rectangleMenu, couleurRectangleFonce);
                        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) || IsKeyReleased(KEY_ENTER))
                        {
                            Ecran = 0;
                            replay = true;
                            PlaySound(sonbouton);
                        }
                    }
                    else
                        DrawRectangleRec(rectangleMenu, couleurRectangle);

                    DrawTextEx(police, "Menu", (Vector2) { largeurEcran / 2 - 152, hauteurEcran / 2 + 90 }, 65, 20, RED);

                    EndDrawing();
                }
            }
        }

        EndDrawing();
    }

    // Decharger toute les ressources importe pour liberer de la place en memoire
    UnloadTexture(overlay3Single);
    UnloadTexture(overlay2Single);
    UnloadTexture(overlay1Single);
    UnloadTexture(overlay0Single);
    UnloadTexture(backgroundWinSingle);
    UnloadTexture(backgroundLooseSingle);
    UnloadTexture(overlay3Multi_G);
    UnloadTexture(overlay2Multi_G);
    UnloadTexture(overlay1Multi_G);
    UnloadTexture(overlay0Multi_G);
    UnloadTexture(overlay3Multi_D);
    UnloadTexture(overlay2Multi_D);
    UnloadTexture(overlay1Multi_D);
    UnloadTexture(overlay0Multi_D);
    UnloadTexture(backgroundWinPlayer1);
    UnloadTexture(backgroundWinPlayer2);
    UnloadTexture(fond);
    UnloadTexture(background);
    UnloadSound(sonboutonOff);
    UnloadSound(son);
    UnloadSound(sonbouton);

    CloseAudioDevice();

    return 0;

}

