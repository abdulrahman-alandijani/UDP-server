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

int main(int argc, char** argv)
{
    struct sockaddr_in si_me, si_other;
     
    int sockfd, i, slen = sizeof(si_other) , recv_len;
    char buf[BUFLEN];
    int port_number = atoi(argv[1]);
    FILE* out_file = fopen("server.out", "w");

    //create a UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }
     
    //zero out the structure
    memset((char *) &si_me, 0, sizeof(si_me));
     
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(port_number);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
     
    //bind socket to port
    if(bind(sockfd, (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
    {
        die("bind");
    }
     
    //keep listening for data
    printf("Waiting for segment...\n");
    fprintf(out_file, "Waiting for segment...\n");
    
    //try to receive some data, this is a blocking call
    if ((recv_len = recvfrom(sockfd, &segment, sizeof(segment), 0, (struct sockaddr *) &si_other, &slen)) == -1)
    {
        die("recvfrom()");
    }

    //display and print meaningful message to file.
    printf("Segment recieved from the client:\n");
    fprintf(out_file, "Segment recieved from the client:\n");
    display(segment, out_file);
    unsigned int client_checksum = find_checksum(segment);
    //do a checksum check 
    //if the checksum recieved was the same as calculated.
    if(client_checksum == segment.checksum)
    {
        FILE* file = fopen("output.txt", "w");
        fputs(segment.payload, file);
        fclose(file);
    }
    else
    {
        printf("Segment was discarded\n");
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
    segment.checksum = 0;
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
    printf("Source port number: \t\t%d\n", segment.destination_port);
    printf("Destination port number: \t%d\n", segment.source_port);
    printf("Length: \t\t\t%d\n", segment.length);
    printf("Checksum: \t\t\t%d\n", segment.checksum);
    //print the results to the log file
    fprintf(log_file, "Source port number: \t\t%d\n", segment.destination_port);
    fprintf(log_file, "Destination port number: \t%d\n", segment.source_port);
    fprintf(log_file, "Length: \t\t\t\t\t%d\n", segment.length);
    fprintf(log_file, "Checksum: \t\t\t\t\t%d\n", segment.checksum);
}