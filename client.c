#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include<pthread.h>
#include <netdb.h>

#define MAX_LINE 256
#define BUF_SIZE 256
#define SERVER_PORT 5432

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
  uint8_t dir_exists;

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

/*
struct File_List{
  int file_name_size;
  char file_list[];
};
typedef struct File_List Flist;
struct Files{

  int dir_exists;
  int no_files;
  Flist file_list[];
};
typedef struct Files files;
*/

void * clientThread(void *arg)
{
  printf("In thread\n");
    pthread_exit(NULL);
}
int main(int argc, char** argv){
  int i = 0;
 
  char message[MAX_LINE];
  //char buffer[1024];
  int clientSocket;
  char delim[] = " ";
  char ls[100];
char *enter_dir;
//char *ptr = strtok(message, delim);
  Pwd_List *file1;
  Pwd_List *pwd;
  pwd = malloc(sizeof(Pwd_List));
u_int32_t choice = 1;
          uint32_t ack;

  FILE *fp;
  struct hostent *hp;
  struct sockaddr_in sin;
  char *host;
  char buf[MAX_LINE];
  int s;
  int len, count=0;
  char buf1[MAX_LINE];
  if (argc==2) {
    host = argv[1];
  }
  else {
    fprintf(stderr, "usage: simplex-talk host\n");
    exit(1);
  }
  /* translate host name into peer's IP address */
  hp = gethostbyname(host);
  if (!hp) {
    fprintf(stderr, "simplex-talk: unknown host: %s\n", host);
    exit(1);
  }
  else{
    printf("Client's remote host: %s\n", argv[1]);
    }
  /* build address data structure */
  bzero((char *)&sin, sizeof(sin));
  sin.sin_family = AF_INET;
  bcopy(hp->h_addr, (char *)&sin.sin_addr, hp->h_length);
  sin.sin_port = htons(SERVER_PORT);
  /* active open */
  if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    perror("simplex-talk: socket");
    exit(1);
  }
  else
    printf("Client created socket.\n");

  if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
      perror("simplex-talk: connect");
      close(s);
      exit(1);
    }

  strcpy(message,"Connected.");
  if( send(s , message , sizeof(message) , 0) < 0)
        {
          printf("Send failed\n");
        }
  while(choice)
  {
    printf("1. CHANGE DIRECTORY \n");
    printf("2. LIST CONTENTS \n");
    printf("3. VIEW A FILE (cat) \n");
    printf("4. EDIT A FILE (vi) \n");
    printf("0. EXIT \n");
    printf("ENTER CHOICE \n");
    scanf("%d", &choice);

    choice = htonl(choice);
        if( send(s , &choice , sizeof(choice) , 0) < 0)
        {
          printf("CHOICE Send failed\n");
        }
    choice = ntohl(choice);
    switch(choice)
    {
      case 1:
      {
          char fld[MAX_LINE];
        printf("\nPLEASE ENTER FOLDER TO ENTER:");
        fflush(stdin);
      //  fgets(fld, MAX_LINE, stdin);
        strcpy(message, "dir1");
        printf("%s \n", message);
        if( send(s , message , sizeof(message) , 0) < 0)
        {
          printf("Send failed\n");
        }
        char *tmp;
        if(recv(s, pwd, sizeof(pwd), 0) < 0)
        {
          printf("Receive failed\n");
        }
        printf("RECEVIED \n");
        printf("NUMBER OF FILES: %s \n", pwd->noOfFiles);
       
        break;
      }
      case 2:
      {
        if(file1->noOfFiles == 0)
        {
          printf("EMPTY FOLDER \n");
        }
        else
        {
          printf("Data received: %s\n", file1->files);
          printf("%d\n", pwd->noOfFiles);
          printf("%s \n", pwd->files->Filename);
        }
        break;
      }
      case 3:
      {
        printf("ENTER FILENAME TO VIEW \n");
        fgets(message, MAX_LINE, stdin);
        if( send(s , message , sizeof(message) , 0) < 0)
        {
          printf("Send failed\n");
        }
        //char *tmp;
        FILE *fp1;
        fp1 = fopen(message, "w");
        if(fp1==NULL)
        {
        	printf("ERROR OPENING FILE \n");
        	exit(1);
		}
        while(1) {
    
        memset(buf, 0, sizeof(buf));

        if ((len = recv(s, buf, BUF_SIZE, 0)) < 0) {
            perror("receiver: recvfrom()");
            exit(1);
        }
        count += len;
        fwrite(buf,1,len,fp);
        printf("%d BYTES RECEIVED \n", len);
        if(len<BUF_SIZE)
        {
        	break;
		}
    	}
	 	printf("TOTAL %d BYTES RECEIVED \n", count);
        
    
    	fclose(fp);
        printf("RECEVIED \n");
        
        char cat[104]="cat ";
        strcat(cat, message);
        system(cat);
        break;
      }
      case 4:
      {
        printf("ENTER FILENAME TO EDIT \n");
        fgets(message, MAX_LINE, stdin);
        if( send(s , message , sizeof(message) , 0) < 0)
        {
          printf("Send failed\n");
        }
        //char *tmp;
        FILE *fp1;
        fp1 = fopen(message, "w");
        if(fp1==NULL)
        {
        	printf("ERROR OPENING FILE \n");
        	exit(1);
		}
        while(1) {
    
        memset(buf, 0, sizeof(buf));

        if ((len = recv(s, buf, BUF_SIZE, 0)) < 0) {
            perror("receiver: recvfrom()");
            exit(1);
        }
        count += len;
        fwrite(buf,1,len,fp);
        printf("%d BYTES RECEIVED \n", len);
        if(len<BUF_SIZE)
        {
        	break;
		}
    	}
	 	printf("TOTAL %d BYTES RECEIVED \n", count);
        
    
    	fclose(fp);
        printf("RECEVIED \n");
        
        
        char cat[103]="vi ";
        strcat(cat, message);
        system(cat);
        break;
      }
      case 0:
      {
        break;
      }
    }
  }

  /*
  printf("\nPLEASE ENTER FOLDER TO ENTER:\n");
fgets(message, sizeof(message), stdin);
printf("%s \n", message);
while(ptr != NULL)
{
fgets(enter_dir, 100, stdin);
ptr = strtok(NULL, delim);
}
printf("%s \n", enter_dir);
   if( send(clientSocket , enter_dir , strlen(enter_dir) , 0) < 0)
    {
            printf("Send failed\n");
    }

    //Read the message from the server into the buffer
    if(recv(clientSocket, file1, sizeof(Pwd_List), 0) < 0)
    {
       printf("Receive failed\n");
    }

    if(file1->dir_exists==1)
    {
      printf("ENTER \"ls\" to LIST FILES IN DIRECTORY \n");
      fgets(ls, 2, stdin);
      if(ls == "ls")
        printf("Data received: %s\n", file1->files);
      else
        printf("WRONG COMMAND \n");
    }
    else
    {
      fprintf(stderr, "DIR DOES NOT EXIST \n");
      exit(1);
    }
    //Print the received message
    */
    close(clientSocket);
 

  return 0;
}