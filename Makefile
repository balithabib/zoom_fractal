#  D'apres le Makefile de :
#  	Auteur : Farès BELHADJ
#  	Email  : amsi@ai.univ-paris8.fr
#  	Date   : 29/01/2017

SHELL = /bin/sh
# définition des commandes utilisées
CC = g++
RM = rm -f
# déclaration des options du compilateur
CFLAGS = -std=c++14 -Wall -O3
CPPFLAGS = -I.
LDFLAGS = -pthread -lm $(shell pkg-config --libs opencv)

# définition des fichiers et dossiers
PROGNAME = Fractal
HEADERS =  fractal.hpp
SOURCES =  fractal.cpp main.cpp
OBJ = $(SOURCES:.cpp=.o)

all: $(PROGNAME)

$(PROGNAME): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $(PROGNAME)

%.o: %.cpp
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	@$(RM) -r $(PROGNAME) $(OBJ)

