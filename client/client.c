#include<stdio.h>
#include<stdlib.h>
#include <arpa/inet.h> 
#include<unistd.h>//for optarg
#include <sys/socket.h>//for sockaddr structure
#include<string.h>

void write_file(int client_socket,char* fnam123){
    int n;
    FILE*fp;
    char buf[1000];
    fp=fopen(fnam123,"w");
    while (1)
    {
        n = recv(client_socket,buf,1000,0);
        if (n <= 0 || strcmp(buf, "EOF") == 0)
        {
            break;
        }
        fprintf(fp, "%s", buf);
        for (int i=0;i<1000;i++){
        i++;}
        bzero(buf,1000);
    }
    fclose(fp);
    return;
}


//-------------------------------------------------------------------------------------------
int main(int argc,char *argv[]){//argument count and argument vector
    int options;
    char server_ip[16];
    int i=0;
    int port;
//reading options
//---------------------------------------------------------------------------------------------------
//https://stackoverflow.com/questions/15173624/command-line-parser-port-number-retrieving


    while((options=getopt(argc,argv,":i:p:"))!=-1){
        switch(options){
            case 'i':
                //i=atoi(optarg);//atoi converts string to integer
                //printf("%d",i);this prints only the first 3 digits of the ip
                strcpy(server_ip,optarg);
                //printf("\n%s\n",server_ip);
                break;
            case 'p':
                port=atoi(optarg);
                //printf("\n %d",port);//port number is stored in port
                break;
        }
    }

//---------------------------------------------------------------------------------------------------

    int client_socket,server_socket;
    client_socket=socket(AF_INET, SOCK_STREAM, 0);//creating the socket server
    struct sockaddr_in server_address,client_address;//declaring two variables of type struct sockaddr_in
    //https://stackoverflow.com/questions/47525037/error-in-processing-ip-address-in-command-line-argument
    server_address.sin_family=AF_INET;
    server_address.sin_port=htons(port);
    server_address.sin_addr.s_addr = inet_addr(server_ip);
    socklen_t length;
    length=sizeof(client_address);
    if(connect(client_socket, (struct sockaddr *)&server_address,sizeof(server_address)) != 0){printf("Connection failed");}//this connects to the server
    printf("Connected to server\n");
    //printf("Enter a command for the server\n");//this is from the descriptor
    //"The client should display the server’s reply to the user, and prompt the user for the next input, until the user terminates the
//client program with Ctrl+C." this is why an infinite loop is used
char *answer_space2;
answer_space2=(char*)malloc(1000000* sizeof(char));
char answer_space[999999];//CHANGE
char read_sum_array[100];
int send1;
int read2;
int read3;
    while(1){
    	printf("Enter a command for the server: \n");
        server_socket=accept(client_socket,(struct sockaddr *)&client_address,&length);
        int variable=0;
        char enter_a_command[100];//to scan the sentence, fails if the first character is a newline
        scanf("%[^\n]%*c", enter_a_command);
        //printf("Enter a command for the server: \n");
        //https://stackoverflow.com/questions/30065675/what-does-scanf-nc-mean
        if (enter_a_command[0]=='k'){
            //send the command*
            //write the file into a data buffer*
            //send the data buffer to the server*
            send1=send(client_socket,enter_a_command,strlen(enter_a_command),0);//sending the command
            int send2;
            FILE* fp;
            long lSize;
            char *filebuffer;
            char fname[]="kmeans-data.txt";//File that has to be sent
            fp=fopen(fname,"rb");//opening the file
            if( !fp ) perror(fname),exit(1);
            fseek( fp , 0L , SEEK_END);
            lSize = ftell( fp );
            rewind( fp );
            filebuffer = calloc( 1, lSize+1 );
            if( !filebuffer ) fclose(fp),fputs("memory alloc fails",stderr),exit(1);
            if( 1!=fread( filebuffer , lSize, 1 , fp) )
                fclose(fp),free(filebuffer),fputs("entire read fails",stderr),exit(1);
            send2=send(client_socket,filebuffer,strlen(filebuffer),0);
            fclose(fp);
            free(filebuffer);


        }
        else if(enter_a_command[0]=='m'){
            send1=send(client_socket,enter_a_command,strlen(enter_a_command),0);//sending the command entered in client
        }
        //send1=send(client_socket,enter_a_command,strlen(enter_a_command),0);
        else{
            printf("Invalid command");
        }
        read3=read(client_socket,read_sum_array,100);
        char fnam123[150];
        sprintf(fnam123,"results/%s",read_sum_array);
        write_file(client_socket,fnam123);
        printf("received %s\n\n",fnam123);
        variable=variable+1;
    }
close(client_socket);


}
