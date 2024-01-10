#include <stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include<sys/wait.h>
#include<string.h>
void send_file(FILE *fp, int socket){
    char data[1000]={0};
    while(fgets(data,1000,fp)!=NULL){
        send(socket,data,sizeof(data),0);
        bzero(data,1000);
    }
    send(socket,"EOF",strlen("EOF"),0);
    fclose(fp);
}
//----------------------------------------------------------------------------
int main(int argc,char *argv[]){
    int options, port;
    //see client code
//----------------------------------------------------------------------------
    while((options=getopt(argc,argv,":p:h"))!=-1){
        switch(options){
            case 'p':
                port=atoi(optarg);
                //printf("%d",port);
                break;
            case 'h':
            printf("\nhelp text\n");
        }
    }
//-----------------------------------------------------------------------------
int server_socket,client_socket;
struct sockaddr_in server_address,client_address;//declaring two variables of type struct sockaddr_in
server_socket=socket(AF_INET, SOCK_STREAM, 0);
memset(&server_address, '\0', sizeof(server_address));
server_address.sin_family=AF_INET;
server_address.sin_port=htons(port);
server_address.sin_addr.s_addr =htonl(INADDR_ANY);
//binding the socket// a bound socket has both an ip address and a port
bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));
int counter_test=0;
if(listen(server_socket,10)!=0){
    printf("Not listening");
    exit(0);
}
else{
    printf("Listening for clients ...\n\n");
socklen_t length;
length=sizeof(client_address);
//Server has to keep listening till exited
int read1;
char buffer[1000];
char buffer2[100];
memset(buffer,'\0',1000);
memset(buffer2,'\0',100);
int out_variable=0;
//----------------------------------------------------------------------------
while(1){
    client_socket=accept(server_socket,(struct sockaddr *)&client_address,&length);
    counter_test=counter_test+1;
//----------------------------------------------------------------------------------------------------    
//Connecting to client
    if(client_socket<0){
        printf("Not connected to client");
        exit(0);
    }
//-------------------------------------------------------------------------------------------------------------------    
//using fork    
    //printf("Connected with client %d\n",getpid());
    int process_id=fork();//creating the fork inside this infinite loop
    if (process_id<0){
        printf("The creation of child is unsuccessful");
    }
//----------------------------------------------------------------------------------------
//child processes  
    else if(process_id==0){
        int counter_matrix=0;
        int counter_kmeans=0;
        //printf("child process created\n");
        //this is what we are interested in we need to handle multiple clients as children
        //https://www.geeksforgeeks.org/design-a-concurrent-server-for-handling-multiple-clients-using-fork/
        while(1){
            //counter_matrix=counter_matrix+1;
            //counter_kmeans=counter_kmeans+1;
            memset(buffer,'\0',1000);
            read1=read(client_socket,buffer,1000);
            if(read1<0){
                printf("error");
                close(client_socket);
            }
            else{
                //client command is stored in buffer
                printf("Client %d from terminal %d commanded %s",out_variable,counter_test,buffer);
                    buffer2[0]='.';
                    buffer2[1]='/';
                for(int i=2;i<50;i++){
                        buffer2[i]=buffer[i-2];
                }
                int sum =0;
                sum=sum+getpid()+out_variable;
                //buffer 2 is the same as buffer1 prepended with ./
                //-----------------------------------------------------------------------------------------
                    //DO MATRIX
                    ///matinv -n 4 -P 1 -I fast
                    //CHANGE THIS IF TO CHECK WHETHER THE NAME OF THE FILE IS CORRECT INSTEAD OF JUST CHECKING FOR M
                    if(buffer2[2]=='m'){
                    counter_matrix=counter_matrix+1;
                    char buffer3[100];
                    sprintf(buffer3," > matinv_client%d_soln%d.txt",counter_test,counter_matrix);
                    strcat(buffer2,buffer3);
                    char* cmd=("%s",buffer2);
                    int sys=system(cmd);

                    char copy[150];
                    sprintf(copy,"cp matinv_client%d_soln%d.txt ./sresults",counter_test,counter_matrix);
                    char* cmdcopy=("%s",copy);
                    int syscopy=system(cmdcopy);
                    for (int i=0;i<10;i++){
                        i++;
                    }
                    out_variable=out_variable+1;


                    char fname[100];
                    sprintf(fname,"matinv_client%d_soln%d.txt",counter_test,counter_matrix);
                    int send3;
                    send3=send(client_socket,fname,strlen(fname),0);
                    for(int i=0;i<999999;i++){
                    	i=i+1;
                    }
                    FILE *file;
                    file=fopen(fname,"r");
                    send_file(file,client_socket);
                    //printf("\nfile successfully sent");
                    printf("\nResult file matinv_client%d_soln%d.txt is sent to the client\n\n",counter_test,counter_matrix);
                }
//---------------------------------------------------------------------------------------------------------
//DO KMEANS
//kmeans -f kmeans_data.txt -k 9

                else if (buffer2[2]='k'){
                    //printf("\nDo kmeans stuff");
                    counter_kmeans=counter_kmeans+1;
                    char read_data_file_buffer[999999];
                    int read2=read(client_socket,read_data_file_buffer,999999);
                    //Putting the data of the kmeans data from the buffer to a file
                    FILE *file = fopen("kmeans-data.txt", "w");
                    int results=fputs(read_data_file_buffer,file);
                    if (results==EOF){
                        printf("Client side could not write to the file");//writing the file from the buffer again on the client side
                    }
                    fclose(file);
                    printf("Received the kmeans data file");
                    char buffer7[100];
                    sprintf(buffer7," > kmeans_client%d_soln%d.txt",counter_test,counter_kmeans);
                    //printf("buffer7: %s",buffer7);
                    strcat(buffer2,buffer7);
                    //printf("\n%s buffer2\n ",buffer2);
                    char *cmdk=("%s",buffer2);
                    int sysk=system(cmdk);
                    char buffer8[100];
                    sprintf(buffer8,"cp kmeans-results.txt kmeans_client%d_soln%d.txt",counter_test,counter_kmeans);
                    char *cmdkresults=("%s",buffer8);
                    int syskresults=system(cmdkresults);
                    char copyk[150];
                    sprintf(copyk,"cp kmeans_client%d_soln%d.txt ./sresults",counter_test,counter_kmeans);
                    char* cmdcopyk=("%s",copyk);
                    int syscopyk=system(cmdcopyk);
                    out_variable=out_variable+1;

                    char fname[100];
                    sprintf(fname,"kmeans_client%d_soln%d.txt",counter_test,counter_kmeans);
                    int send3;
                    send3=send(client_socket,fname,strlen(fname),0);
                    for(int i=0;i<999999;i++){//give some delay after sending the filename
                    	i=i+1;
                    }                    
                    FILE *file1;
                    file1=fopen(fname,"r");
                    send_file(file1,client_socket);
                    //printf("\nfile successfully sent");
                    printf("\nResult file kmeans_client%d_soln%d.txt is sent to the client\n\n",counter_test,counter_kmeans);
                }
                //---------------------------------------------------------------------------------------------------------
                //DO NOTHING
                                else{
                                    printf("Not a valid command second first character either has to be m or k");
                                }
                 

            }
        }
    }
    else{
        close(client_socket);
    }//close(client_socket);
}
}
close(client_socket);
close(server_socket);
}
