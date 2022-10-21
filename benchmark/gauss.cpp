#include<iostream>
#include <chrono>
#include <random>

void fillIdentity( float** matrix, int order)
{
     for (int i = 0; i < order; i++) {
 
        for (int j = 0; j < order; j++) {
 
            // Add '1' at the diagonal places of
            // the matrix to create a identity matrix
            if (j == i)
                matrix[i][j] = 1;
            else
                matrix[i][j] = 0;
    
        }
    }
}
void PrintMatrix(float** ar, int n,int m)
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
        std::cout << ar[i][j] << "  ";
        }
        printf("\n");
    }
    return;
}

// Function to Print inverse matrix
void PrintInverse(float** ar, int n, int m)
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            printf("%.3f  ", ar[i][j]);
        }
        printf("\n");
    }
    return;
}

void InverseOfMatrix(float** matrix, float** output_matrix, int order, int count_print)
{
  
    float temporary3, temporary2, temporary4;
 
    // PrintMatrix function to print the element
    // of the matrix.
    //printf("=== Matrix ===\n");
    //PrintMatrix(matrix, order, order);
 
    // Interchange the row of matrix,
    // interchanging of row will start from the last row
    for (int i = order - 1; i > 0; i--) {
        if (matrix[i - 1][0] < matrix[i][0]) {
            std::swap(output_matrix[i],output_matrix[i-1]);
            std::swap(matrix[i],matrix[i-1]);
        }
    }
   
    //Print matrix after interchange operations.
    //printf("\n=== Augmented Matrix ===\n");
    //PrintMatrix(matrix, order, order);
    //PrintMatrix(output_matrix, order, order);

    // Replace a row by sum of itself and a
    // constant multiple of another row of the matrix
    for (int i = 0; i < order; i++) {
        auto temporary1 = 1/matrix[i][i];
 
        for (int j = 0; j < i-1; j++) {
 
            {
 
                temporary2 = matrix[j][i] * temporary1;
                for (int k = 0; k < order; k++) {
                    
                    matrix[j][k] -= matrix[i][k] * temporary2;
                    output_matrix[j][k] -= output_matrix[i][k] * temporary2;
                }
            }
        }
         for (int j = i+1; j < order ; j++) {
 
            {
 
                temporary3 = matrix[j][i] * temporary1;
                for (int k = 0; k < order; k++) {
                    
                    matrix[j][k] -= matrix[i][k] * temporary3;
                    output_matrix[j][k] -= output_matrix[i][k] * temporary3;
                }
            }
        }
    }

 
    // Multiply each row by a nonzero integer.
    // Divide row element by the diagonal element
    for (int i = 0; i < order; i++) {
 
        temporary4 = 1/matrix[i][i];
        for (int j = 0; j < order; j++) {
 
            output_matrix[i][j] = output_matrix[i][j] * temporary4;
        }
    }
 
    // print the resultant Inverse matrix.
    
    /*
    if(count_print == 999)
    {
    printf("\n=== Inverse Matrix ===\n");
    PrintInverse(output_matrix, order,order);
    }
    */
    
    
    
    
  
 
    return;
}
int main()
{
   
    int order=200;
    int nb_occurences= 1000;
    float array_temp_matrix[order*order]={0};
    float array_matrix[order*order] ={0};
    float** temp_matrix = new float*[order];
    for (int i = 0; i < order; ++i)
        temp_matrix[i] = (array_temp_matrix + order*i);
    srand (1);
    for (int n = 0; n < order * order; ++n) {
        array_matrix[n]=rand() % 10 + 1;
    }


    float** matrix = new float*[order];
    for (int i = 0; i < order; ++i)
        matrix[i] = (array_matrix + order*i);
    float output_array_matrix[order*order] ={0};
    float** output_matrix = new float*[order];
    for (int i = 0; i < order; i++)
        output_matrix[i] = (output_array_matrix + order*i);
    

    auto start= std::chrono::steady_clock::now();
    for(int count=0; count<nb_occurences; count++)
    {
    fillIdentity(output_matrix,order);
    for(int i=0; i < order ; i++)
     for(int j=0; j < order ; j++)
    {
        temp_matrix[i][j]=matrix[i][j];
       
    }
    InverseOfMatrix(temp_matrix, output_matrix, order, count);
    }
    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    auto count_elapsed=elapsed.count();
    std::cout << count_elapsed << std::endl;
}
