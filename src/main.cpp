#include "raylib.h"
#include <vector>
#include <cstdlib> // Pour rand()

// Configuration de la simulation
const int CELL_SIZE = 2;                          // Taille d'une cellule (pixels)
const int SCREEN_WIDTH = 800;                     // Largeur de la fenêtre
const int SCREEN_HEIGHT = 600;                    // Hauteur de la fenêtre
const int GRID_WIDTH = SCREEN_WIDTH / CELL_SIZE;  // Nombre de cellules en largeur
const int GRID_HEIGHT = SCREEN_HEIGHT / CELL_SIZE; // Nombre de cellules en hauteur

// Grilles : une pour l'état actuel, une temporaire pour la mise à jour
std::vector<std::vector<int>> grid(GRID_HEIGHT, std::vector<int>(GRID_WIDTH));
std::vector<std::vector<int>> tempGrid(GRID_HEIGHT, std::vector<int>(GRID_WIDTH));

// Initialise la grille avec des cellules vivantes/mortes aléatoires
void initializeGrid() {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            grid[y][x] = rand() % 2; // 0 ou 1 aléatoire
        }
    }
}

// Dessine uniquement les cellules vivantes pour améliorer les performances
void drawGrid() {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (grid[y][x] == 1) { // Dessine uniquement les cellules vivantes
                DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, GREEN);
            }
        }
    }
}

// Compte les voisins vivants autour de la cellule (x, y)
int countNeighbors(int x, int y) {
    int count = 0;
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dx == 0 && dy == 0) continue; // Ignorer la cellule elle-même

            int nx = x + dx; // Nouvelle position en X
            int ny = y + dy; // Nouvelle position en Y

            // Vérifier si la cellule voisine est dans les limites de la grille
            if (nx >= 0 && nx < GRID_WIDTH && ny >= 0 && ny < GRID_HEIGHT) {
                count += grid[ny][nx]; // Ajouter 1 si la cellule est vivante
            }
        }
    }
    return count;
}

// Met à jour la grille selon les règles de Conway
void updateGrid() {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            int neighbors = countNeighbors(x, y);

            if (grid[y][x] == 1) { // Cellule vivante
                tempGrid[y][x] = (neighbors == 2 || neighbors == 3) ? 1 : 0;
            } else { // Cellule morte
                tempGrid[y][x] = (neighbors == 3) ? 1 : 0;
            }
        }
    }

    // Copier tempGrid dans grid
    grid = tempGrid;
}

int main() {
    // Initialisation de la fenêtre
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Jeu de la Vie - Grande Population");
    SetTargetFPS(30); // Ajuste la vitesse de la simulation

    // Initialiser la grille
    initializeGrid();

    // Boucle principale
    while (!WindowShouldClose()) {
        // Mise à jour et affichage
        BeginDrawing();
        ClearBackground(BLACK);

        drawGrid();  // Dessiner la grille actuelle
        updateGrid(); // Mettre à jour la grille

        EndDrawing();
    }

    // Nettoyage et fermeture
    CloseWindow();
    return 0;
}
