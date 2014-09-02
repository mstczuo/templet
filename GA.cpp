#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

/* Change any of these parameters to match your needs */

#define POPSIZE 100                /* population size */
#define MAXGENS 500               /* max. number of generations */
#define NVARS 2                    /* no. of problem variables */
#define __PXOVER 0.7               /* probability of crossover */
#define __PMUTATION 0.07           /* probability of mutation */
double PXOVER;
double PMUTATION;

int generation;                  /* current generation no. */
int cur_best;                    /* best individual */
FILE *galog;                     /* an output(1) file */
FILE *output;                    /* an output(1) file */

struct genotype /* genotype (GT), a member of the population */
{
    double gene[NVARS];        /* a string of variables */
    double fitness;            /* GT's fitness */
    double upper[NVARS];       /* GT's variables upper bound */
    double lower[NVARS];       /* GT's variables lower bound */
    double rfitness;           /* relative fitness */
    double cfitness;           /* cumulative fitness */
};

struct genotype population[POPSIZE+1];    /* population */
struct genotype newpopulation[POPSIZE+1]; /* new population; */
/* replaces the */
/* old generation */

/* Declaration of procedures used by this genetic algorithm */

void initialize(void);
double randval(double, double);
void evaluate(void);
void keep_the_best(void);
void elitist(void);
void select(void);
void crossover(void);
void Xover(int,int);
void swap(double *, double *);
void mutate(void);
void report(void);

void get_parameter(int argc,char *argv[]);

/***************************************************************/
/* Initialization function: Initializes the values of genes    */
/* within the variables bounds. It also initializes (to zero)  */
/* all fitness values for each member of the population. It    */
/* reads upper and lower bounds of each variable from the      */
/* input file `gadata.txt'. It randomly generates values       */
/* between these bounds for each gene of each genotype in the  */
/* population. The format of the input file `gadata.txt' is    */
/* var1_lower_bound var1_upper bound                           */
/* var2_lower_bound var2_upper bound ...                       */
/***************************************************************/

void initialize(void)
{
    FILE *infile;
    int i, j;
    double lbound, ubound;

    if ((infile = fopen("gadata.txt","r"))==NULL)
    {
        fprintf(galog,"\nCannot open input file!\n");
        exit(1);
    }

    remove("output.dat");
    /* initialize variables within the bounds */

    for (i = 0; i < NVARS; i++)
    {
        fscanf(infile, "%lf",&lbound);
        fscanf(infile, "%lf",&ubound);


        for (j = 0; j < POPSIZE; j++)
        {
            population[j].fitness  = 0;
            population[j].rfitness = 0;
            population[j].cfitness = 0;
            population[j].lower[i] = lbound;
            population[j].upper[i] = ubound;
            population[j].gene[i]  = randval(population[j].lower[i],
                    population[j].upper[i]);
        }
    }
    population[POPSIZE].fitness = 0;

    fclose(infile);
}

/***********************************************************/
/* Random value generator: Generates a value within bounds */
/***********************************************************/

double randval(double low, double high)
{
    double val;
    val = (rand() * 1.0 / RAND_MAX)*(high - low) + low;
    return(val);
}

/*************************************************************/
/* Evaluation function: This takes a user defined function.  */
/* Each time this is changed, the code has to be recompiled. */
/* The current function is:  x[1]^2-x[1]*x[2]+x[3]           */
/*************************************************************/

void evaluate(void)
{
    if ((output = fopen("output.dat","a"))==NULL)
    {
        puts("File error");
        exit(1);
    }
    int mem;
    int i;
    double x[NVARS+1];

    for (mem = 0; mem < POPSIZE; mem++)
    {
        for (i = 0; i < NVARS; i++)
            x[i+1] = population[mem].gene[i];

        population[mem].fitness = 21.5 + x[1] * sin(4 * M_PI * x[1]) + x[2] * sin(20 * M_PI * x[2]);

        if (population[mem].fitness <40 && population[mem].fitness>35)
        {
            fprintf(output, "\n%5d,      %6.9f, %6.9f, %6.9f ", generation, 
                    x[1],x[2],population[mem].fitness);
        }
    }
    fclose(output);
}

/***************************************************************/
/* Keep_the_best function: This function keeps track of the    */
/* best member of the population. Note that the last entry in  */
/* the array Population holds a copy of the best individual    */
/***************************************************************/

