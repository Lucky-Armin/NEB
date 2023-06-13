#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>


int main(int argc, char *argv[]){


    char *output_filename = "neb_inital_structures.xyz";
    char *input_filename1 = argv[1];
    char *input_filename2 = argv[2];

    if (argc != 3) {
        printf("Usage: %s  initial_minimum.xyz  end_minimum.xyz\nReads the starting and end geometry that correspond to 2 different energy minima of a structure.\nCreates intermediate structures for nudged elastic band calculations via incremental translation.\nWrites output to a file named \"%s\".\n" , argv[0] , output_filename);
        return 1;
    }

    FILE *fileptr1=fopen(output_filename, "r");
    FILE *input_file1 = fopen(input_filename1, "r");
    FILE *input_file2 = fopen(input_filename2, "r"); 

    if(fileptr1 != NULL)
     {
       printf("File \"%s\" already exists!\n***********************\n******  EXITING  ******\n***********************\n" , output_filename);
//     return 1;
     }
     if(input_file1 == NULL || input_file2 == NULL)
     {
       printf("Error: unable to open input file(s) of type *.xyz\n***********************\n******  EXITING  ******\n***********************\n");
       return 1;
     }

    FILE *output_file = fopen(output_filename, "w");

    double atom1[3] = {-7.60685267 , -7.86371433 , -7.87758439 };
    double atom2[3] = {4.72194477 , 5.70601140 , 5.59311951 };
    double box_size[3] = {5.0 , 5.0 , 5.0 };

    double delta_x;
    double delta_y;
    double delta_z;

    atom1[0] = atom1[0] - box_size[0] * round(atom1[0] / box_size[0]);
    atom1[1] = atom1[1] - box_size[1] * round(atom1[1] / box_size[1]);
    atom1[2] = atom1[2] - box_size[2] * round(atom1[2] / box_size[2]);

    atom2[0] = atom2[0] - box_size[0] * round(atom2[0] / box_size[0]);
    atom2[1] = atom2[1] - box_size[1] * round(atom2[1] / box_size[1]); 
    atom2[2] = atom2[2] - box_size[2] * round(atom2[2] / box_size[2]);  

    delta_x = (atom2[0] - atom1[0]) - box_size[0] * round((atom2[0] - atom1[0]) / box_size[0]);
    delta_y = (atom2[1] - atom1[1]) - box_size[1] * round((atom2[1] - atom1[1]) / box_size[1]);
    delta_z = (atom2[2] - atom1[2]) - box_size[2] * round((atom2[2] - atom1[2]) / box_size[2]);

    printf("delta_x = %lf\n delta_y = %lf\n delta_z = %lf\n" , delta_x , delta_y , delta_z);

    char   line1[256];
    char   line2[256];
    int natoms1 = 0;
    int natoms2 = 0;
    double lattice_x_1 = 0.0;
    double lattice_y_1 = 0.0;
    double lattice_z_1 = 0.0;
    double lattice_x_2 = 0.0;
    double lattice_y_2 = 0.0;
    double lattice_z_2 = 0.0;
    char   titel1[256];
    char   titel2[256];
    char   element1[5];
    char   element2[5];
    double x_1 = 0.0;
    double y_1 = 0.0;
    double z_1 = 0.0;
    double x_2 = 0.0;
    double y_2 = 0.0;
    double z_2 = 0.0;
    double new_x = 0.0;
    double new_y = 0.0;
    double new_z = 0.0;


  while (fgets(line1, sizeof(line1), input_file1) != NULL && fgets(line2, sizeof(line2), input_file2) != NULL) {
   
    // reading and writing of the 1st line
  sscanf(line1 , "%d %lf %lf %lf" , &natoms1 , &lattice_x_1 , &lattice_y_1 , &lattice_z_1);
  sscanf(line2 , "%d %lf %lf %lf" , &natoms2 , &lattice_x_2 , &lattice_y_2 , &lattice_z_2);
  fprintf(output_file , "%s" , line1);

    // reading and writing of the 2nd line
  sscanf(line1 , "%s" , titel1);
  sscanf(line2 , "%s" , titel2);
  fprintf(output_file , "ape is best\n");
   
    // reading and writing of the coordinate lines

    while (fgets(line1, sizeof(line1), input_file1) != NULL && fgets(line2, sizeof(line2), input_file2) != NULL) {

    sscanf(line1 , "%s %lf %lf %lf" , element1 , &x_1 , &y_1 , &z_1);
    sscanf(line2 , "%s %lf %lf %lf" , element2 , &x_2 , &y_2 , &z_2);

    new_x = x_1 - lattice_x_1 * round(x_1 / lattice_x_1);
    new_y = y_1 - lattice_y_1 * round(y_1 / lattice_y_1);
    new_z = z_1 - lattice_z_1 * round(z_1 / lattice_z_1);    


    fprintf(output_file , "%s   %lf   %lf   %lf\n" , element1 , new_x , new_y , new_z);
    printf("%lf\n" , x_1);
    }

  }

















    fclose(input_file1);
    fclose(input_file2);
    fclose(output_file);
    printf("Initial structures for nudged elastic band calculations generated successfully!\n");
    return 0;
}