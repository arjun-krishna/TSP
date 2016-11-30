#include <cstdio>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cmath>

using namespace std;
typedef vector<int> candidate;

class Data {
public:
	vector< pair<double,double> > cityCoords;
	vector< vector<double> > cost;
	int N;
	char s[30];

	Data() {	// N cities
		scanf("%s",s);
		scanf("%d",&N);
		
		// index from 1
		cityCoords.push_back(make_pair(0,0));
		vector< double> T;
		cost.push_back(T);

		for(int i=0;i<N;i++) {
			double x,y;
			scanf("%lf%lf",&x,&y);
			cityCoords.push_back(make_pair(x,y));
		}
		for(int i=0;i<N;i++) {
			vector<double> V(N+1);
			for(int j = 1;j<N;j++) {
				scanf("%lf",&V[j]);
			}
			cost.push_back(V);
		}
	}
	static int myrandom (int i) { return rand()%i;}

	candidate genRand(candidate C) {
		int l,a,b,temp;
		l = C.size();
		a = rand()%l;
		b = rand()%l;
		temp = C[a];
		C[a] = C[b];
		C[b] = temp;
		return C;
	}

	double evaluate(candidate C) {
		return tourCost(C);
	}

	double tourCost(candidate C) {
		int l = C.size();
		double tourCost = 0.0;
		l = l-1;
		for (int i=0;i <l; i++) {
			tourCost += cost[C[i]][C[i+1]];
		}
		tourCost += cost[C[l]][C[0]];
		return tourCost;
	}

	void print(candidate C) {
		for(int i=0;i<C.size();i++)
			printf("%d ",C[i]);
		printf("\n");
	}
};

class SimulatedAnnealing {
public:
	candidate best;
	double minTour;
	Data data;

	SimulatedAnnealing() {
		minTour = 0.0;
	} 
	double randProb()
	{
			double p = (rand() / (RAND_MAX + 1.0));
	    return p;
	}
	void run() {
		vector <int> V(data.N);
		for (int i=0;i < data.N;i++) {
			V[i] = i+1;
		}
		candidate c = data.genRand(V);
		best = c;
		minTour = data.tourCost(c);
		for(int i=0;i<1000;i++) {
			random_shuffle(c.begin(),c.end());
			double tCost = data.tourCost(c);
			if(tCost < minTour) {
				minTour = tCost;
				best = c;
			}
		}
		c = best;
		double p,expP,deltaE;
		double Temp = 1000000000;
		for(double T=Temp;T > 0.01;T*=0.5) {
			for(int i=0;i<100000;i++) {
				candidate n = data.genRand(c);
				//data.print(n);
				deltaE = eval(c) - eval(n);
				deltaE /= T;
				p = 1 / (1+exp(-1*deltaE));
				expP = randProb();
				// printf("p : %lf , expP : %lf\n",p,expP);
				if(expP < p) { // move
					c = n;
					double tCost = data.tourCost(c);
					if(tCost < minTour ) {
						best = c;
						minTour = tCost;
					}
				}	
			}
		}
		printf("%lf\n", minTour);
	}

	double eval(candidate C) {
		return data.evaluate(C);
	}

};

int main(void) {
	time_t sysTime;
	srand(time(&sysTime));
	SimulatedAnnealing SA;					// algorithm
	SA.run();
	return 0;
}