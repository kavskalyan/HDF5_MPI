#include "hdf5.h"
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define FILE            "sample_data.h5"
#define DATASET         "DS"
#define DIM0            10
#define DIM1            10

int
main (int argc , char* argv[])
{
    hid_t       file, space, dset;          /* Handles */
    herr_t      status;
    hsize_t     dims[2] = {DIM0, DIM1};
    int         wdata[DIM0][DIM1],          /* Write buffer */
                rdata[DIM0][DIM1];          /* Read buffer */
    hsize_t         i, j;
    

    
    
    
    
    int rank, buf;  
    MPI_Status mpi_status;   
    MPI_Init(&argc, &argv);      
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    /* Process 0 sends and Process 1 receives */   
    if (rank == 0) {
        buf = 123456;
        
        for (i=0; i<DIM0; i++)
            for (j=0; j<DIM1; j++)
                wdata[i][j] = drand48();
        
        file = H5Fcreate (FILE, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

        space = H5Screate_simple (2, dims, NULL);
        printf("Create_dspace done\n");
        
        dset = H5Dcreate (file, DATASET, H5T_STD_I32LE, space, H5P_DEFAULT,
                    H5P_DEFAULT, H5P_DEFAULT);
        printf("Create dataset done\n");
        
        status = H5Dwrite (dset, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT,
                    wdata[0]);
        printf("Data write done\n");
        
        status = H5Dclose (dset);
        status = H5Sclose (space);
        status = H5Fclose (file);
         MPI_Send( &buf, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);  
            
    }   
    else if (rank == 1) {
        MPI_Recv( &buf, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &mpi_status );     
        printf( "Received %d\n", buf );   
        
        file = H5Fopen (FILE, H5F_ACC_RDONLY, H5P_DEFAULT);
        dset = H5Dopen (file, DATASET, H5P_DEFAULT);
    
        status = H5Dread (dset, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT,
                    rdata[0]);
    
        printf ("%s:\n", DATASET);
        for (i=0; i<DIM0; i++) {
            printf (" [");
            for (j=0; j<DIM1; j++)
                printf (" %3d", rdata[i][j]);
            printf ("]\n");
        
        }
        status = H5Dclose (dset);
        status = H5Fclose (file);
      
    }   
    MPI_Finalize();
    return 0;
    
    

}
