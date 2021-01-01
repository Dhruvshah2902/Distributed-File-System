/*
    Team : Dhruv Shah (1741008)
           Mann Bilimoria (1741021)
           Param Raval (1741083)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/queue.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <inttypes.h>

#define SERVER_PORT 5432
#define MAX_LINE 256

char buf[MAX_LINE];

// ------- Structures for server ------- //

// STRUCT FILE_INFO
// Send File information to the structure

struct file_info{

    struct filename{
        uint8_t filename_size;
        char filename[];
    }Filename;
    uint8_t file_size;
};
typedef struct file_info File_Info;

// STRUCT PWD_LIST
// This struct sends the information of the current directory files to the client

struct pwd_list{
    uint8_t  dirExists;
    uint8_t noOfFiles;
    File_Info files[];
};
typedef struct pwd_list Pwd_List;

struct file_table{
    File_Info file;
    char location[];
};
typedef struct file_table File_Table;

struct file_block_table{
    File_Info file;
    uint8_t block;
    uint8_t blockNo;
};
typedef struct file_block_table File_Block_Table;

///// Linked List Implementation -------------------------------//

struct node{

    int data;
    uint32_t ipaddr;
    uint8_t port;
    struct node * next;
};
typedef struct node LList;

void insert(LList * head,int blockid,uint32_t ipaddr,uint8_t port){
    LList *item = malloc(sizeof(LList));
    item->data = blockid;
    item->ipaddr = ipaddr;
    item->port = port;
    item->next = NULL;
    LList *list = head;
    while ((list->next)!=NULL)
    {
        list = list->next;
    }
    list->next = item;
}

void print(LList * head){
    LList * list = head;
    while (list!=NULL)
    {
        printf("%d\n",list->ipaddr);
        printf("%d\n",list->port);
        printf("%d\n",list->data);
        list = list->next;
    }
    

}

void delete(LList * head,int blockid){
    LList * list = head;
    
    if(list->data == blockid){
        //list = list->next;
        //head = list;
    }
    else{
        LList * list2 = head;
        list = list->next;
        while (list!=NULL)
        {
            if(list->data == blockid){
                list = list->next;
                list2->next = list;
                break;
            }
            list2 = list2->next;
            list = list->next;
        }
    }
}

// -------------------------------------------------------- //

//Linked List implementation for File_Block_Table

struct node1{

    File_Block_Table *data;
    struct node1 *next;
};
typedef struct node1 LList1;


void initStructValues(File_Block_Table *newval, File_Block_Table *oldval){
    
    newval->block = oldval->block;
    newval->blockNo = oldval->blockNo;
    newval->file = oldval->file;
}

void insertFileBlock(LList1 *head,File_Block_Table *pfile){
    
    LList1 *item = malloc(sizeof *item);
    LList1 *list = head;
    
    item->data = pfile;
    item->next = NULL;
    printf("%d",head->data->block);
    while(list->next != NULL){
        list = list->next;
    }
    list->next->data = item->data;
 // printf("LIST noOfFiles %d \n", list->data->noOfFiles);

}

void printList(LList1 *temp)
{
    while(temp!=NULL)
    {
    printf("%d \n",temp->data->block);
    printf("%d \n",temp->data->blockNo);
    printf("%s \n",temp->data->file);
    temp=temp->next;
    }
}

void deleteStation(LList1 *head, File_Block_Table *pfile){

    LList1 *temp = head;
    LList1 *prev;

    if(temp != NULL && temp->data->block == pfile->block){
        head = temp->next;
        free(temp);
        return;
    }
    if(temp == NULL){
        return;
    }

    prev->next = temp->next;
    free(temp);
} 

// -----------------------------------------------------//

//          Data Node Algorithm for backup data node ---//

struct backupnode{
    int datanode1;
    int datanode2;
    int datanode3;
};
typedef struct backupnode backupnode;
LList freequeue;
struct backupnode * datanodeselect(LList * head){
    backupnode * nodelist = malloc(sizeof(backupnode));
    LList * listqueue = head;
    nodelist->datanode1 = pop(head);
    nodelist->datanode2 = pop(head);
    nodelist->datanode3 = pop(head);
    return nodelist;
    }

#define ack_msg_type 1
struct ack_block
{
    uint8_t type;
};
typedef struct ack_block ack_block;


// -------------------------------------------------------//

// Queue Implementation ----------------------------------//

struct node{

    int data;
    struct node * next;
};
typedef struct node LList;

void insertNode(LList * head,int blockid){
    LList *item = malloc(sizeof(LList));
    item->data = blockid;
    item->next = NULL;
    LList *list = head;
    while ((list->next)!=NULL)
    {
        list = list->next;
    }
    list->next = item;
}

void printNode(LList * head){
    LList * list = head;
    while (list!=NULL)
    {
        printf("%d",list->data);
        list = list->next;
    }
    

}

void deleteNode(LList * head,int blockid){
    LList * list = head;
    
    if(list->data == blockid){
        //list = list->next;
        //head = list;
    }
    else{
        LList * list2 = head;
        list = list->next;
        while (list!=NULL)
        {
            if(list->data == blockid){
                list = list->next;
                list2->next = list;
                break;
            }
            list2 = list2->next;
            list = list->next;
        }
    }
}


void addtofreequeue(LList * head){
    LList * listqueue = head;
    insertNode(listqueue);
}

int pop(LList * head){
    LList * list = head;
    LList * list2 = head;
    list = list->next;
    int x = list->data;
    return x;
        list = list->next;
        list2->next = list;
}

struct msgnode{

    int data;
    uint32_t ipaddr;
    uint8_t port;
    struct msgnode * next;
};
typedef struct msgnode msgLList;

void msginsert(msgLList * head,int blockid,uint32_t ipaddr,uint8_t port){
    msgLList *item = malloc(sizeof(LList));
    item->data = blockid;
    item->ipaddr = ipaddr;
    item->port = port;
    item->next = NULL;
    msgLList *list = head;
    while ((list->next)!=NULL)
    {
        list = list->next;
    }
    list->next = item;
}

void msgprint(msgLList * head){
    msgLList * list = head;
    while (list!=NULL)
    {
        printf("%d\n",list->ipaddr);
        printf("%d\n",list->port);
        printf("%d\n",list->data);
        list = list->next;
    }
    

}

void msgdelete(msgLList * head,int blockid){
    msgLList * list = head;
    
    if(list->data == blockid){
        //list = list->next;
        //head = list;
    }
    else{
        msgLList * list2 = head;
        list = list->next;
        while (list!=NULL)
        {
            if(list->data == blockid){
                list = list->next;
                list2->next = list;
                break;
            }
            list2 = list2->next;
            list = list->next;
        }
    }
}

int msgpop(msgLList * head){
    msgLList * list = head;
    msgLList * list2 = head;
    list = list->next;
    int x = list->data;
    return x;
    list = list->next;
    list2->next = list;
}
// ----------------------------------------------------------//

struct tcpthreadarg
{
    char *host;
};
typedef struct tcpthreadarg tcpthreadarg;

void * tcpthreadfunc(tcpthreadarg *tcpinfo){

    TCP_connect(tcpinfo->host);
}
// ----------------------------------------------------------//


// ---------Utility functions for data nodes ----------------//

int tcpblocktransfer(char *ipaddr,uint8_t port){
    char buf[MAX_SIZE];
    int len;
    int sock,new_s;
    struct hostent *hp;
    struct sockaddr_in sin;
    char str[INET_ADDRSTRLEN];    

    //printf("Host address : %s\n",host);

    // Building address data structure
    bzero((char *)&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(ipaddr);
    sin.sin_port = htons(port);
    if((sock = socket(PF_INET, SOCK_STREAM,0)) < 0){
        perror("Error building socket!\n");
        return 0;
    }
    else{
        //printf("Socket established for %s\n",host);
    }

    inet_ntop(AF_INET, &(sin.sin_addr), str, INET_ADDRSTRLEN);  
    //printf("Server is using address %s and port %d %d.\n", str, MC_PORT,sock);

    if((bind(sock,(struct sockaddr*)&sin,sizeof(sin))) < 0){
        perror("Error binding socket.\n");
        return -4;
    }
    else{
        //printf("Server bind done.\n");
    }    
    listen(sock,100);
    // enter file send code over here
    while(1) {
        
        if ((new_s = accept(sock, (struct sockaddr *)&sin, &len)) < 0) {
            perror("simplex-talk: accept");
            exit(1);
        }
        else{
            
        }
        
    }

    return 1;
}

// general code for tcp receive

int tcpblockreceive(char *ipaddr,uint8_t port){

    char buf[MAX_SIZE];
    struct hostent *hp;
    struct sockaddr_in sin;
    int s;
    int len;
    /* translate host name into peer's IP address */
    hp = gethostbyname(ipaddr);
    if (!hp) {
        fprintf(stderr, "simplex-talk: unknown host: %s\n",ipaddr);
        exit(1);
    }
    else
        printf("Client's remote host:\n");
    
    /* build address data structure */
    bzero((char *)&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    bcopy(hp->h_addr, (char *)&sin.sin_addr, hp->h_length);
    sin.sin_port = htons(port);
    /* active open */
    
    if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("simplex-talk: socket");
        exit(1);
    }
    else
        printf("Client created socket.\n");

    if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
        perror("simplex-talk: connect mcast");
        close(s);
        exit(1);
    }
    
    printf("x");


    return 1;
}

