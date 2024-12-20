# Spécifie le compilateur Java à utiliser
JAVAC = javac

# Spécifie l'exécutable Java
JAVA = java


# Spécifie le dossier contenant les fichiers sources Java
SRC_DIR = src/main/java/lammm/

# Spécifie le dossier où les fichiers .class seront générés
BIN_DIR = bin/

all: package run

package:
	@echo 
	@echo "Building the project"
	@echo "====================="
	@echo
	mvn package

clean:
	mvn clean

run:
	@echo 
	@echo "Running lammm.Peer"
	@echo "=================="
	@echo
	@java -cp target/peer-1.0.jar lammm.Peer

all-java: build-java run-java

run-java:
	@java -cp bin/ lammm.Peer

build-java: 
	$(JAVAC) -Xlint:unchecked -d $(BIN_DIR) $(SRC_DIR)*.java 

# Nettoie les fichiers .class
clean-java:
	rm -f $(BIN_DIR)/*.class
