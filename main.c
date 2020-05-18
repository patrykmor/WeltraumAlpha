#include <stdlib.h>
#include <stdbool.h>

#include "mpi/mpi.h"
typedef struct window Window;
#include "Color.h"

#include "Object.h"
#include "ObjectListNode.h"
#include "Window.h"

MPI_Datatype mpi_object, mpi_color;
Window *window;
int windowsHorizontally, windowsVertically;
int rank, windowsTotal;

void mpiSend();
void createMPIDataTypes();
void sendInfo();

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Expected arguments in form: windowshorizontally, windowsvertically");
        return 0;
    }
    windowsHorizontally = atoi(argv[1]);
    windowsVertically = atoi(argv[2]);
    int expectedWindows = windowsHorizontally * windowsVertically;
    if (expectedWindows == 0)
    {
        printf("Neither of the arguments can be equal 0!");
        return 0;
    }
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &windowsTotal);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (windowsTotal != expectedWindows)
    {
        printf("The arguments don't match the total number of started windows!");
        MPI_Finalize();
        return 0;
    }
    window = Window_create(rank, windowsHorizontally, windowsVertically);
    printf("Rank %d, x from %d to %d, y from %d to %d\n", rank, window->lowerX, window->upperX, window->lowerY, window->upperY);
    createMPIDataTypes();

    while (true)
    {

        Window_process_clicks(window);
        if (window->isClosed)
        {
            MPI_Abort(MPI_COMM_WORLD, 0);
            return 0;
        }
        Window_clear(window);
        sendInfo();
        Window_process_internal_influences(window);
        Window_move(window);
        Window_draw_local_circles(window);
        Window_present(window);
        MPI_Barrier(MPI_COMM_WORLD);
        Window_reset_local_objects(window);
    }
}

void sendInfo()
{
    for (int i = 0; i < windowsTotal; i++)
    {
        if (i == rank)
        {
            //printf("Communication rank %d\n", i);
            ObjectListNode **current = &(window->head);
            while ((*current) != NULL)
            {
                // printf("Sending\n");
                Object *temp = &((*current)->info);
                //printf("....\n");
                bool a = true;
                MPI_Barrier(MPI_COMM_WORLD);
                // printf(".....\n");
                MPI_Bcast(&a, 1, MPI_C_BOOL, i, MPI_COMM_WORLD);
                MPI_Barrier(MPI_COMM_WORLD);
                // printf("......\n");
                MPI_Bcast(temp, 1, mpi_object, i, MPI_COMM_WORLD);
                // printf("Sent/n");
                MPI_Barrier(MPI_COMM_WORLD);
                if (temp->x >= window->upperX +1 || temp->x < window->lowerX || temp->y < window->lowerY || temp->y >= window->upperY + 1)
                {
                    printf("Removing\n");
                    List_remove(current);
                }
                else
                {
                    current = &(*current)->next;
                }
            }
            bool a = false;
            MPI_Barrier(MPI_COMM_WORLD);
            MPI_Bcast(&a, 1, MPI_C_BOOL, i, MPI_COMM_WORLD);
            MPI_Barrier(MPI_COMM_WORLD);
        }
        else
        {
            while (true)
            {
                bool a = true;
                MPI_Barrier(MPI_COMM_WORLD);
                MPI_Bcast(&a, 1, MPI_C_BOOL, i, MPI_COMM_WORLD);
                MPI_Barrier(MPI_COMM_WORLD);
                if (!a)
                {
                    //printf("End receiving from rank %d\n", i);
                    break;
                }
                //printf("Receiving from rank %d\n", i);
                Object object;
                MPI_Bcast(&object, 1, mpi_object, i, MPI_COMM_WORLD);
                //printf("Received...\n");
                MPI_Barrier(MPI_COMM_WORLD);
                Window_process_external_object(window, &object);
            }
        }
    }
}

void createMPIDataTypes()
{
    //creating mpi color datatype
    MPI_Datatype colorTypes[4] = {MPI_INT, MPI_INT, MPI_INT, MPI_INT};
    int colorBlockLengths[4] = {1, 1, 1, 1};
    long colorOffsets[4];
    colorOffsets[0] = offsetof(Color, red);
    colorOffsets[1] = offsetof(Color, green);
    colorOffsets[2] = offsetof(Color, blue);
    colorOffsets[3] = offsetof(Color, alpha);
    MPI_Type_create_struct(4, colorBlockLengths, colorOffsets, colorTypes, &mpi_color);
    MPI_Type_commit(&mpi_color);

    //creating mpi object datatype
    MPI_Datatype objectTypes[9] = {MPI_LONG, MPI_DOUBLE, MPI_DOUBLE, MPI_INT, MPI_DOUBLE, MPI_DOUBLE, MPI_DOUBLE, MPI_DOUBLE, mpi_color};
    int objectBlockLengths[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    long objectOffsets[9];
    objectOffsets[0] = offsetof(Object, mass);
    objectOffsets[1] = offsetof(Object, x);
    objectOffsets[2] = offsetof(Object, y);
    objectOffsets[3] = offsetof(Object, radius);
    objectOffsets[4] = offsetof(Object, xSpeed);
    objectOffsets[5] = offsetof(Object, ySpeed);
    objectOffsets[6] = offsetof(Object, xAcceleration);
    objectOffsets[7] = offsetof(Object, yAcceleration);
    objectOffsets[8] = offsetof(Object, color);
    MPI_Type_create_struct(9, objectBlockLengths, objectOffsets, objectTypes, &mpi_object);
    MPI_Type_commit(&mpi_object);
}
