1. Open unix_project_final in terminal
2. Use the command make all
3. Open mathserver3 in terminal
4. To run the server use ./server -p <PORT>
5. Open client in terminal
6. To run the client use ./client -i 127.0.0.1 -p <PORT>
7. Example for matrix inversion  $matinvpar -n 4 -P 1 -I fast
8. Example for kmeans  $kmeans -k 9 -f kmeans-data.txt (This is because kmeans is not parallelized)
9. -s flag for muxbasic/muxadvanced is not specified only fork is used
10. No default values for kmeans, number of clusters and filename have to be specified
