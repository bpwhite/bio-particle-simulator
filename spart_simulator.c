#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <term.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <time.h>

#define MAX_PARTICLES 1000

// Structures
struct particle {
	int particle_id;
	int x;
	int y;
	float velocity;
	float momentum;
	float direction;
	
} particles[MAX_PARTICLES];

// Function prototypes
float dist2d (float *x1, float *x2, float *y1, float *y2);
void ClearScreen();
void spawn_particle(void);
void set_particles(void);
float rand_num(gsl_rng *r, float sigma, float mu);

// Declare simulation starting values
// number of starting particles
int num_start_particles		= 100;
int num_current_particles 	= 0;
// number of steps in the simulation
int num_steps 					= 100000000;
// default mean strength of brownian motion force
int bf_mu			= 2;
// default mean particle size 
int p_size_mu		= 5;
// default mean electrostatic force strength
int ef_mu	= 100;
// default mean electrostatic radius
int ef_r_mu	= 25;
// matrix width (x) and height (y)
int mtrx_x	= 50;
int mtrx_y	= 50;


int main() {
	char code;

   // timer
	time_t start,end;
	time (&start);
	
	// start random number generator
	gsl_rng *r = gsl_rng_alloc(gsl_rng_mt19937);
  	gsl_rng_set (r, time(NULL));
  	
	ClearScreen();
	goto begin;
	for (;;) {
		ClearScreen();
		printf("Menu:\n");
		printf("(p) # starting particles: %d\n", num_start_particles);
		printf("(s) # simulation steps: %d\n", num_steps);
		printf("(b) brownian motion strength: %d\n", bf_mu);
		scanf(" %c", &code);
		while (getchar() != '\n') /* skips to end of line */
			;
		switch (code) {
			case 'p' : 	set_particles();
							ClearScreen();
							break;
			case 's' : 	scanf("%d", &num_steps);
							/*							
							while (num_start_particles > MAX_PARTICLES) {
								printf("Max particles allowed is %d\n", MAX_PARTICLES);
								printf("Enter a number of particles less than %d\n", MAX_PARTICLES);
								scanf("%d", &num_steps);
							}
							*/
							ClearScreen();
							break;
			case 'u' :
							ClearScreen();
							break;
			case 'x' : 
							ClearScreen();
							break;
			case 'b' : goto begin;
			
			case 'q' : return 0;
			default: printf("Illegal code\n");
		}
		printf("\n");
	}
	begin:

	// Begin simulation
	ClearScreen();
	
	// Spawn particles
	printf("Spawning particles...\n");
	int particle_i;
	for(particle_i = 0;
		particle_i < num_start_particles && particle_i < MAX_PARTICLES; 
		particle_i++)
		spawn_particle();
	//printf("Spawned %d particles\n",num_current_particles);
	
	// Compute distance matrix
	//printf("Timestamp: %d\n",(int)time(NULL));
	float x1,x2,y1,y2;
	x1 = 1;
	x2 = 5;
	y1 = 1;
	y2 = 5;
	float dist;
	
	
	// print to file
	FILE *f = fopen("output.csv", "w");
	if (f == NULL)
	{
	    printf("Error opening file!\n");
	    exit(1);
	}
	
	ClearScreen();
	int steps_i;
	int progress = 1;
	int progress_bar = 0;
	int progress_bar_i;
	float rand_num_gauss;
	float step_interval = round(0.0001*num_steps);
	// Main simulation loop
	for(steps_i = 0; steps_i < num_steps; steps_i++) {
		dist = dist2d(&x1,&x2,&y1,&y2);

		
		if(progress == step_interval) {
		//if((steps_i % 1000) == 0) {
			ClearScreen();
			printf("Step: %d\n", steps_i);
			
			// Random Gaussian variate = r, sigma + mu
			rand_num_gauss = rand_num(r,2, p_size_mu);
			
			printf("Rand num: %f\n",rand_num_gauss);
			
			// Progress Bar			
			printf("Progress: %d\n", progress);
			progress = 0;
			progress_bar++;
			float pcnt_progress = (step_interval*progress_bar)/(float)num_steps*100;
			
			printf("|");
			for(progress_bar_i = 0; progress_bar_i < pcnt_progress; progress_bar_i++)
				printf("=");
			printf("| %.1lf%%\n", pcnt_progress);
			
			// Elapsed Time
			time (&end);
			double dif = difftime (end,start);
			printf ("Elasped time is %.2lf seconds.\n", dif );
			

			/* print some text */
			//const char *text = "Write this to the file";
			//fprintf(f, "Some text: %s\n", text);
			
			/* print integers and floats */

			fprintf(f, "%d,%f\n", steps_i,rand_num_gauss);
			
			/* printing single chatacters */
			//char c = 'A';
			//fprintf(f, "A character: %c\n", c);
			
			
			
			
		}
		progress++;
		
 	}
 	fclose(f);
 	printf("\nResults: \n");
 	//time (&end);
	//double dif = difftime (end,start); 
	//printf ("Elasped time is %.2lf seconds.\n", dif ); 
 	printf("Steps: %d\n",steps_i);
	printf("Dist: %f\n",dist);

	return 0;
}

void ClearScreen() {
	if (!cur_term) {
		int result;
		setupterm( NULL, STDOUT_FILENO, &result );
		if (result <= 0) return;
	}

	putp( tigetstr( "clear" ) );
}
  
float dist2d (float *x1, float *x2, float *y1, float *y2) {
	return sqrt(pow((*x1-*x2),2)+pow(*y1-*y2,2));
}

float rand_num(gsl_rng *r, float sigma, float mu) {
	int i,n;
	n = 1;
  	float gauss; 
  	
  	for (i=0;i<n;i++) {
      gauss=gsl_ran_gaussian(r,sigma) + mu;
     // printf("%2.4f\n", gauss);
	}
	return gauss;
}

void set_particles(void) {
	printf("Enter # of particles less than %d: ",MAX_PARTICLES);
	scanf("%d", &num_start_particles);
	while (num_start_particles > MAX_PARTICLES) {
		printf("Error, max particles allowed is %d\n", MAX_PARTICLES);
		//printf("Enter a number of particles less than %d\n", MAX_PARTICLES);
		scanf("%d", &num_start_particles);
	}
}

void spawn_particle(void) {
	if(num_current_particles == MAX_PARTICLES) {
		printf("Simulation at capacity; can't add more particles.\n");
		return;
	}
	particles[num_current_particles+1].particle_id = num_current_particles+1;
	printf("Spawned particle #: %d\n", particles[num_current_particles].particle_id);
	
	
	
	num_current_particles++;
}