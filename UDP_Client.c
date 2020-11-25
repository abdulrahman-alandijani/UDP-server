/*
Developer: Abdulrahman
Email: AbdulramanAlandijani@my.unt.edu
Student ID: 11118717
Due date: 10-27-2020
Program Discription: UPD segment simulation; the program stimulates the process
of transmitting a message in a UPD segment on a client-server based network 
communication program.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
 
#define SERVER "129.120.151.96"//IP adress for cse03.cse.unt.edu
#define BUFLEN 512//Max length of buffer
 
//this structure represesnt the segments.
struct header
{
    unsigned short int source_port;
    unsigned short int destination_port;
    unsigned short int length;
    unsigned short int checksum;
    char payload[1024];
}segment;//structure tag

//functions declaration.
void die(char *s);
unsigned int find_checksum(struct header segment);
void display(struct header segment, FILE* log_file);

//argv[0] = executable file
//arg[1] = port number
//arg[2] = input file name
int main(int argc, char** argv)
{
    struct sockaddr_in si_other;
    int sockfd, i=0, slen=sizeof(si_other);
    char buf[BUFLEN];
    char message[BUFLEN];
    int port_number = atoi(argv[1]);
    FILE* out_file = fopen("client.out", "w");

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }

    //zero out the structure
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(port_number);
     
    //establish socket connection
    if (inet_aton(SERVER , &si_other.sin_addr) == 0) 
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }
 
    //Prepare the segment before transmitting.
    segment.destination_port = 1234;
    segment.source_port = 4321;
    segment.length = sizeof(segment);
    
    //open a file provided for reading the payload
    FILE* file = fopen(argv[2], "r");
    if(file == NULL)
    {
        printf("File doesn't exist");
        exit(1);
    }
    //read through the file
    fseek(file, 0L, SEEK_END);
    int number_bytes = ftell(file);
    fseek(file, 0L, SEEK_SET);
    char* read_buff = (char*)calloc(number_bytes, sizeof(char));
    fread(read_buff, sizeof(char), number_bytes, file);
    fclose(file);
    //store the payload in the structure
    strcpy(segment.payload, read_buff);
    //calculate the checksum
    segment.checksum = find_checksum(segment);

    printf("Sending segment to the server...\n");
    fprintf(out_file, "Sending segment to the server...\n");
    display(segment, out_file);
    //send the segment to the server.
    if(sendto(sockfd, &segment, sizeof(segment), 0, (struct sockaddr *) &si_other, slen) == -1)
    {
        die("sendto()");
    }

    //close socket and files
    close(sockfd);
    close(out_file);
    return 0;
}

void die(char *s)
{
    perror(s);
    exit(1);
}

//function used to calculate the checksum
unsigned int find_checksum(struct header segment)
{
    unsigned int sum = 0;
    unsigned int checksum;
    unsigned short int check[12];

    memcpy(check, &segment, 24);
    //summing loop
    for(int i = 0; i < 12; i++)
    {
            sum += check[i];
    }
    //calculate the sum
    checksum = sum >> 16;
    sum = sum & 0x0000FFFF;
    sum = checksum + sum;
    //add the sum to the check sum
    checksum = sum >> 16;
    sum = sum & 0x0000FFFF;
    checksum = checksum + sum;
    //return the calculated results 
    return 0xFFFF^checksum;
}

//function used to display results
void display(struct header segment, FILE* log_file)
{
        //display the reults to the user
        printf("Source port number:\t\t%d\n", segment.source_port);
        printf("Destination port number: \t%d\n", segment.destination_port);
        printf("Length: \t\t\t%d\n", segment.length);
        printf("Checksum: \t\t\t%d\n", segment.checksum);
        //print the results to the log file
        fprintf(log_file, "Source port number: \t\t%d\n", segment.source_port);
        fprintf(log_file, "Destination port number: \t%d\n", segment.destination_port);
        fprintf(log_file, "Length: \t\t\t\t\t%d\n", segment.length);
        fprintf(log_file, "Checksum: \t\t\t\t\t%d\n", segment.checksum);
}