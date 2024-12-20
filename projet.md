# Pairs

## Communication

    - protocole : TCP
    - messages : textes sauf données binaires
    - separation des messages : ` `
    - liste de champs entre `[]`
    - les pairs s'échangent des pieces du fichier
    - 

# Tracker

- connu par tous les pairs

# Utilisateur

# Protocole Réseau de Communications Pair- Tracker :

1. On annonce sa présence au tracker + liste des fichiers que l'on possède

# Liste des commandes

- `< announce` : annonce sa présence au tracker
  - `listen <port>` : spécifie le port d'écoute du tracker
  - `seed [<fichier> <longeur_fichier> <taille_d'une_piece> <hash_fichier> <...pareil pour les autres fichier>]` : spécifie les fichiers que l'on possède
  - `leech [<hash1> <hash2> <...>]` : spécifie les fichiers que l'on télécharge
  
- `< look [<critere1> <critere2> <...>]` : spécifie les critères de recherche
- `> list [<fichier> <longeur_fichier> <taille_d'une_piece> <hash_fichier> <...pareil pour les autres fichier>]`

- `< getfile <hash>` : demande un fichier au tracker
- `> peers <hash> [<ip1>:<port> <ip2>:<port>]` : liste des pairs possédant le fichier   

- `< interested <hash>` : demande à un pair de lui envoyer un fichier
- `> have <hash> <buffer>` : 
    > Le buffer est une séquence de bits dont le nombre correspond au nombre max de pièces du fichier. Chaque bit indique la présence ou non de la pièce correspondante dans le buffer

# Liste des taches

- spécifier le port d'écoute du tracker (*lancement* ou *fichier de configuration*)
  - On peut en proposer un par default

- Spécifier les informations de connexion du Tracker au Client P2P en ligne de commande ou dans un fichier de configuration.
 
- Message envoyé indiqué par caractère "<"
- Message reçu indiqué par caractère ">"
- implémenter un critère d'égalité
- Il faut pouvoir parametrer le nombre maximal de pairs connectés