void keep_the_best()
{
    int mem;
    int i;
    cur_best = 0; /* stores the index of the best individual */

    for (mem = 0; mem < POPSIZE; mem++)
    {
        if (population[mem].fitness > population[POPSIZE].fitness)
        {
            cur_best = mem;
            population[POPSIZE].fitness = population[mem].fitness;
        }
    }
    /* once the best member in the population is found, copy the genes */
    for (i = 0; i < NVARS; i++)
        population[POPSIZE].gene[i] = population[cur_best].gene[i];
}

/****************************************************************/
/* Elitist function: The best member of the previous generation */
/* is stored as the last in the array. If the best member of    */
/* the current generation is worse then the best member of the  */
/* previous generation, the latter one would replace the worst  */
/* member of the current population                             */
/****************************************************************/

void elitist()
{
    int i;
    double best, worst;             /* best and worst fitness values */
    int best_mem, worst_mem; /* indexes of the best and worst member */

    best = population[0].fitness;
    worst = population[0].fitness;
    for (i = 0; i < POPSIZE; ++i)
    {
        if (population[i].fitness >= best)
        {
            best = population[i].fitness;
            best_mem = i;
        }
        if (population[i].fitness <= worst)
        {
            worst = population[i].fitness;
            worst_mem = i;
        }
    }
    /* if best individual from the new population is better than */
    /* the best individual from the previous population, then    */
    /* copy the best from the new population; else replace the   */
    /* worst individual from the current population with the     */
    /* best one from the previous generation                     */

    if (best >= population[POPSIZE].fitness) {
        population[POPSIZE] = population[best_mem];
    }
    else {
        population[worst_mem]= population[POPSIZE];
    } 
}
/**************************************************************/
/* Selection function: Standard proportional selection for    */
/* maximization problems incorporating elitist model - makes  */
/* sure that the best member survives                         */
/**************************************************************/

int find(double x) {
    if(population[0].cfitness > x) return 0;
    int l = 0, r = POPSIZE - 1;
    while(r - l > 1) {
        int mid = (l + r) >> 1;
        if(population[mid].cfitness > x) 
            r = mid;
        else l = mid;
    }
    return r;
}

void select(void)
{
    int mem, i, j;
    double sum = 0;
    double p;

    /* find total fitness of the population */
    for (mem = 0; mem < POPSIZE; mem++)
    {
        sum += population[mem].fitness;
    }

    /* calculate relative fitness */
    for (mem = 0; mem < POPSIZE; mem++)
    {
        population[mem].rfitness =  population[mem].fitness/sum;
    }
    population[0].cfitness = population[0].rfitness;

    /* calculate cumulative fitness */
    for (mem = 1; mem < POPSIZE; mem++)
    {
        population[mem].cfitness =  population[mem-1].cfitness +       
            population[mem].rfitness;
    }

    /* finally select survivors using cumulative fitness. */

    for (i = 0; i < POPSIZE; i++)
    { 
        p = rand() * 1.0 / RAND_MAX;
        j = find(p);
        newpopulation[i] = population[j];
        /*
        if (p < population[0].cfitness)
            newpopulation[i] = population[0];      
        else
        {
            for (j = 0; j < POPSIZE;j++)      
                if (p >= population[j].cfitness && 
                        p<population[j+1].cfitness)
                    newpopulation[i] = population[j+1];
        }
        */
    }
    /* once a new population is created, copy it back */

    for (i = 0; i < POPSIZE; i++)
        population[i] = newpopulation[i];      
}

/***************************************************************/
/* Crossover selection: selects two parents that take part in  */
/* the crossover. Implements a single point crossover          */
/***************************************************************/

void crossover(void)
{
    int mem, one;
    int first  =  0; /* count of the number of members chosen */
    double x;

    for (mem = 0; mem < POPSIZE; ++mem)
    {
        x = rand() * 1.0 / RAND_MAX;
        if (x < PXOVER)
        {
            ++first;
            if (first % 2 == 0)
                Xover(one, mem);
            else
                one = mem;
        }
    }
}
/**************************************************************/
/* Crossover: performs crossover of the two selected parents. */
/**************************************************************/

void Xover(int one, int two)
{
    int i;
    int point; /* crossover point */
    /* select crossover point */
    if(NVARS > 1)
    {
        if(NVARS == 2)
            point = 1;
        else
            point = (rand() % (NVARS - 1)) + 1;
        for (i = 0; i < point; i++)
            swap(&population[one].gene[i], &population[two].gene[i]);
    }
}

