#include "space_explorer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

struct Planet {
    int id;
    int diameter;
};

struct Node {
    struct Planet planet;
    struct Node* next;
};

struct Graph {
    int V; // Number of vertices
    struct Node** adjList; // Adjacency list
};


struct Node* createNode(struct Planet planet) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->planet = planet;
    newNode->next = NULL;
    return newNode;
}


struct Graph* createGraph() {
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->V = 0;
    graph->adjList = NULL;
    return graph;
}

void addVertex(struct Graph* graph) {
    graph->V++;
    graph->adjList = (struct Node**)realloc(graph->adjList, graph->V * sizeof(struct Node*));
    graph->adjList[graph->V - 1] = NULL;
}

// Function to add an edge to an undirected graph
void addEdge(struct Graph* graph, int src, int dest, struct Planet srcPlanet, struct Planet destPlanet) {
    if (src >= graph->V || dest >= graph->V) {
        printf("Error: Trying to add edge for non-existent vertices\n");
        return;
    }
    // Add an edge from src to dest
    struct Node* newNode = createNode(destPlanet);
    newNode->next = graph->adjList[src];
    graph->adjList[src] = newNode;

    // Since the graph is undirected, add an edge from dest to src as well
    newNode = createNode(srcPlanet);
    newNode->next = graph->adjList[dest];
    graph->adjList[dest] = newNode;
}

typedef struct {
    unsigned int previous_planet;
    unsigned int *visited_planets; // Array of visited planets
    double *visited_distances; // Array of distances from mixer
    int num_visited;
} ShipState;

int is_planet_visited(unsigned int *visited_planets, int num_visited, unsigned int planet) {
    for (int i = 0; i < num_visited; i++) {
        if (visited_planets[i] == planet) {
            return 1;
        }
    }
    return 0;
}

ShipAction space_hop(unsigned int crt_planet,
                    unsigned int *connections,
                    int num_connections,
                    double distance_from_mixer,
                    void *ship_state){


    //if ship_state is NULL, allocate memory for it
    printf("Current planet: %u\n", crt_planet);
    ShipState *state;
    if (ship_state == NULL) {
        
        printf("Allocating memory for ship state...\n");
        state = malloc(sizeof(ShipState));
        if (state == NULL) {
            fprintf(stderr, "Memory allocation failed in shipAction\n");
            exit(EXIT_FAILURE);
        }
        //allocate memory for visited planets
        state->visited_planets = malloc(sizeof(unsigned int));
        state->num_visited = 0;
        state->visited_distances = malloc(sizeof(double));
    } else {
        state = (ShipState *)ship_state;
    }

    state->visited_planets = realloc(state->visited_planets, sizeof(unsigned int)*(state->num_visited + 1));
    state->visited_planets[state->num_visited] = crt_planet;

    state->visited_distances = realloc(state->visited_distances, sizeof(double)*(state->num_visited + 1));
    state->visited_distances[state->num_visited] = distance_from_mixer;

    state->num_visited++;
    state->previous_planet = crt_planet;


    unsigned int next;
    int all_visited = 1;
for (int i = 0; i < num_connections; i++) {
    if (!is_planet_visited(state->visited_planets, state->num_visited, connections[i])) {
        all_visited = 0;
        break;
    }
}

if (all_visited) {
    int min_distance_index = 0;
for (int i = 1; i < state->num_visited; i++) {
    if (state->visited_distances[i] < state->visited_distances[min_distance_index]) {
        min_distance_index = i;
    }
}   next = state->visited_planets[min_distance_index];
} else {
    do {
        int index = rand() % num_connections;
        next = connections[index];
    } while(is_planet_visited(state->visited_planets, state->num_visited, next));
}   
    


    ShipAction action = { .next_planet = next, .ship_state = state };
    printf("Next planet chosen: %u\n", next);
    return action;
}





