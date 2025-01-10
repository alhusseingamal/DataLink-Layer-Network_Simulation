#define GO_BACK_N_RECEIVER_WS 1
#define TEXT_START_INDEX 5
#define ERROR_CODE_LENGTH 4
#define BYTE_LENGTH 8
#define FLAG_BYTES_COUNT 1

#define ERROR_CODE_MODIFICATION_BIT 0
#define ERROR_CODE_LOSS_BIT 1
#define ERROR_CODE_DUPLICATION_BIT 2
#define ERROR_CODE_DELAY_BIT 3

#define BYTE_STUFFING_ESCAPE_CHARACTER '/'
#define BYTE_STUFFING_FLAG '$'

#define MAX_LOSS_PROB 100

enum messageType
{
    COORD_MSG,
    NORMAL_MSG,
    TIMEOUT_MSG,
    PRINT_MSG
};

enum role
{
    SENDER,
    RECEIVER
};


enum frameType
{
    DATA,
    ACK,
    NACK
};

enum ErrorType
{
    CORRECT,
    LOSS,
    DELAYED,
    DUPLICATED,
    DELAYED_AND_DUPLICATED,
};

enum MessageState
{
    WAITING,
    SENT
};