/*************************************************************/
/* Swap: A swap procedure that helps in swapping 2 variables */
/*************************************************************/

void swap(double *x, double *y)
{
    double temp;

    temp = *x;
    *x = *y;
    *y = temp;
}

/**************************************************************/
/* Mutation: Random uniform mutation. A variable selected for */
/* mutation is replaced by a random value between lower and   */
/* upper bounds of this variable                              */
/**************************************************************/

void mutate(void)
{
    int i, j;
    double lbound, hbound;
    double x;

    for (i = 0; i < POPSIZE; i++)
        for (j = 0; j < NVARS; j++)
        {
            x = rand() * 1.0 / RAND_MAX;
            if (x < PMUTATION)
            {
                /* find the bounds on the variable to be mutated */
                lbound = population[i].lower[j];
                hbound = population[i].upper[j];  
                population[i].gene[j] = randval(lbound, hbound);
            }
        }
}

/***************************************************************/
/* Report function: Reports progress of the simulation. Data   */
/* dumped into the  output file are separated by commas        */
/***************************************************************/

void report(void)
{
    int i;
    double best_val;            /* best population fitness */
    double avg;                 /* avg population fitness */
    double stddev;              /* std. deviation of population fitness */
    double sum_square;          /* sum of square for std. calc */
    double square_sum;          /* square of sum for std. calc */
    double sum;                 /* total population fitness */

    sum = 0.0;
    sum_square = 0.0;

    for (i = 0; i < POPSIZE; i++)
    {
        sum += population[i].fitness;
        sum_square += population[i].fitness * population[i].fitness;
    }

    avg = sum/(double)POPSIZE;
    square_sum = avg * avg * POPSIZE;
    stddev = sqrt((sum_square - square_sum)/(POPSIZE));
    best_val = population[POPSIZE].fitness;

    fprintf(galog, "\n%5d,      %6.9f, %6.9f, %6.9f ", generation, 
            best_val, avg, stddev);
}


/**************************************************************/
/* Main function: Each generation involves selecting the best */
/* members, performing crossover & mutation and then          */
/* evaluating the resulting population, until the terminating */
/* condition is satisfied                                     */
/**************************************************************/

void run(int argc,char *argv[])
{
    if ((galog = fopen("galog.txt","w"))==NULL) {
        exit(1);
    }
    generation = 0;

    fprintf(galog, "\n generation  best  average  standard \n");
    fprintf(galog, " number value fitness  deviation \n");

    initialize();
    evaluate();
    keep_the_best();
    while(generation<MAXGENS)
    {
        generation++;
        select();
        crossover();
        mutate();
        report();
        evaluate();
        elitist();
    }
    fprintf(galog,"\n\n Simulation completed\n");
    fprintf(galog,"\n Best member: \n");

    int i;
    for (i = 0; i < NVARS; i++)
    {
        fprintf (galog,"\n var(%d) = %3.9f",i,population[POPSIZE].gene[i]);
    }
    fprintf(galog,"\n\n Best fitness = %3.9f",population[POPSIZE].fitness);
    fclose(galog);
    void add_result();
    add_result();
}
/***************************************************************/


void add_result() {
    FILE * file;
    if ((file = fopen("logdata.txt","a"))==NULL) {
        fprintf(galog,"\nCannot open input file!\n");
        exit(1);
    }
    fprintf(file, "%lf\n", population[POPSIZE].fitness);
    fclose(file);
}

void get_parameter(int argc,char *argv[]) {
    int c;
    double val;
    PXOVER = __PXOVER;
    PMUTATION = __PMUTATION;
    while((c = getopt(argc,argv,"c:m:")) != -1) {
        switch(c) {
            case 'c':
                sscanf(optarg, "%lf", &val);
                PXOVER = val;
                printf("set crossover %lf\n", val);
                break;
            case 'm':
                sscanf(optarg, "%lf", &val);
                PMUTATION = val;
                printf("set mutation %lf\n", val);
                break;
            default:
                abort();
        }
    }
}

int main(int argc,char *argv[]) {
    remove("logdata.txt");
    srand((unsigned)time(NULL));
    get_parameter(argc, argv);
    for(int i = 0; i < 30; ++i) {
        run(argc, argv);
    }
    printf("Program execute normally\n");
    return 0;
}