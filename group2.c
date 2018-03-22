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

	int original_rank[7] = {1,2,3,4,5,6,7};
	int copy_rank[7] = {1,2,3,4,5,6,7};
	int similar_rank[7] = {2,5,3,1,4,6,7};
	int notequal_rank[7] = {1,2,3,5,7,8,9};

	/* create new groups */
	MPI_Group original_group, copy_group, similar_group, notequal_group;
	MPI_Group_incl(world_group, 7, original_rank, &original_group);
	MPI_Group_incl(world_group, 7, copy_rank, &copy_group);
	MPI_Group_incl(world_group, 7, similar_rank, &similar_group);
	MPI_Group_incl(world_group, 7, notequal_rank, &notequal_group);

	/* create new communicators */
	MPI_Comm original_comm, copy_comm, similar_comm, notequal_comm;
	MPI_Comm_create(MPI_COMM_WORLD, original_group, &original_comm);
	MPI_Comm_create(MPI_COMM_WORLD, copy_group, &copy_comm);
	MPI_Comm_create(MPI_COMM_WORLD, similar_group, &similar_comm);
	MPI_Comm_create(MPI_COMM_WORLD, notequal_group, &notequal_comm);

	/* Test Operation */
	if (world_rank == 0) {
		int ident, similar, unequal = 0;

		MPI_Group_compare(original_group, copy_group, &ident);
		MPI_Group_compare(original_group, similar_group, &similar);
		MPI_Group_compare(original_group, notequal_group, &unequal);

		printf("Compare original with copy: %d\n", ident);
		printf("Compare original with similar: %d\n", similar);
		printf("Compare original with notequal: %d\n", unequal);
	}

	MPI_Finalize();
}
