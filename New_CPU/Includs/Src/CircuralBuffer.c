/**
* @file 
* @author Dawid Żądło
* @version 1.0
*
* @section DESCRIPTION
*
* 
*/



#include "main.h"



/**
  * @brief Initilize circular buffer of of size [BUFFERSIZE][MESSEGESIZE] defined in CircuralBuffer.h
  * @param NULL  
  * 
  * @retval Pointer to allocated memory 
  */
circuralbuffer* Circural_InitBuffer()
{   
    int ret = 0;
    circuralbuffer* buffer = malloc((sizeof(circuralbuffer)+(sizeof(char[BUFFERSIZE][MESSEGESIZE]))));

    if(buffer == NULL)
    {
        perror("Could not init allocate meamory for circural buffor\n");
        exit(EXIT_FAILURE);
    }
    
    ret = pthread_mutex_init(&buffer->mtx,NULL);
    if (ret!=0)
    {
        perror("Could not init mutex for circural buffor\n");
        exit(EXIT_FAILURE);
    }
    
    ret = pthread_cond_init(&buffer->cond,NULL);
    if (ret!=0)
    {
        perror("Could not init cond for circural buffor\n");
        exit(EXIT_FAILURE);
    }
    buffer->tail = -1;
    buffer->head = -1;

    return buffer;

}

/**
  * @brief Denitilize circular buffer of of size [BUFFERSIZE][MESSEGESIZE] defined in CircuralBuffer.h
  * @param allocated circuralbuffer object  
  * 
  * @retval None
  */
void Circural_DeInitBuffer(circuralbuffer* object [static  1]) 
{
    assert(*object!=NULL);
    
    free(*object);
    *object = NULL;
}

/**
  * @brief Adding new data to circural buffer 
  * @param pointer to circural buffer to which data should be writen 
  * @param string data to be writen to buffer 
  * 
  * @retval status: 0 - data has been writen to buffor
  *                 -2 - messege was to long to be writen to buffor  
  */

char Circural_Enque(circuralbuffer* object, const char data[])
{
    char tempbuffor[(MESSEGESIZE+30)];

    strncpy(tempbuffor,data,(MESSEGESIZE+30));

    int temp_head = (object->head)+1;
    if(temp_head==BUFFERSIZE)
    {
        temp_head=0;
    }



 
    if(strlen(tempbuffor)>MESSEGESIZE)
    {
        return -2;
    }
    strncpy((object->buffer[temp_head]),tempbuffor,MESSEGESIZE);
    object->head = temp_head;

    return 0;
}

/**
  * !@brief Reads data from circural buffor
  * !@param   pointer to circural buffor from which data should be read
  * 
  * !@retval status:  - "Nothing to read" - if buffor is empty 
  *                  - data from buffer  - if buffor has data
  */

char* Circural_Deque(circuralbuffer* object)
{
    static char temp[MESSEGESIZE];
    if(object->head == object->tail)
    {
        return "Nothing to read";
    }
    ++object->tail;
    if(object->tail==BUFFERSIZE)
    {
        object->tail =0;
    }

    strncpy(temp,(object->buffer[object->tail]),MESSEGESIZE);
    return temp;
}

/**
  * @brief Checking if there is new data to read in buffor 
  * @param   pointer to circural buffor from which data should be read
  * 
  * @retval status:  - 1 - nothing to read 
  *                  - 0 - new data to read
    */
char Circural_CheckIfSomeThingToRead(circuralbuffer* object)
{
    if(object->head == object->tail)
    {
        return 1;
    }
    if(object->head !=  object->tail)
    {
        return 0;
    }
}

