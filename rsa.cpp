#include <iostream>
#include <cstdlib>
#include <deque>
#include <ctime>
#include <fstream>
#include <random>

using namespace std;


// Random number generator using uniform [0,1)

double get_random(std::default_random_engine &gen,
                  std::uniform_real_distribution<double> &dist) {
    return dist(gen);
}

// Run one simulation for given m and n

double run_simulation(int m, double n,
                      std::default_random_engine &gen,
                      std::uniform_real_distribution<double> &dist)
{
    double test = 1000000;
    double sigma = 1.0 / (double)m;
    double delta = sigma / 2.0;

    double a = 0, b = 0, c = 0, d = 0;

    deque<double> occupied_max;
    deque<double> occupied_min;

    // Initialize with boundaries
    occupied_max.push_back(0);
    occupied_max.push_back(1);
    occupied_min.push_back(0);
    occupied_min.push_back(1);

    // Main loop
    for (int i = 0; i < test; i++) {

        double y = get_random(gen, dist);

        for (int j = 0; j < (int)occupied_max.size(); j++) {

            if (y > occupied_max[j] && y < occupied_min[j + 1]) {

                double f = occupied_max[j] - (n * delta);
                double g = occupied_min[j + 1] + (n * delta);

                double x = f + (g - f) * get_random(gen, dist);

                if (x >= f && x <= occupied_max[j] &&
                    (occupied_min[j + 1] - occupied_max[j]) >= sigma) {

                    occupied_max[j] += sigma;
                    a++;
                }

                if (x >= occupied_min[j + 1] && x <= g &&
                    (occupied_min[j + 1] - occupied_max[j]) >= sigma) {

                    occupied_min[j + 1] -= sigma;
                    b++;
                }

                if (x > occupied_max[j] && x < occupied_min[j + 1] &&
                    (occupied_min[j + 1] - occupied_max[j]) >= sigma) {

                    occupied_max.insert(occupied_max.begin() + j + 1, x);
                    occupied_min.insert(occupied_min.begin() + j + 1, x);
                    c++;
                }

                if (occupied_max.size() == test) break;
                else break;
            }
            else {
                d++;
                continue;
            }
        }
    }

    // Compute coverage p
    double p = 0;
    for (int i = 0; i < (int)occupied_min.size(); i++) {
        double q = occupied_max[i] - occupied_min[i];
        p += q;
    }

    return p;
}

// Main

int main()
{
    ofstream fout("ndelta_theta.txt");

    // Random generator
    std::default_random_engine generator(time(NULL));
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    int n = 5;   // change this to modify expansion factor

    for (int m = 5000; m <= 100000; m += 5000) {

        double s = 0, t = 0;

        for (int k = 0; k < 10; k++) {

            double p = run_simulation(m, n, generator, distribution);

            s += p;
            t = s / (k + 1);

            cout << p << '\t' << t << endl;
        }

        fout << 1.0 / (double)m << '\t' << t << endl;
    }

    return 0;
}
