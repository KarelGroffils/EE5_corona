#include "network_transport.h"

char readMode;
char updateMode;

//available ID's 1 to 16
unsigned char IDTable[16];
unsigned char IDTableIndex;
unsigned char bufferFIFO[6];
unsigned char bufferFIFOIndex;
unsigned char localHostID = 0;
unsigned char externalHostID = 0;
unsigned char timeOutTimer = 0;
unsigned int refreshIDTimer = 400;

//#define CODE_PARITY_ERROR 255
//#define CODE_RX_ERROR 254
//#define CODE_SENDER_ID 253
//#define CODE_RECIPIENT_ID 252
//#define CODE_REQUEST_ID 251
//#define CODE_DATA_RESEND_REQUEST 250

void nt_initialize(void)
{
    readMode = false;
    updateMode = false;
    updateMode = true;
    IDTableIndex = 1;
}
void nt_updateIDTable()
{
    if(IDTableIndex == 16)
    {
        updateMode = false;
    }
    if(!readMode)
    {
        if(localHostID == 0)//looking for free available ID
        {
            if(IDTable[IDTableIndex] == 0)
            {
                localHostID = IDTableIndex;
                updateMode = false;
            }
        }
        readMode = true;
        trpBufferSendID[trpBufferSendIndex] = IDTableIndex;
        externalHostID = IDTableIndex;
        trpBufferSend[trpBufferSendIndex] = CODE_REQUEST_ID;
    }
}

void nt_chekData(unsigned char byte)
{
    //ID_update_algorithm
    if(updateMode)
    {
        nt_updateIDTable();
    }
    //check for codes
    if (byte == CODE_PARITY_ERROR)
    {
        if (readMode)
        {
            trpBufferSendID[trpBufferSendIndex] = externalHostID;
            trpBufferSend[trpBufferSendIndex] = CODE_DATA_RESEND_REQUEST;
        }
        bufferFIFO[bufferFIFOIndex-1] = 0;
        bufferFIFOIndex--;
    }
    else if (byte == CODE_RX_ERROR)
    {
        //remove value
        bufferFIFO[bufferFIFOIndex-1] = 0;
        bufferFIFOIndex--;
    }
    else if (byte == CODE_REQUEST_ID)
    {
        trpBufferSendID[trpBufferSendIndex] = externalHostID;
        trpBufferSend[trpBufferSendIndex] = localHostID;
        
        bufferFIFO[bufferFIFOIndex-1] = 0;
        bufferFIFOIndex--;
    }
    else if (byte == CODE_SENDER_ID)
    {
        if(updateMode)
        {
            IDTable[IDTableIndex] = bufferFIFO[bufferFIFOIndex-1];
            IDTableIndex++;
            readMode = false;
        }
        else
        {
            externalHostID = bufferFIFO[bufferFIFOIndex-1];
        }
        bufferFIFO[bufferFIFOIndex-1] = 0;
        bufferFIFOIndex--;
    }
    else if (byte == CODE_RECIPIENT_ID)
    {
        if(bufferFIFO[bufferFIFOIndex-1] == localHostID)
        {
            readMode = true;
            bufferFIFO[bufferFIFOIndex-1] = 0;
            bufferFIFOIndex--;
        }
    }
    else
    {
        //store in buffer
        bufferFIFOIndex++;
        if (bufferFIFOIndex > 5)
        {
             bufferFIFOIndex = 0;
        }
        bufferFIFO[bufferFIFOIndex] = byte;
        if (readMode)
        {
            trpBufferRecieve[trpBufferRecieveIndex] = bufferFIFO[bufferFIFOIndex-1];
            trpBufferRecieveIndex++;
        }
    }
}


void nt_UpdateTimer()
{
    timeOutTimer--;
    if (timeOutTimer < 1)
    {
        timeOutTimer = TIMEOUT;
        readMode = false;
    }
    refreshIDTimer--;
    if (refreshIDTimer < 1)
    {
        nt_updateIDTable();
        refreshIDTimer = REFRESH_ID;
        updateMode = true;
        IDTableIndex = 1;
    }
}