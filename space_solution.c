#include "space_explorer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>


typedef struct Stack{
    unsigned int* data;
    unsigned int top;
    int capacity;
} Stack;

Stack* createStack(int capacity){
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    stack->capacity = capacity;
    stack->data = (unsigned int *)malloc(stack->capacity * sizeof(unsigned int));
    stack->top = -1;
    
    return stack;
}

int isStackEmpty(Stack *stack){
    return stack->top == -1;
}

void push(Stack *stack, unsigned int item)
{
    if (stack->top == stack->capacity - 1)
    {
        // Print the capacity before realloc
        printf("Capacity before realloc: %d\n", stack->capacity);

        // Increase the capacity of the stack by one
        stack->capacity += 1;
        stack->data = (unsigned int *)realloc(stack->data, stack->capacity * sizeof(unsigned int));
        if (stack->data == NULL) {
            fprintf(stderr, "Failed to allocate memory for stack\n");
            exit(EXIT_FAILURE);
        }

        // Print the capacity after realloc
        printf("Capacity after realloc: %d\n", stack->capacity);
    }
    stack->data[++stack->top] = item;
}

unsigned int pop(Stack *stack)
{
    if (isStackEmpty(stack))
    {
        printf("Stack underflow.\n");
        return -1;
    }
    return stack->data[stack->top--];
}



typedef struct
{
    unsigned int previous_planet;
    unsigned int *visited_planets; // Array of visited planets
    double *visited_distances;     // Array of distances from mixer
    int num_visited;
    int num_all_visited;
    Stack* stack;
} ShipState;

int is_planet_visited(unsigned int *visited_planets, int num_visited, unsigned int planet)
{
    for (int i = 0; i < num_visited; i++)
    {
        if (visited_planets[i] == planet)
        {
            return 1;
        }
    }
    return 0;
}

ShipAction space_hop(unsigned int crt_planet,
                    unsigned int *connections,
                    int num_connections,
                    double distance_from_mixer,
                    void *ship_state)
{
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
        state->stack = createStack(10);
        push(state->stack, crt_planet);
    } else {
        state = (ShipState *)ship_state;
    }

    if(is_planet_visited(state->visited_planets, state->num_visited, crt_planet)){
        printf("Planet already visited...\n"); // Debugging print statement
        exit(1);
    }

    state->visited_planets = realloc(state->visited_planets, sizeof(unsigned int)*(state->num_visited + 1));
    printf("Reallocated memory for visited planets...\n"); // Debugging print statement
    state->visited_planets[state->num_visited] = crt_planet;

  //  state->visited_distances = realloc(state->visited_distances, sizeof(double)*(state->num_visited + 1));
   // printf("Reallocated memory for visited distances...\n"); // Debugging print statement
    //state->visited_distances[state->num_visited] = distance_from_mixer;

    state->num_visited++;
    state->previous_planet = crt_planet;
    printf("Number of connections: %d\n", num_connections); // Debugging print statement

for(int i =0; i < num_connections; i++){
    printf("Pushing planet: %u\n", connections[i]); // Debugging print statement
    push(state->stack, connections[i]);
}

printf("Stack size after push: %d\n", state->stack->top + 1); // Debugging print statement

    printf("Pushed all connections to stack...\n"); // Debugging print statement

    unsigned int next;

    if (isStackEmpty(state->stack)){
        printf("Stack is empty... Random planet chosen\n"); // Debugging print statement
        exit(1);
        next = RAND_PLANET;}

    while(!isStackEmpty(state->stack)){
        next = pop(state->stack);
        if(!is_planet_visited(state->visited_planets, state->num_visited, next)){
            break;
        }
    }
    
    if (is_planet_visited(state->visited_planets, state->num_visited, next)) {
        printf("All planets visited...\n"); // Debugging print statement
        exit(1);
    next = RAND_PLANET;
}

    ShipAction action;
    action.next_planet = next;
    action.ship_state = state;
    printf("Next planet: %u\n", next);
    return action;
}