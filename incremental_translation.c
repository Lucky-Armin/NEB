#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

int main(int argc, char *argv[])
{

  char *output_filename = "neb_inital_structures.xyz";
  char *input_filename1 = argv[1];
  char *input_filename2 = argv[2];

  if (argc != 3)
  {
    printf("Usage: %s  initial_minimum.xyz  end_minimum.xyz\nReads the starting and end geometry that correspond to 2 different energy minima of a structure.\nCreates intermediate structures for nudged elastic band calculations via incremental translation.\nWrites output to a file named \"%s\".\n", argv[0], output_filename);
    return 1;
  }

  FILE *fileptr1 = fopen(output_filename, "r");
  FILE *input_file1 = fopen(input_filename1, "r");
  FILE *input_file2 = fopen(input_filename2, "r");

  if (fileptr1 != NULL)
  {
    printf("File \"%s\" already exists!\n*******************************************\n****************  EXITING  ****************\n*******************************************\n", output_filename);
    //    return 1;
  }
  if (input_file1 == NULL || input_file2 == NULL)
  {
    printf("Error: unable to open input file(s) of type *.xyz\n*******************************************\n****************  EXITING  ****************\n*******************************************\n");
    return 1;
  }

  FILE *output_file = fopen(output_filename, "w");

  char line1[256];
  char line2[256];
  int natoms1 = 0;
  int natoms2 = 0;
  double lattice_x_1 = 0.0;
  double lattice_y_1 = 0.0;
  double lattice_z_1 = 0.0;
  double lattice_x_2 = 0.0;
  double lattice_y_2 = 0.0;
  double lattice_z_2 = 0.0;
  char title1[256];
  char title2[256];
  char element1[5];
  char element2[5];
  double x_1 = 0.0;
  double y_1 = 0.0;
  double z_1 = 0.0;
  double x_2 = 0.0;
  double y_2 = 0.0;
  double z_2 = 0.0;
  double centered_x_1 = 0.0;
  double centered_x_2 = 0.0;
  double centered_y_1 = 0.0;
  double centered_y_2 = 0.0;
  double centered_z_1 = 0.0;
  double centered_z_2 = 0.0;
  double new_x = 0.0;
  double new_y = 0.0;
  double new_z = 0.0;
  double i = 0;
  char user_input[256];
  double n = 11;

  printf("Number of desired intermediate structures (Default = 10): ");
  fgets(user_input, sizeof(user_input), stdin);
  user_input[strcspn(user_input, "\n")] = '\0';
  if (strlen(user_input) == 0)
  {
    n = atoi(user_input) + 11;
  }
  else
  {
    n = atoi(user_input) + 1;
  }

  for (i = 0; i < (n + 1); i++)
  {
    FILE *input_file1 = fopen(input_filename1, "r");
    FILE *input_file2 = fopen(input_filename2, "r");

    // reading and writing of the 1st line
    fgets(line1, sizeof(line1), input_file1);
    fgets(line2, sizeof(line2), input_file2);
    sscanf(line1, "%d %lf %lf %lf", &natoms1, &lattice_x_1, &lattice_y_1, &lattice_z_1);
    sscanf(line2, "%d %lf %lf %lf", &natoms2, &lattice_x_2, &lattice_y_2, &lattice_z_2);
    fprintf(output_file, "%s", line1);

    // reading and writing of the 2nd line
    fgets(line1, sizeof(line1), input_file1);
    fgets(line2, sizeof(line2), input_file2);
    sscanf(line1, "%s", title1);
    sscanf(line2, "%s", title2);
    fprintf(output_file, "ape is best\n");

    // reading and writing of the coordinate lines
    while (fgets(line1, sizeof(line1), input_file1) != NULL && fgets(line2, sizeof(line2), input_file2) != NULL)
    {
      sscanf(line1, "%s %lf %lf %lf", element1, &x_1, &y_1, &z_1);
      sscanf(line2, "%s %lf %lf %lf", element2, &x_2, &y_2, &z_2);

      centered_x_1 = (x_1 - lattice_x_1 * round(x_1 / lattice_x_1));
      centered_x_2 = (x_2 - lattice_x_2 * round(x_2 / lattice_x_2));
      centered_y_1 = (y_1 - lattice_y_1 * round(y_1 / lattice_y_1));
      centered_y_2 = (y_2 - lattice_y_2 * round(y_2 / lattice_y_2));
      centered_z_1 = (z_1 - lattice_z_1 * round(z_1 / lattice_z_1));
      centered_z_2 = (z_2 - lattice_z_2 * round(z_2 / lattice_z_2));

      new_x = centered_x_1 + (i / n) * ((centered_x_2 - centered_x_1) - lattice_x_1 * round((centered_x_2 - centered_x_1) / lattice_x_1));
      new_y = centered_y_1 + (i / n) * ((centered_y_2 - centered_y_1) - lattice_y_1 * round((centered_y_2 - centered_y_1) / lattice_y_1));
      new_z = centered_z_1 + (i / n) * ((centered_z_2 - centered_z_1) - lattice_z_1 * round((centered_z_2 - centered_z_1) / lattice_z_1));

      fprintf(output_file, "%s   %-13.9lf     %-13.9lf     %-13.9lf\n", element1, new_x, new_y, new_z);
    }
  }

  fclose(input_file1);
  fclose(input_file2);
  fclose(output_file);
  printf("Initial structures for nudged elastic band calculations generated successfully!\n");
  return 0;
}