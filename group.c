#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"

main(int argc, char **argv)
{
        MPI_Init(NULL, NULL);

        int world_rank, world_size;
        MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
        MPI_Comm_size(MPI_COMM_WORLD, &world_size);

        MPI_Group world_group;
        MPI_Comm_group(MPI_COMM_WORLD, &world_group);

        int prime_rank[7] = {1, 2, 3, 5, 7, 11, 13};
        int odd_rank[7] = {2, 4, 6, 8, 10, 12, 14};

        /* create new groups */
        MPI_Group prime_group, odd_group;
        MPI_Group_incl(world_group, 7, prime_rank, &prime_group);
        MPI_Group_incl(world_group, 7, odd_rank, &odd_group);

        /* create new communicators  */
        MPI_Comm prime_comm, odd_comm;
        MPI_Comm_create(MPI_COMM_WORLD, prime_group, &prime_comm);
        MPI_Comm_create(MPI_COMM_WORLD, odd_group, &odd_comm);

        /* worker program */
        int rank = -1, size = -1;
        if (MPI_COMM_NULL != prime_comm) {
                MPI_Comm_rank(prime_comm, &rank);
                MPI_Comm_size(prime_comm, &size);
                printf("WORLD RANK/SIZE: %d/%d \t PRIME RANK/SIZE: %d/%d\n",
                world_rank, world_size, rank, size);
        }
       if (MPI_COMM_NULL != odd_comm) {
                MPI_Comm_rank(odd_comm, &rank);
                MPI_Comm_size(odd_comm, &size);
                printf("WORLD RANK/SIZE: %d/%d \t ODD RANK/SIZE: %d/%d\n",$
                world_rank, world_size, rank, size);

        }

        /* Test Operation */
        MPI_Group union_group, intersect_group, diff_group;
        MPI_Group_union(prime_group, odd_group, &union_group);
        MPI_Group_intersection(prime_group, odd_group, &intersect_group);
        MPI_Group_difference(prime_group, odd_group, &diff_group);

        MPI_Comm union_comm, intersect_comm, diff_comm;
        MPI_Comm_create(MPI_COMM_WORLD, union_group, &union_comm);
        MPI_Comm_create(MPI_COMM_WORLD, intersect_group, &intersect_comm);
        MPI_Comm_create(MPI_COMM_WORLD, diff_group, &diff_comm);

        if (MPI_COMM_NULL != union_comm) {
                MPI_Comm_rank(union_comm, &rank);
                MPI_Comm_size(union_comm, &size);
                printf("WORLD RANK/SIZE: %d/%d \t UNION RANK/SIZE: %d/%d\n",
                world_rank, world_size, rank, size);
        }

        if (MPI_COMM_NULL != intersect_comm) {
                MPI_Comm_rank(intersect_comm, &rank);
                MPI_Comm_size(intersect_comm, &size);
                printf("WORLD RANK/SIZE: %d/%d \t INTERSECT RANK/SIZE: %d/%d\n",
                world_rank, world_size, rank, size);
        }

        if (MPI_COMM_NULL != diff_comm) {
                MPI_Comm_rank(diff_comm, &rank);
                MPI_Comm_size(diff_comm, &size);
                printf("WORLD RANK/SIZE: %d/%d \t PRIME DIFF ODD RANK/ %d/%d\n",
                world_rank, world_size, rank, size);
        }

        MPI_Finalize();
}