void tcpserverthreadroutine(tcpthreadarg * arg){
    tcpblockreceive(arg->ipaddr,arg->port,arg->head);
}

// ------------------------------------------------------//

LList *head = NULL;
int memory[10000];

int main(){

    uint32_t choice;
    int len;
    int sock, new_s;
    socklen_t length;
    struct hostent *hp;
    struct sockaddr_in sin;
    char str[INET_ADDRSTRLEN];

    Pwd_List *pwd;
    pwd = malloc(sizeof *pwd);
    bzero((char *)&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr("0.0.0.0");
    sin.sin_port = htons(SERVER_PORT);
    FILE *fp;
    pthread_t thread;
    pthread_attr_t tattr;
    pthread_attr_init(&tattr);
    pthread_attr_setdetachstate(&tattr,PTHREAD_CREATE_DETACHED);

    pthread_create(&thread ,&tattr, (void *)tcpthreadfunc,&tcphostaddr);


    //Make threads for the data nodes

    msgLList * msgqueuehead;
    pthread_t tcpserverthread;
    pthread_attr_t attr;
    tcpthreadarg tcpserverthreadarg;
    inet_pton(AF_INET,server,tcpserverthreadarg.ipaddr);
    tcpserverthreadarg.port = itoa(SERVER_PORT);
    tcpserverthreadarg.head = msgqueuehead;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    pthread_create(&tcpserverthread,&attr,tcpserverthreadroutine,&tcpserverthreadarg);
    if(pop(msgqueuehead)){
            tcpblocktransfer();
        }

    //Init the back up node
    backupnode bnode = malloc(sizeof *bnode);


    if((sock = socket(PF_INET,SOCK_STREAM, 0)) < 0){
        perror("Incorrect socket");
        exit(1);
    }

    inet_ntop(AF_INET, &(sin.sin_addr), str, INET_ADDRSTRLEN);

    if((bind(sock, (struct sockaddr*)&sin, sizeof(sin))) < 0){
        perror("Bind error");
        exit(1);
    }
    else{
        printf("Server bind done!\n");
    }

    listen(sock, 256);

    
    while(1){

        if(((new_s = accept(sock,(struct sockaddr *)&sin,&length)) < 0))
        {
            perror("Could not accept!\n");
            exit(1);
        }
        else{
            printf("Connection accepted!\n");
            if((len = recv(new_s,buf,sizeof(buf),0)) < 0){
                perror("Simplex talk : Error\n");
                exit(1);
            }
            printf("\n");
            fputs(buf,stdout);
        }

        if((len = recv(new_s,&choice,sizeof(choice),0)) < 0){
                perror("Simplex talk : Error\n");
                exit(1);
        }
        choice = ntohl(choice);
        printf("CHOICE: %d \n", choice);
        switch(choice){

            case 1:{
                uint8_t temp;
                if((len = recv(new_s,buf,sizeof(buf),0)) < 0){
                    perror("Simplex talk : Error\n");
                    exit(1);
                }
                fputs(buf,stdout);
                system("cd dir1;");
                fp = popen("ls dir1|wc -l", "r");
                while (fgets(buf, sizeof(buf), fp) != NULL) 
                {
                    pwd->noOfFiles = buf;
                    printf("%s", buf);
                }
                //void *ptr = &pwd;
                if( send(new_s , pwd , sizeof(pwd) , 0) < 0)
                {
                    printf("Send failed\n");
                }
                fclose(fp);
                
            break;
            }
            case 3:
                if((len = recv(new_s,buf,sizeof(buf),0)) < 0){
                    perror("Simplex-talk : Error\n");
                    exit(1);
                }
                fp = fopen(buf,"r");
                while((len = fread(buf, sizeof(char), MAX_LINE, fp))>0)
                {
                    if(send(new_s, buf, len, 0) < 0)
                    {
                        printf("ERROR: Failed to send file .\n");
                    }
                    fputs(buf,stdout);
                    bzero(buf, MAX_LINE);
                }
            break;

            case 4:
                if((len = recv(new_s,buf,sizeof(buf),0)) < 0){
                    perror("Simplex-talk : Error\n");
                    exit(1);
                }
                fp = fopen(buf,"r");
                while((len = fread(buf, sizeof(char), MAX_LINE, fp))>0)
                {
                    if(send(new_s, buf, len, 0) < 0)
                    {
                        printf("ERROR: Failed to send file .\n");
                    }
                    fputs(buf,stdout);
                    bzero(buf, MAX_LINE);
                }
            break;
        }

    


        close(new_s);        
    }
    close(sock);

    return 0;
}

