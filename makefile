all:	mathserver3/server.exe mathserver3/matinvpar.exe mathserver3/kmeans.exe client/client.exe
mathserver3/server.exe: mathserver3/server.c
	gcc -o mathserver3/server mathserver3/server.c
mathserver3/matinvpar.exe: mathserver3/matinvpar.c
	gcc -o mathserver3/matinvpar mathserver3/matinvpar.c
mathserver3/kmeans.exe: mathserver3/kmeans.c
	gcc -o mathserver3/kmeans mathserver3/kmeans.c
client/client.exe: client/client.c
	gcc -o client/client client/client.c
