#include <iostream>
#include <math.h>
#include <stdio.h>

#define SIEVE_RANGE (1000000)
#define PRIME_COUNT (78498)

bool sieve_visited[SIEVE_RANGE] = {};
long long prime[PRIME_COUNT] = {};

void Sieve()
{
    int curr_pos = 0;
    for (long long i = 2; i < SIEVE_RANGE; i++)
    {
        if (!sieve_visited[i])
        {
            prime[curr_pos] = i;
            curr_pos++;
            for (long long j = i + i; j < SIEVE_RANGE; j += i)
            {
                sieve_visited[j] = true;
            }
        }
    }
}

void Solve()
{
    long long count = 0;
    for (int i = 0; i < PRIME_COUNT; i++)
    {
        for (long long n = 1; 3*n*n <= prime[i]; n++)
        {
            long long x = (long long)(
                (sqrt((double)(4*prime[i] - 3*n*n)) - (double)(n))*0.5);
            for (long long m = x-1; m <= x+1; m++)
            {
                if (m > n && n*n % (m-n) == 0 && m*m + m*n + n*n == prime[i])
                {
                    count++;
                }
            }
        }
    }
    std::cout << count << std::endl;
}

int main(int argc, char* argv[])
{
    Sieve();
    Solve();
    return 0;
}
