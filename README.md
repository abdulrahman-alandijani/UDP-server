To compile:
       server:
              make server
       client: 
              make client
it will create binary files for both codes

To Execute:
       server:
              ./server (port_numer)
       client:
              ./client (port_number) (input_file)
port number is needed in order to run 

To clean:
       make clean
it will remove all binary files

To Test:
       compile code and run executables with port number at command line, and an input file name for the client.
       Example:
              server: 
                     make server
                     ./server 1234
              client:
                     make client
                     ./client 1234 input.txt
              when done:
                     make clean