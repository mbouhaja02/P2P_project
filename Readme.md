# Eirbone: Application d'échange de gros fichiers en P2P

## Description du projet
**Eirbone** est une application dédiée au partage de fichiers volumineux via la technologie peer-to-peer (P2P), permettant à chaque participant de fonctionner à la fois comme client et serveur. Le réseau se compose de **fournisseurs (seeders)**, détenant le fichier entier, et de **consommateurs (leechers)**, en cours de téléchargement. Cette approche favorise une distribution équilibrée des données.

## Travail demandé
Le projet implique la mise en œuvre d'un protocole pour la communication entre pairs et avec un Tracker centralisé. Les échanges s'effectuent via TCP, avec des messages en texte, sauf pour les données binaires.

### Version Centralisée
Un Tracker aide à localiser les fichiers et à découvrir d'autres pairs. Les utilisateurs doivent pouvoir configurer le port d'écoute du Tracker et les informations de connexion.

### Protocole de Communication Pair-Tracker
Les pairs annoncent leur présence et les fichiers partagés au Tracker, qui maintient une vue d'ensemble du réseau. Le protocole détaille les commandes pour annoncer, rechercher des fichiers, et se connecter aux pairs.

### Protocole de Communication Pair-Pair
Les pairs échangent des informations sur les parties du fichier possédées via un "BufferMap", définissant la demande et le transfert des pièces du fichier.

### Configuration et Organisation
- Le projet est à réaliser en groupes de 5 étudiants maximum.
- La programmation requiert l'usage de C pour le Tracker et de Java pour le client (peer).
- Un serveur HTTP embarqué peut être inclus pour la configuration et la visualisation.
- Un fichier `config.ini` est utilisé pour les paramètres du réseau et du programme.

### Considérations Techniques
- Les événements sont consignés dans un fichier de log.
- L'usage de thread pools est conseillé pour optimiser les performances.
- Les commandes du protocole se terminent par `\r\n` ou `\n`.
- L'utilisation de systèmes de versionnage est recommandée.
