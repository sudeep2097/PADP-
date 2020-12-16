//command:
	// g++ -fopenmp p3.cpp
 	// ./a.out

/*
Write a program for Cache unfriendly sieve of Eratosthenes and Cache friendly Sieve of Eratosthenes for enumerating prime numbers upto N and prove the correctness.

*/

#include<math.h>
#include<string.h>
#include<omp.h>
#include<iostream>
using namespace std;
double t,t1,t2,t3,t4,t5;
inline long Strike( bool composite[], long i,long stride, long limit )
{
   for( ; i<=limit; i+=stride )
      composite[i] = true;
   return i;
}

long CacheUnfriendlySieve( long n )
{
   long count = 0;
   long m = (long)sqrt((double)n);
   bool* composite = new bool[n+1];
   memset( composite, 0, n );

   t=omp_get_wtime();

   for( long i=2; i<=m; ++i )
      if( !composite[i] )
      {
         ++count;
         Strike( composite, 2*i, i, n );
      }
   for( long i=m+1; i<=n; ++i )
      if( !composite[i] )
         ++count;

   t1=omp_get_wtime()-t;

   delete[] composite;

   return count;
}

long CacheFriendlySieve( long n )
{
   long count = 0;
   long m = (long)sqrt((double)n);
   bool* composite = new bool[n+1];
   memset( composite, 0, n );
   long* factor = new long[m];
   long* striker = new long[m];
   long n_factor = 0;

   t2= omp_get_wtime();

   for( long i=2; i<=m; ++i )
      if( !composite[i] )
      {
         ++count;
         striker[n_factor] = Strike( composite, 2*i, i, m );
         factor[n_factor++] = i;
      }

   for( long window=m+1; window<=n; window+=m )
   {
      long limit = min(window+m-1,n);
      for( long k=0; k<n_factor; ++k )
         striker[k] = Strike( composite, striker[k], factor[k],limit );
      for( long i=window; i<=limit; ++i )
         if( !composite[i] )
            ++count;
   }

   t3 = omp_get_wtime() - t2;

   delete[] striker;
   delete[] factor;
   delete[] composite;

   return count;
}

long ParallelSieve( long n, int numThreads)
{
   long count = 0;
   long m = (long)sqrt((double)n);

   long n_factor = 0;
   long* factor = new long[m];

   t4= omp_get_wtime();

   omp_set_num_threads(numThreads);
#pragma omp parallel
   {
      bool* composite = new bool[m+1];
      long* striker = new long[m];

#pragma omp single
      {
         memset( composite, 0, m );
         for( long i=2; i<=m; ++i )
            if( !composite[i] )
            {
               ++count;
               striker[n_factor] = Strike( composite, 2*i, i, m );
               factor[n_factor++] = i;
            }
      }
      long base = -1;

      // Chops sieve into windows of size Å sqrt(n)
#pragma omp for reduction(+:count)
      for( long window=m+1; window<=n; window+=m )
      {
         memset(composite, 0, m);
         if(base != window)
         {
            //Must compute stiker from scratch
            base = window;
            for(long k=0; k<n_factor; ++k)
               striker[k] = (base+factor[k]-1)/factor[k] * factor[k] - base;
         }

         long limit = min(window+m-1, n) - base;
         for(long k=0; k<n_factor; ++k)
            striker[k] = Strike(composite, striker[k], factor[k], limit) - m;

         for(long i=0; i<=limit; ++i)
            if(!composite[i])
               ++count;
            base += m;
      }

      delete[] striker;
      delete[] composite;
   }

   t5 = omp_get_wtime() - t4;

   delete[] factor;
   return count;
}

int main()
{

   long N;
   cout<<"Enter the value of N : ";
   cin>>N;


   cout<<"\nCACHE UNFRIENDLY SEIVE"<<endl;
   cout<< "Count = "<<CacheUnfriendlySieve(N)<<endl;
   cout<< "Time = "<<t1<<endl;

   cout<<endl;

   cout<<"CACHE FRIENDLY SEIVE"<<endl;
   cout<< "Count = "<<CacheFriendlySieve(N)<<endl;
   cout<< "Time = "<<t3<<endl;

   cout<<endl;

   int numThreads;
   cout<<"PARALLEL SEIVE"<<endl;
   cout<<"Enter the number of threads: ";
   cin>>numThreads;
   cout<< "Count = "<<ParallelSieve(N, numThreads)<<endl;
   cout<< "Time = "<<t5<<endl;
   cout<< "\n";
}
