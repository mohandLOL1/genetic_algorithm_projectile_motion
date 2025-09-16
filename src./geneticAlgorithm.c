#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define G 9.81
#define PI 3.14159
#define TARGETX 200.0
#define TARGETY 50.0
#define POP_SIZE 200
#define GENERATIONS 100
#define KH 30 // Penalty constant for the height

double angles[POP_SIZE];
double velocity[POP_SIZE];
double fitness[POP_SIZE];
float mutationDegree = 0.5;
// Function definitions
double randomDouble(double max, double min)
{
    return min + (rand() / (double)RAND_MAX) * (max - min);
}

double height(double a, double v)
{
    double radians = a * PI / 180.0;
    return (TARGETX * tan(radians) - (G * TARGETX * TARGETX) / (2.0 * (v * v * cos(radians) * cos(radians))));
}

double totalEnergy(double v)
{
    return (0.5 * v * v);
}

double penalty(double height)
{
    return KH * fabs(TARGETY - height);
}

double calculateFitness(double totalEnergy, double penalty)
{
    return (totalEnergy + penalty); // The lower, the better
}

void bubbleSort(double angles[], double velocity[], double fitness[])
{
    double temp;
    for (int i = 0; i < POP_SIZE - 1; i++)
    {
        for (int j = 0; j < POP_SIZE - i - 1; j++)
        {
            if (fitness[j] > fitness[j + 1])
            {
                // Swap fitness
                temp = fitness[j];
                fitness[j] = fitness[j + 1];
                fitness[j + 1] = temp;

                // Swap angles
                temp = angles[j];
                angles[j] = angles[j + 1];
                angles[j + 1] = temp;

                // Swap velocity
                temp = velocity[j];
                velocity[j] = velocity[j + 1];
                velocity[j + 1] = temp;
            }
        }
    }
}

int parentSelection()
{
    int best = rand() % POP_SIZE / 2;
    for (int i = 0; i < 15; i++)
    {
        int contender = rand() % POP_SIZE / 2;
        if (fitness[best] > fitness[contender])
        {
            best = contender;
        }
    }
    return best;
}

double crossover(double parentA1, double parentA2)
{
    return (parentA1 + parentA2) / 2.0;
}

double mutationA(double a)
{
    if ((rand() / (double)RAND_MAX) < mutationDegree)
    {
        a += randomDouble(0.5, -0.5);
        if (a > 90)
            a = 90.0;
        if (a < 0)
            a = 0.0;
    }
    return a;
}

double mutationV(double v)
{
    if ((rand() / (double)RAND_MAX) < mutationDegree)
    {
        v += randomDouble(1.0, -1.0);
        if (v > 300.0)
            v = 300.0;
        if (v < 0)
            v = 5;
    }
    return v;
}

void intializePopulation()
{
    for (int i = 0; i < POP_SIZE; i++)
    {
        angles[i] = randomDouble(90.0, 0.0);
        velocity[i] = randomDouble(300.0, 0.0);
        fitness[i] = calculateFitness(
            totalEnergy(velocity[i]),
            penalty(height(angles[i], velocity[i])));
    }
}
void saveGenerationData(int generation, double angles[], double velocity[], double fitness[]) {
    char filename[50];
    sprintf(filename, "generation_%d.txt", generation); // Create a unique file name for each generation

    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        perror("Error opening file");
        exit(1);
    }

    fprintf(fp, "Angle\tVelocity\tFitness\n");
    for (int i = 0; i < POP_SIZE; i++) {
        fprintf(fp, "%.2lf\t%.2lf\t%.5lf\n", angles[i], velocity[i], fitness[i]);
    }

    fclose(fp);}

void geneticAlgorithm()
{
    intializePopulation();
    saveGenerationData(0, angles, velocity, fitness);
    bubbleSort(angles, velocity, fitness);
    for (int i = 1; i <= GENERATIONS; i++)
    {
        double newAngles[POP_SIZE];
        double newVelocity[POP_SIZE];
        double newFitness[POP_SIZE];

        newAngles[0] = angles[0];
        newVelocity[0] = velocity[0]; // Elitism
        newFitness[0] = fitness[0];
        mutationDegree = mutationDegree + 0.2 * mutationDegree; //mutation degree increases each generation to increase pressure 

        for (int j = 1; j < POP_SIZE; j++)
        {
            int indexParent1 = parentSelection();
            int indexParent2 = parentSelection();
            newVelocity[j] = mutationV(crossover(velocity[indexParent1], velocity[indexParent2]));
            newAngles[j] = mutationA(crossover(angles[indexParent1], angles[indexParent2]));
            newFitness[j] = calculateFitness(
                totalEnergy(newVelocity[j]),
                penalty(height(newAngles[j], newVelocity[j])));
        }

        for (int k = 0; k < POP_SIZE; k++)
        {
            angles[k] = newAngles[k];
            velocity[k] = newVelocity[k];
            fitness[k] = newFitness[k];
        }

        bubbleSort(angles, velocity, fitness);
        saveGenerationData(i, angles, velocity, fitness);
        printf("Best element from generation %d: Angle = %.2lf, Velocity = %.2lf, Height Error = %.5lf Fitness = %.5lf, Energy= %.5lf\n",
               i, angles[0], velocity[0], fabs(TARGETY - height(angles[0],velocity[0])), fitness[0], totalEnergy(velocity[0]));
                
    }
}

int main()
{
    srand(time(0));
    clock_t start_time = clock();

    geneticAlgorithm();

    clock_t end_time = clock();
    double time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("Time taken: %.2f seconds\n", time_taken);
    return 0;
}